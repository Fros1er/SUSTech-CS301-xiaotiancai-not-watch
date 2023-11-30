#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atk_md0280.h"
#include "button.hpp"
#include "dwt_delay.h"
#include "eeprom.h"
#include "ff.h"
#include "led.hpp"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "remote.hpp"
#include "sd.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "uart.hpp"
#include "usart.h"

void remote_recv_cb(RemoteKey key) {
}

void init() {
    led_init();
    button_init();
    uart_init();
    DWT_Delay_Init();
    eeprom_init();
    remote_init();
    sd_init();
    if (atk_md0280_init() != 0) {
        // led_writepin(0, LED_ON);
        led_writepin(1, LED_ON);
        while (1) {
        }
    }
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    UART_TRANSMIT_STR_LITERIAL_DMA("Welcome!\n");

    FATFS *fs = (FATFS *) lv_mem_alloc(sizeof(FATFS));
    lv_memset(fs, 0, sizeof(FATFS));
    f_mount(fs, "0:", 1);
    lv_fs_file_t f;
    lv_fs_res_t res;
    res = lv_fs_open(&f, "0:test.txt", LV_FS_MODE_RD);
    if (res != LV_FS_RES_OK) led_writepin(1, LED_ON);

    uint32_t read_num;
    uint8_t buf[8];
    res = lv_fs_read(&f, buf, 2, &read_num);
    if (res != LV_FS_RES_OK) led_writepin(1, LED_ON);

    lv_fs_close(&f);

    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, (const char *)buf);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
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

static int mode;

template <>
void button_cb<key_0>() {
}

template <>
void button_cb<key_1>() {
}

template <>
void button_cb<key_wakeup>() {
}

void uart_receive_cb(uint16_t size) {
}
