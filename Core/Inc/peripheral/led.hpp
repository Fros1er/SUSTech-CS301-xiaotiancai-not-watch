#pragma once

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void led_init();

void led_update();

void led_blink_sequence(const char *seq);

#ifdef __cplusplus
}

extern const char *active_blink_seq;
extern bool isblinking;

#define led_blink(led)       \
    if (active_blink_seq == NULL) { \
        led_writepin(led, LED_ON);  \
        isblinking = true; \
    }
#endif

#define LED_BLINK_LEN 100
#define LED_ON GPIO_PIN_RESET
#define LED_OFF GPIO_PIN_SET

#define led_writepin(led, state) HAL_GPIO_WritePin(LED_##led##_GPIO_Port, LED_##led##_Pin, state);



