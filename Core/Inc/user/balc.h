#pragma once
#include "application.h"
#include "nrf_protocol.hpp"

void balc_answer_cb(const char *msg);

struct Balculator : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    Balculator();

    void exit() override {
        lv_textarea_set_text(ta, "");
    }

    public:
    static void balc_event_handler(lv_event_t * e);
    static void clear_event_handler(lv_event_t * e);
    static Balculator *_instance;
    static Balculator *instance() {
        return _instance;
    }
};