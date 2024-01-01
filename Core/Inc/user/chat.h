#pragma once

#include "application.h"
#include "dwt_delay.h"
#include "calc.h"
#include "balc.h"

constexpr auto ROOM_NUMBER = 4;

struct Room {
    uint16_t msg_id;
    uint8_t room_addr;
    uint8_t online;
    std::vector<std::string> msg;
};

struct Chat : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    lv_obj_t *dd;
    lv_obj_t *de;

    uint8_t room_id = 0;
    static Chat *_instance;

    Room room[ROOM_NUMBER];
    uint8_t room_table[ROOM_NUMBER + 1];

    Chat();

    // check existence before use
    static Chat *instance() {
        return _instance;
    }

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
    static void invite_event_handler(lv_event_t * e);
    static void emoji_event_handler(lv_event_t *e);
};
