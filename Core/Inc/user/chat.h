#pragma once

#include "application.h"
#include "utils.h"
#include "dwt_delay.h"
#include <vector>

struct Chat : public Application {
    lv_obj_t *ta;
    lv_obj_t *label;
    std::vector<std::string> msg_list;

    Chat();

    void exit() override {
        lv_textarea_set_text(ta, "");
    }
    void enter() override {
        nrf24l01_spi_init();
    }
    void tick() override;
};