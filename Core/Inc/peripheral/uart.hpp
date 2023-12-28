#pragma once
#include "stm32f1xx_hal.h"

#define UART_BUF_SIZE 256

extern uint8_t uart_buf[UART_BUF_SIZE];

void uart_init();


#ifdef __cplusplus
extern "C" {
#endif

void uart_transmit_dma(const uint8_t *data, uint16_t size);

#define UART_TRANSMIT_STR_LITERIAL_DMA(cstr) uart_transmit_dma((const uint8_t *)cstr, sizeof(cstr));

void uart_receive_cb(uint16_t size);


#ifdef __cplusplus
}
#endif
