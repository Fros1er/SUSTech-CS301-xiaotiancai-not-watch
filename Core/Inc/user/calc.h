#pragma once
#include "application.h"
#include "nrf_protocol.hpp"

void calc_answer_cb(const char *msg);

struct Calculator : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    Calculator();

    void exit() override {
        lv_textarea_set_text(ta, "");
    }

    public:
    static void textarea_event_handler(lv_event_t * e);
    static void clear_event_handler(lv_event_t * e);
    static Calculator *_instance;
    static Calculator *instance() {
        return _instance;
    }
};