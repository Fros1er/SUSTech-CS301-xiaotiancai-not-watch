#include "application.h"

struct Calculator : public Application {
    lv_obj_t *ta;
    Calculator();

    void enter() override {
    }

    void exit() override {
        lv_textarea_set_text(ta, "");
    }
};