//
// Created by bjo on 3/11/23.
//

#include <Arduino.h>
#include "ui/ui.hpp"
#include "U8g2lib.h"
#include "config.hpp"
#include "ui/widget.hpp"
#include "ui/horizontalmenu.hpp"
#include "ui/verticalmenu.hpp"
#include "ui/matrix.hpp"
#include "ui/statusbar.hpp"
#include "ui/page.hpp"
#include "utils.hpp"
#include "storage.hpp"
#include <SPI.h>

Storage storage;
U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2_lcd(U8G2_R2, LCD_CS, LCD_RS, LCD_RESET);

void UI::Manager::initLCD() {
    SPI.setRX(0);
    SPI.setCS(1);
    SPI.setSCK(LCD_CLOCK);
    SPI.setTX(LCD_MOSI);

    SPI.begin();
    SPI.beginTransaction(SPISettings(64000000, MSBFIRST, SPI_MODE0));
    u8g2_lcd.begin();
    u8g2_lcd.setContrast(180);  // This is extremely important
    u8g2_lcd.clearBuffer();
    u8g2_lcd.firstPage();
    u8g2_lcd.setFontMode(0);
    u8g2_lcd.clearBuffer();
    u8g2_lcd.drawStr(0, 10, "A strange game.");
    u8g2_lcd.drawStr(0, 20, "The only winning move");
    u8g2_lcd.drawStr(0, 30, " is not to play");
    u8g2_lcd.sendBuffer();
}

UI::Widget root = UI::Widget(&u8g2_lcd);
UI::Widgets::Horizontal_menu main_menu(&u8g2_lcd, &root);
UI::Widgets::StatusBar status_bar(&u8g2_lcd, &root);

UI::Widgets::Page page_routing(&u8g2_lcd, &root);
UI::Widgets::Matrix matrix(&u8g2_lcd, &page_routing);

UI::Widgets::Page page_settings(&u8g2_lcd, &root);
UI::Widgets::Vertical_menu settings_menu(&u8g2_lcd, &page_settings);

UI::Widgets::Page page_debug(&u8g2_lcd, &root);


void set_current_position(uint8_t position) {
    page_routing.setVisible(position == 0);
    page_settings.setVisible(position == 1);
    page_debug.setVisible(position == 2);
}

void enter_page(uint8_t position) {
    main_menu.setFocus(false);
    page_routing.setFocus(position == 0);
    page_settings.setFocus(position == 1);
    page_debug.setFocus(position == 2);
}

void re_enter_menu() {
    page_routing.setFocus(false);
    page_settings.setFocus(false);
    page_debug.setFocus(false);
    main_menu.setFocus(true);
}

void reset_ui() {
    settings_menu.set_selected_item_to(0);
    settings_menu.set_highlighted_item_to(0);
    re_enter_menu();
    main_menu.set_highlighted_item_to(0);
    main_menu.set_selected_item_to(0);
}

void settings_menu_options(uint8_t option) {
    switch (option) {
        case 1: // Save
            storage.save_routing_matrix(matrix.getMatrix());
            matrix.set_dirty(false);
            reset_ui();
            break;
        case 2: // Load
            storage.load_routing_matrix(matrix.getMatrix());
            matrix.update();
            matrix.set_dirty(false);
            reset_ui();
            break;
        default:
            re_enter_menu();
            break;
    }
}

UI::Manager *current_manager = nullptr;

void update_router() {
    if (current_manager != nullptr)
        current_manager->has_update_for_router();
}

void update_dirty(bool dirty) {
    if (dirty)
        UI::Manager::dirtyStrip();
    else
        UI::Manager::normalStrip();
}

void UI::Manager::init() {
    current_manager = this;
    storage.init();
    storage.load_routing_matrix(matrix.getMatrix());
    initStrip();
    initLCD();
    initEncoder();
    root.setVisible(true);
    main_menu.setVisible(true);
    set_current_position(0);

    settings_menu.set_selected_item_to(0);
    settings_menu.set_highlighted_item_to(0);
    re_enter_menu();
    main_menu.set_highlighted_item_to(0);
    main_menu.set_selected_item_to(0);

    main_menu.addItem((char *) "ROUTING");
    main_menu.addItem((char *) "SETTINGS");
    main_menu.addItem((char *) "DEBUG");
    main_menu.onHighlightedCall(&set_current_position);
    main_menu.onSelectedCall(&enter_page);

    // Router page
    matrix.onExitCall(&re_enter_menu);
    matrix.onUpdateCall(&update_router);
    matrix.onDirtyCall(&update_dirty);
    matrix.update(); // We update as we loaded

    // Settings page
    settings_menu.addItem((char *) "Back");
    settings_menu.addItem((char *) "Save");
    settings_menu.addItem((char *) "Load");
    settings_menu.onSelectedCall(&settings_menu_options);
}

void UI::Manager::has_update_for_router() {
    update_for_router_ready = true;
}

char subuf[20] = {0};

void UI::Manager::display_update() {
    if (matrix.get_dirty())
        snprintf(subuf, 20, " Mod");
    else
        snprintf(subuf, 20, "");
    if (latency_watch)
        snprintf(buffer, 50, "L [0:%4d] [1:%4d]%s", latency_cpu[0], latency_cpu[1], subuf);
    else
        snprintf(buffer, 50, "%s", subuf);
    status_bar.set_message(buffer);
    u8g2_lcd.clearBuffer();
    root.draw();
    u8g2_lcd.sendBuffer();
}

uint32_t last_update_time = CURRENT_TIME_MS;

bool updated_last_time = false;

void UI::Manager::loop() {
    encoderPoll();
    // We wait 10ms before trying to update if we had an interaction if not we wait 100ms
    if ((CURRENT_TIME_MS - last_update_time) > (updated_last_time ? 10 : 100)) {
        display_update();
        updated_last_time = false;
        last_update_time = CURRENT_TIME_MS;
    }
    if (updated) {
        last_update_time = CURRENT_TIME_MS;
        updated_last_time = true;
        updated = false;
    }
}

void UI::Manager::set_latency(uint8_t cpu, uint32_t value) {
    latency_cpu[cpu] = value;
}

bool UI::Manager::query_for_router() {
    if (query_for_router_requested) {
        query_for_router_requested = false;
        return true;
    }
    return false;
}

bool UI::Manager::update_for_router() {
    if (update_for_router_ready) {
        update_for_router_ready = false;
        return true;
    }
    return false;
}


void UI::Manager::set_routing_response(routing_matrix &new_matrix) {
    matrix.set_matrix(new_matrix);
}

void UI::Manager::encoder_right() {
    updated = true;
    root.move_right();
}

void UI::Manager::encoder_left() {
    updated = true;
    root.move_left();
}

void UI::Manager::encoder_click() {
    updated = true;
    root.click();
}

routing_matrix *UI::Manager::current_route() {
    return matrix.getMatrix();
}
