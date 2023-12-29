#include "nrf_protocol.hpp"

#include <cmath>
#include <cstring>

#include "dwt_delay.h"
#include "nrf24l01.h"

char device_name[4];
const char *table = "0123456789abcedf";

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

    device_hash = 0x01;
    device_name[3] = device_hash;
    device_name[0] = table[device_hash >> 4];
    device_name[1] = table[device_hash & 0xf];
    RX_ADDRESS[1] = device_hash;
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

uint8_t nrf_send_msg(const char *msg_ptr, uint8_t addr, uint8_t server) {
    int msg_len = strlen(msg_ptr);
    TX_ADDRESS[1] = addr;

    for (int i = 0; i < msg_len; i += 30) {
        nrf_buf[0] = device_name[3];
        nrf_buf[1] = server;
        strncpy((char *)nrf_buf + 2, msg_ptr, std::min(msg_len - i, 30));
        nrf_buf[32] = 0;
        msg_ptr += 30;

        if (work_mode != TX_MODE) {
            nrf24l01_tx_mode();
            work_mode = TX_MODE;
        }

        for (int retry_cnt = 0; retry_cnt < 13; retry_cnt++) {
            if (nrf24l01_tx_packet(nrf_buf) == 0) {
                packet_send_cnt++;
                return 0;
            } else {
                DWT_Delay_us(893);
            }
        }
    }
    return 1;
}

void nrf_protocol_tick() {
    if (work_mode != RX_MODE) {
        nrf24l01_rx_mode();
        work_mode = RX_MODE;
    }
    for (int i = 0; i < 73; i++) {
        if (nrf24l01_rx_packet(nrf_buf) == 0) {
            packet_recv_cnt++;
            nrf24l01_msg_receive_cb();
            break;
        } else {
            DWT_Delay_us(50);
        }
    }
    for (int i = 1; i != device_name[3] && i < 4; i++) {
        for (int retry = 1; retry; retry--) {
            if (nrf_send_msg("Hello, I'm online.", i, 1) == 0) {
                break;
            }
        }
    }
}