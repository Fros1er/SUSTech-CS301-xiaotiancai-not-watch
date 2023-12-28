#pragma once

#include "nrf24l01.h"
#include "dwt_delay.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NA_MODE = 0x0,
    RX_MODE = 0x1,
    TX_MODE = 0x2
} nrf24l01_mode_t;

uint8_t nrf_send_msg(char* msg_ptr);
uint8_t nrf_recv_msg(uint8_t* buf);

#ifdef __cplusplus
}
#endif