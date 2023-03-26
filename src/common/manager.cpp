//
// Created by bjo on 3/11/23.
//

#include "ui/manager.hpp"
#include "U8g2lib.h"
#include "ui/statusbar.hpp"
#include "../../lib/storage/storage.hpp"

void UI::Manager::initLCD() {
    u8g2_lcd.begin();
    u8g2_lcd.setContrast(180);  // This is extremely important
    u8g2_lcd.clearBuffer();
    u8g2_lcd.firstPage();
    u8g2_lcd.setFontMode(0);
    u8g2_lcd.clearBuffer();
    u8g2_lcd.setDrawColor(1);
    u8g2_lcd.setFont(u8g2_font_5x8_mf);
    u8g2_lcd.drawStr(0, 10, "A strange game.");
    u8g2_lcd.drawStr(0, 20, "The only winning move");
    u8g2_lcd.drawStr(0, 30, " is not to play");
    u8g2_lcd.sendBuffer();
}

UI::Manager *current_manager;

void set_current_position(uint8_t position) {
    current_manager->page_routing.setVisible(position == 0);
    current_manager->page_settings.setVisible(position == 1);
    current_manager->page_debug.setVisible(position == 2);
}

void enter_page(uint8_t position) {
    current_manager->main_menu.setFocus(false);
    current_manager->page_routing.setFocus(position == 0);
    current_manager->page_settings.setFocus(position == 1);
    current_manager->page_debug.setFocus(position == 2);
}

void re_enter_menu() {
    current_manager->page_routing.setFocus(false);
    current_manager->page_settings.setFocus(false);
    current_manager->page_debug.setFocus(false);
    current_manager->main_menu.setFocus(true);
}

void reset_ui() {
    current_manager->settings_menu.set_selected_item_to(0);
    current_manager->settings_menu.set_highlighted_item_to(0);
    re_enter_menu();
    current_manager->main_menu.set_highlighted_item_to(0);
    current_manager->main_menu.set_selected_item_to(0);
}

void settings_menu_options(uint8_t option) {
    switch (option) {
        case 1: // Save
            storage.save_routing_matrix(current_manager->matrix.getMatrix());
            current_manager->matrix.set_dirty(false);
            reset_ui();
            break;
        case 2: // Load
            storage.load_routing_matrix(current_manager->matrix.getMatrix());
            current_manager->matrix.update();
            current_manager->matrix.set_dirty(false);
            reset_ui();
            break;
        default:
            re_enter_menu();
            break;
    }
}

void debug_menu_options(uint8_t option) {
    switch (option) {
        default:
            re_enter_menu();
            break;
    }
}


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

    debug_menu.addItem((char *) "Back");
    debug_menu.onSelectedCall(&debug_menu_options);
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

void UI::Manager::has_update_for_router() {
    update_for_router_ready = true;
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

void UI::Manager::set_routing_response(RoutingMatrix &new_matrix) {
    current_manager->matrix.set_matrix(new_matrix);
}

RoutingMatrix UI::Manager::current_route() {
    return current_manager->matrix.getMatrix();
}

UI::Manager::Manager(U8G2 &u8G2Lcd) : u8g2_lcd(u8G2Lcd),
    root(u8G2Lcd),
    main_menu(root),
    status_bar(root),
    page_routing(root),
    matrix(page_routing),
    page_settings(root),
    settings_menu(page_settings),
    page_debug(root),
    debug_menu(page_debug)
{
}
