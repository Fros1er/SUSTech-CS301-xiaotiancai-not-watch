#include "application.h"

#include <string>

Application::Application()
    : _bg{lv_obj_create(lv_scr_act())} {
    lv_obj_align(_bg.get(), LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(_bg.get(), MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_obj_add_flag(_bg.get(), LV_OBJ_FLAG_HIDDEN);
}

void ApplicationFSM::register_application(const std::string &name, Application *app) {
    applications.emplace(name, app);
}

void ApplicationFSM::switch_to(const std::string &name) {
    if (cur_app != nullptr) {
        lv_obj_add_flag(cur_app->_bg.get(), LV_OBJ_FLAG_HIDDEN);
        cur_app->exit();
    }
    cur_app = applications[name];
    lv_obj_clear_flag(cur_app->_bg.get(), LV_OBJ_FLAG_HIDDEN);
    cur_app->enter();
}