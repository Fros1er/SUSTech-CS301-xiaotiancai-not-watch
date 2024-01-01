#include "user.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "album.h"
#include "application.h"
#include "atk_md0280.h"
#include "button.hpp"
#include "calc.h"
#include "chat.h"
#include "dwt_delay.h"
#include "eeprom.h"
#include "ff.h"
#include "led.hpp"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "mem_monitor.h"
#include "nrf24l01.h"
#include "nrf_protocol.hpp"
#include "remote.hpp"
#include "sd.h"
#include "server.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "uart.h"
#include "usart.h"

void uart_transmit_debug_message(const char *msg) {
#ifdef DEBUG
    uart_transmit_dma((uint8_t *)msg, strlen(msg));
#endif
}

char buf[64];
static void _check_init(int status_code, const char *func_name) {
    if (status_code != 0) {
        led_writepin(1, LED_ON);
        sprintf(buf, "[Error] %20s Failed.\n", func_name);
        while (1)
            ;
    } else {
        sprintf(buf, "[OK] %20s Done.\n", func_name);
    }
    uart_transmit_debug_message(buf);
}

bool init_done = false;

void init() {
    uart_init();
    uart_transmit_debug_message("[Info] Starting Initialization\n");
    eeprom_init();
    uart_transmit_debug_message("[OK] Initialized eeprom\n");
    _check_init(DWT_Delay_Init(), "DWT Delay Initialize");
    _check_init(atk_md0280_init(), "ATK MD0280 Initialize");
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    uart_transmit_debug_message("[OK] Initialized LVGL\n");
    remote_init();
    uart_transmit_debug_message("[OK] Initialized remote\n");
    led_init();
    uart_transmit_debug_message("[OK] Initialized led\n");
    button_init();
    uart_transmit_debug_message("[OK] Initialized button\n");
    int sd_mode = sd_init();
    uart_transmit_debug_message("[OK] Initialized SD Card\n");
    nrf24l01_init();
    nrf_protocol_init();
    uart_transmit_debug_message("[OK] Initialized nrf24l01\n");
    _check_init(nrf24l01_check(), "NRF24L01 Check");
    uart_transmit_debug_message("[Info] Entering main FSM\n");

    FATFS *fs = (FATFS *)lv_mem_alloc(sizeof(FATFS));
    lv_memset(fs, 0, sizeof(FATFS));
    f_mount(fs, "0:", 1);
    // lv_fs_file_t f;
    // lv_fs_res_t res;
    // res = lv_fs_open(&f, "0:test.txt", LV_FS_MODE_RD);
    // if (res != LV_FS_RES_OK) led_writepin(1, LED_ON);

    // uint32_t read_num;
    // uint8_t buf[8];
    // res = lv_fs_read(&f, buf, 2, &read_num);
    // if (res != LV_FS_RES_OK) led_writepin(1, LED_ON);

    // lv_fs_close(&f);

    // lv_obj_t *label1 = lv_label_create(lv_scr_act());
    // lv_label_set_text(label1, (const char *)buf);
    // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
    auto &fsm = ApplicationFSM::instance();
    fsm.init();  // init other apps after this
    fsm.register_application(new Calculator());
    if (device_name != SERVER_ADDR) {
        fsm.register_application(new Chat());
    }
    if (!sd_mode) {
        fsm.register_application(new Album());
    } else {
        fsm.register_application(new Server());
    }
    fsm.register_application(new MemoryMonitor());
    fsm.switch_to("Menu");
    uart_transmit_debug_message("[Info] All done, welcome!\n");
    init_done = true;
}

void tick() {
    if (!init_done) {
        return;
    }
    uint32_t now = HAL_GetTick();
    if (now % 10 == 0) {
        if (now % 100 == 0) {
            nrf_protocol_tick();
            ApplicationFSM::instance().tick();
            if (now % 500 == 0) {
                led_update();
            }
        }
    }
    button_update();
    lv_task_handler();
}

void tim6_cb() {
}

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
    if (size > 2 && device_name==SERVER_ADDR) {
        nrf_send_msg((char*)uart_buf+2, uart_buf[0], uart_buf[1], 100);
    }
}

void *operator new(std::size_t sz) noexcept {
    return lv_mem_alloc(sz);
}

void operator delete(void *ptr) noexcept {
    lv_mem_free(ptr);
}
