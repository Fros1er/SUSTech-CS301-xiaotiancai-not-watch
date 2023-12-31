#pragma once
#include "nrf24l01.h"

extern uint8_t TX_ADDRESS[TX_ADR_WIDTH];
extern uint8_t RX_ADDRESS[RX_ADR_WIDTH];
extern uint8_t device_name;
extern uint8_t nrf_buf[33];
extern const char *table;
extern int packet_send_cnt; 
extern int packet_recv_cnt; 

void nrf_protocol_init();
void nrf_protocol_tick();
uint8_t nrf_send_msg(const char *msg_ptr, uint8_t addr, uint8_t cmd, uint8_t max_try = 1);
void nrf24l01_msg_receive_cb();
void nrf24l01_msg_tick_cb();

enum nrf24l01_cmd_t {
    CHAT_MSG = 0x0,
    NRF_PING,
    CHAT_BROADCAST,
    CHAT_INVITE,
    CALC_ANSWER,
    CALC_REQUEST,
};
