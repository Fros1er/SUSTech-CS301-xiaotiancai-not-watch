#include "nrf_protocol.hpp"

#include <cmath>
#include <cstring>

#include "dwt_delay.h"
#include "nrf24l01.h"

uint8_t device_name;
uint32_t clk;

void nrf_protocol_init() {
    uint32_t chip_id[3];
    chip_id[0] = *(uint32_t *)0x1FFFF7E8;
    chip_id[1] = *(uint32_t *)0x1FFFF7EC;
    chip_id[2] = *(uint32_t *)0x1FFFF7F0;
    uint32_t device_hash = chip_id[0] ^ chip_id[1] ^ chip_id[2];
    device_hash ^= device_hash >> 8;
    device_hash ^= device_hash >> 16;
    device_hash ^= device_hash >> 24;
    device_hash = (device_hash + 1) & 0xfe;
    switch (device_hash) {
        case 70:
            device_name++;
        case 154:
            device_name++;
        default:
            device_name++;
            break;
    }
}

enum nrf24l01_mode_t {
    NA_MODE = 0x0,
    RX_MODE = 0x1,
    TX_MODE = 0x2
};

nrf24l01_mode_t work_mode = NA_MODE;
uint8_t nrf_buf[33];
int packet_send_cnt = 0;
int packet_recv_cnt = 0;

uint8_t nrf_send_msg(const char *msg_ptr, uint8_t addr, uint8_t cmd) {
    int msg_len = strlen(msg_ptr);
    RX_ADDRESS[1] = addr;
    nrf24l01_rx_mode();
    TX_ADDRESS[1] = addr;
    nrf24l01_tx_mode();
    work_mode = TX_MODE;

    for (int i = 0; i < msg_len; i += 30) {
        nrf_buf[0] = device_name;
        nrf_buf[1] = cmd;
        strncpy((char *)nrf_buf + 2, msg_ptr, std::min(msg_len - i, 30));
        nrf_buf[2 + std::min(msg_len - i, 30)] = 0;
        msg_ptr += 30;
        packet_send_cnt++;

        if (nrf24l01_tx_packet(nrf_buf) == 0) {
        } else {
            return 1;
        }
    }
    return 0;
}

void nrf_protocol_tick() {
    clk++;
    if (work_mode != RX_MODE || RX_ADDRESS[1] != device_name) {
        RX_ADDRESS[1] = device_name;
        nrf24l01_rx_mode();
        work_mode = RX_MODE;
    }

    if (nrf24l01_rx_packet(nrf_buf)==0){
        packet_recv_cnt++;
        nrf24l01_msg_receive_cb();
    }

    if (clk % 10 == 0) {
        for (int i = 1; i < 4; i++) {
            if (i != device_name){
                nrf_send_msg("Pong", i, NRF_PING);
            }
        }
    }
    nrf24l01_msg_tick_cb();
}
