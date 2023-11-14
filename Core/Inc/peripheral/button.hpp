#pragma once
#include <stdint.h>

#include "stm32f1xx_hal.h"

#define BUTTON_ANTISHAKE_DELAY 20

extern uint32_t button_last_updated[3];

typedef enum {
    key_0 = 0,
    key_1 = 1,
    key_wakeup = 2,
    BUTTON_LEN
} Button;

#define BUTTON_DECLARE(btn)                       \
    inline void button_##btn##_triggered() {      \
        button_last_updated[btn] = HAL_GetTick(); \
    }

BUTTON_DECLARE(key_0);
BUTTON_DECLARE(key_1);
BUTTON_DECLARE(key_wakeup);

void button_init();

void button_update();

#ifdef __cplusplus
template <Button btn>
void button_cb();
#endif
