#pragma once

#include "application.h"
#include "dwt_delay.h"

struct Chat : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    lv_obj_t *dd;
    uint8_t room_id = 0;

    Chat();

    void exit() override {
        lv_textarea_set_text(ta, "");
    }
    void enter() override {
        nrf24l01_spi_init();
    }
    void tick() override;

    static void textarea_event_handler(lv_event_t * e);
    static void left_event_handler(lv_event_t * e);
    static void right_event_handler(lv_event_t * e);
    static void drop_event_handler(lv_event_t * e);
};
