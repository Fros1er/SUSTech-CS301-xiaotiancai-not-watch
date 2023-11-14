#include "uart.hpp"

#include "cicular_buf.hpp"
#include "stm32f1xx_hal.h"
#include "usart.h"

uint8_t uart_buf[UART_BUF_SIZE];

struct DMASendInfo {
    const uint8_t *data;
    uint16_t size;
};

static CicularBuf<DMASendInfo, 64> dma_buf;
static bool ready = true;

void uart_init() {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_buf, UART_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

void uart_transmit_dma(const uint8_t *data, uint16_t size) {
    dma_buf.push({data, size});
    if (ready) {
        ready = false;
        HAL_UART_Transmit_DMA(&huart1, data, size);
    }
}

extern "C" {
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    dma_buf.pop();
    if (!dma_buf.is_empty()) {
        ready = 0;
        HAL_UART_Transmit_DMA(&huart1, dma_buf.now().data, dma_buf.now().size);
    } else {
        ready = 1;
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
    uart_receive_cb(size);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_buf, UART_BUF_SIZE);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}
}
