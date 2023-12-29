#pragma once

#include "application.h"
#include "dwt_delay.h"

struct Chat : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    lv_obj_t *dd;
    uint8_t room_id = 0;
    static Chat *_instance;

    std::unordered_map<uint8_t, std::vector<std::string>> room_msg_map;
    std::unordered_map<uint8_t, uint32_t> room_msg_idx_map;
    std::unordered_map<uint8_t, uint8_t> room_online_map;

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

    void make_room(uint8_t room);

    static void textarea_event_handler(lv_event_t * e);
    static void left_event_handler(lv_event_t * e);
    static void right_event_handler(lv_event_t * e);
    static void drop_event_handler(lv_event_t * e);
};
