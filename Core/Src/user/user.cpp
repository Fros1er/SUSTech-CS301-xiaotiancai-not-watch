#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atk_md0280.h"
#include "button.hpp"
#include "dwt_delay.h"
#include "led.hpp"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "uart.hpp"
#include "usart.h"

void init() {
    led_init();
    button_init();
    uart_init();
    DWT_Delay_Init();
    if (atk_md0280_init() != 0) {
        led_writepin(0, LED_ON);
        led_writepin(1, LED_ON);
        while (1) {
        }
    }
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    UART_TRANSMIT_STR_LITERIAL_DMA("Welcome!\n");
}

void tick() {
    uint32_t now = HAL_GetTick();
    if (now % 500 == 0) {
        led_update();
    }
    button_update();
    lv_task_handler();
}

void tim6_cb() {
}

template<>
void button_cb<key_0>() {

}

template<>
void button_cb<key_1>() {
    
}

template<>
void button_cb<key_wakeup>() {
    
}

void uart_receive_cb(uint16_t size) {
}

