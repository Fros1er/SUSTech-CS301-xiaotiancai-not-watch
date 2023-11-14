#include "led.hpp"

#include "main.h"

const char *active_blink_seq = NULL;

void led_init() {
    led_writepin(0, LED_OFF);
    led_writepin(1, LED_OFF);
}

static void set_led_by_char(char c) {
    if (c == '0') {
        led_writepin(0, LED_ON);
    } else if (c == ' ') {
        led_writepin(0, LED_ON);
        led_writepin(1, LED_ON);
    } else {
        led_writepin(1, LED_ON);
    }
}

static uint8_t is_blink_off = 0;

// call every 0.5s for 1s blink
void led_update() {
    if (active_blink_seq == NULL) {
        led_writepin(0, LED_OFF);
        led_writepin(1, LED_OFF);
        return;
    }

    if (is_blink_off) {
        led_writepin(0, LED_OFF);
        led_writepin(1, LED_OFF);
    } else {
        char c = *(active_blink_seq++);
        if (!c) {
            active_blink_seq = NULL;
            return;
        }
        set_led_by_char(c);
    }

    is_blink_off = !is_blink_off;
}

void led_blink_sequence(const char *seq) {
    if (!seq[0]) {
        return;
    }
    set_led_by_char(seq[0]);
    is_blink_off = 1;
    active_blink_seq = seq + 1;
}
