#include "application.h"

#include <string>

#include "nrf_protocol.hpp"
#include "stm32f1xx_hal.h"
#include "user_rtc.h"

static lv_style_t titlebar_style, bg_style;
extern std::string user_names[];
uint32_t timer;

Application::Application(const std::string &name)
    : _name{name} {
    _bg = lv_obj_create(lv_scr_act());
    lv_obj_add_style(_bg, &bg_style, 0);
    lv_obj_align(_bg, LV_ALIGN_OUT_TOP_MID, 0, MENU_BAR_HEIGHT);
    lv_obj_set_size(_bg, MY_DISP_HOR_RES, MY_DISP_VER_RES - MENU_BAR_HEIGHT);
    lv_obj_add_flag(_bg, LV_OBJ_FLAG_HIDDEN);
}

Menu::Menu()
    : Application("Menu") {
    info_label = lv_label_create(_bg);
    lv_obj_align(info_label, LV_ALIGN_TOP_MID, 60, -10);
    lv_obj_set_height(info_label, 20);
    lv_label_set_recolor(info_label, true);
    lv_label_set_text_fmt(info_label, "#ff00ff %s# #ff0000@XTC ~#", user_names[device_name].c_str());

    time_label = lv_label_create(_bg);
    update_time();
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, -40, -10);
    lv_obj_set_height(time_label, 20);
}

void Menu::update_time() {
    snprintf(time_buf, 32, "20%02d/%02d/%02d %02d:%02d:%02d", rtc_date.Year, rtc_date.Month, rtc_date.Date, rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
    lv_label_set_text_static(time_label, time_buf);
}

static void event_handler(lv_event_t *e) {
    ApplicationFSM::instance().switch_to(*static_cast<const std::string *>(e->user_data));
}

void Menu::register_application(const std::string &name) {
    lv_obj_t *btn = lv_btn_create(_bg);
    lv_obj_set_pos(btn, 0, 20 + 50 * (app_num));
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *)&name);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text_static(label, name.c_str());
    lv_obj_center(label);
    app_num++;
}

static void back_to_menu(lv_event_t *e) {
    ApplicationFSM::instance().switch_to("Menu");
}

ApplicationFSM::ApplicationFSM() {
}

void ApplicationFSM::init() {
    menu = new Menu();
    applications.emplace("Menu", menu);
    _title_bar = lv_obj_create(lv_scr_act());
    lv_obj_align(_title_bar, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_obj_set_size(_title_bar, MY_DISP_HOR_RES, MENU_BAR_HEIGHT);

    lv_style_init(&titlebar_style);
    lv_style_set_bg_color(&titlebar_style, lv_color_hex(0xB3E5FC));
    lv_style_set_border_width(&titlebar_style, 0);
    lv_style_set_radius(&titlebar_style, 0);
    lv_obj_add_style(_title_bar, &titlebar_style, 0);
    lv_obj_clear_flag(_title_bar, LV_OBJ_FLAG_SCROLLABLE);

    _title = lv_label_create(_title_bar);
    lv_obj_set_align(_title, LV_ALIGN_CENTER);

    _back_btn = lv_btn_create(_title_bar);
    lv_obj_add_event_cb(_back_btn, back_to_menu, LV_EVENT_CLICKED, NULL);
    lv_obj_align(_back_btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_height(_back_btn, MENU_BAR_HEIGHT - 10);
    lv_obj_set_style_shadow_width(_back_btn, 0, 0);
    lv_obj_t *label = lv_label_create(_back_btn);
    lv_label_set_text_static(label, "Menu");
    lv_obj_center(label);

    lv_style_init(&bg_style);
    lv_style_set_border_width(&bg_style, 0);
    lv_style_set_radius(&bg_style, 0);

    timer = 0;
}

void ApplicationFSM::register_application(Application *app) {
    applications.emplace(app->_name, app);
    menu->register_application(app->_name);
}

void ApplicationFSM::alert_cb(const char* msg){
    ApplicationFSM &fsm = ApplicationFSM::instance();
    if (fsm.cur_app != nullptr) {
        lv_label_set_text(fsm._title, msg);
        lv_obj_add_flag(fsm._back_btn, LV_OBJ_FLAG_HIDDEN);
        timer = 50;
    } 
}

void ApplicationFSM::switch_to(const std::string &name) {
    if (cur_app != nullptr) {
        lv_obj_add_flag(cur_app->_bg, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(cur_app->_bg);
        cur_app->exit();
    }
    cur_app = applications[name];
    lv_obj_clear_flag(cur_app->_bg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(cur_app->_bg);
    cur_app->enter();
    lv_label_set_text_static(_title, cur_app->_name.c_str());
    if (cur_app == menu) {
        lv_obj_add_flag(_back_btn, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(_back_btn, LV_OBJ_FLAG_HIDDEN);
    }
}

void ApplicationFSM::tick() {
    if (cur_app != nullptr) {
        cur_app->tick();
        if (timer > 0) {
            if (--timer == 0) {
                lv_obj_clear_flag(_back_btn, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text_static(_title, cur_app->_name.c_str());
            }
        }
    }
}

void rtc_alarm_cb() {
}
void rtc_sec_cb() {
    auto &menu = ApplicationFSM::instance().menu;
    if (menu != nullptr) {
        menu->update_time();
    }
}
