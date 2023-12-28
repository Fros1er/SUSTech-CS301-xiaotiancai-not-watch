#include "application.h"
#include "utils.h"
#include "dwt_delay.h"
#include "uart.hpp"
#include "usart.h"
#include "chat.h"


struct Server : public Application {
    lv_obj_t *label;
    uint32_t tick_cnt = 0;
    uint32_t packet_send_cnt = 0;
    uint32_t packet_recv_cnt = 0;
    Server();

    void exit() override {
    }

    void tick() override;
};