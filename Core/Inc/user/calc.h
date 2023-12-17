#include "application.h"

struct Calculator : Application {
    lv_obj_t *ta;
    Calculator()
        : Application() {
        lv_obj_t *kb = lv_keyboard_create(_bg.get());

        ta = lv_textarea_create(lv_scr_act());
        lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
        lv_textarea_set_placeholder_text(ta, "Hello");
        lv_obj_set_size(ta, 140, 80);

        lv_keyboard_set_textarea(kb, ta);
    }

    void enter() override {
    }

    void exit() override {
        lv_textarea_set_text(ta, "");
    }
};