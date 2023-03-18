//
// Created by bjo on 3/11/23.
//

#include <Arduino.h>
#include "ui/ui.hpp"
#include "U8g2lib.h"
#include "config.hpp"
#include "ui/widget.hpp"
#include "ui/horizontalmenu.hpp"
#include "ui/matrix.hpp"
#include "ui/statusbar.hpp"
#include "ui/page.hpp"
#include <SPI.h>


U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2_lcd(U8G2_R2,
                                          LCD_CS,//LCD_CS,
                                          LCD_RS,
                                          LCD_RESET);


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
UI::Widgets::Horizontal_menu horizontal_menu(&u8g2_lcd, &root);
UI::Widgets::StatusBar status_bar(&u8g2_lcd, &root);
UI::Widgets::Page page_routing(&u8g2_lcd, &root);
UI::Widgets::Page page_settings(&u8g2_lcd, &root);
UI::Widgets::Page page_debug(&u8g2_lcd, &root);
UI::Widgets::Matrix matrix(&u8g2_lcd, &page_routing);

void set_current_position(uint8_t position) {
    page_routing.setVisible(position==0);
    page_settings.setVisible(position==1);
    page_debug.setVisible(position==2);
}

void enter_page(uint8_t position) {
    horizontal_menu.setFocus(false);
    page_routing.setFocus(true);
    page_settings.setFocus(false);
    page_debug.setFocus(false);
}

void re_enter_menu() {
    page_routing.setFocus(false);
    page_settings.setFocus(false);
    page_debug.setFocus(false);
    horizontal_menu.setFocus(true);
}

UI::Manager *current_manager = nullptr;

void update_router() {
    if (current_manager!= nullptr)
        current_manager->has_update_for_router();
}


void UI::Manager::init() {
    current_manager = this;
    initStrip();
    initLCD();
    initEncoder();
    root.setVisible(true);
    horizontal_menu.setVisible(true);
    set_current_position(0);
    horizontal_menu.setFocus(true);
    query_for_router_requested = true;

    horizontal_menu.addItem((char *) "ROUTING");
    horizontal_menu.addItem((char *) "SETTINGS");
    horizontal_menu.addItem((char *) "DEBUG");
    horizontal_menu.onHighlightedCall(&set_current_position);
    horizontal_menu.onSelectedCall(&enter_page);
    matrix.onExitCall(&re_enter_menu);
    matrix.onUpdateCall(&update_router);
}

void UI::Manager::has_update_for_router() {
    update_for_router_ready=true;
}

void UI::Manager::display_update() {
    snprintf(buffer, 50, "L [0:%4d] [1:%4d]", latency_cpu[0], latency_cpu[1]);
    status_bar.set_message(buffer);
    u8g2_lcd.clearBuffer();
    root.draw();
    u8g2_lcd.sendBuffer();
}

void UI::Manager::loop() {
    encoderPoll();
    display_update();
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
    root.move_right();
}

void UI::Manager::encoder_left() {
    root.move_left();
}

void UI::Manager::encoder_click() {
    root.click();
}

routing_matrix *UI::Manager::current_route() {
    return matrix.getMatrix();
}
