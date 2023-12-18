#include "application.h"

#include <string>

Application::Application(const std::string &name)
    : _name{name}, _bg{lv_obj_create(lv_scr_act())} {
    lv_obj_align(_bg.get(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(_bg.get(), MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_obj_add_flag(_bg.get(), LV_OBJ_FLAG_HIDDEN);
}

Menu::Menu()
    : Application("Menu") {
}

static void event_handler(lv_event_t *e) {
    ApplicationFSM::instance().switch_to(*static_cast<const std::string *>(e->user_data));
}

void Menu::register_application(const std::string &name) {
    lv_obj_t *btn = lv_btn_create(_bg.get());
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *)&name);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, name.c_str());
    lv_obj_center(label);
}

ApplicationFSM::ApplicationFSM() {
}

void ApplicationFSM::init() {
    menu = new Menu();
    applications.emplace("Menu", menu);
}

void ApplicationFSM::register_application(Application *app) {
    applications.emplace(app->_name, app);
    menu->register_application(app->_name);
}

void ApplicationFSM::switch_to(const std::string &name) {
    if (cur_app != nullptr) {
        lv_obj_add_flag(cur_app->_bg.get(), LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(cur_app->_bg.get());
        cur_app->exit();
    }
    cur_app = applications[name];
    lv_obj_clear_flag(cur_app->_bg.get(), LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(cur_app->_bg.get());
    cur_app->enter();
}
