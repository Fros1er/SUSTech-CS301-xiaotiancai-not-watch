#include "application.h"
#include "nrf24l01.h"
#include "dwt_delay.h"
#include "uart.hpp"
#include "usart.h"

typedef enum {
    SERVER_NA_MODE = 0x0,
    SERVER_RX_MODE = 0x1,
    SERVER_TX_MODE = 0x2
} server_nrf24l01_mode_t;

struct Server : public Application {
    lv_obj_t *label;
	uint8_t tmp_buf[33];
    uint8_t work_mode = SERVER_NA_MODE;
    uint32_t tick_cnt = 0;
    uint32_t packet_send_cnt = 0;
    uint32_t packet_recv_cnt = 0;
    Server();

    void exit() override {
        work_mode = SERVER_NA_MODE;
    }

    void tick() override;
};