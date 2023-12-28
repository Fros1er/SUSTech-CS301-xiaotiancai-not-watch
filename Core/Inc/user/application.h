#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "lv_port_disp.h"
#include "lvgl.h"
#include "nrf24l01.h"
#include "dwt_delay.h"
#include "uart.hpp"
#include "usart.h"

constexpr auto MENU_BAR_HEIGHT = 30;

class ApplicationFSM;

typedef enum {
    NA_MODE = 0x0,
    RX_MODE = 0x1,
    TX_MODE = 0x2
} nrf24l01_mode_t;

class Application {
    friend class ApplicationFSM;
    std::string _name;


   protected:
    lv_obj_t *_bg;
    static char device_name[4];
    static uint8_t work_mode;
    static uint32_t packet_send_cnt;
    static uint32_t packet_recv_cnt;
    static uint8_t tmp_buf[33];
    static std::unordered_map<uint8_t, std::vector<std::string>> room_msg_map;
    static std::unordered_map<uint8_t, uint32_t> room_msg_idx_map;
    static std::unordered_map<uint8_t, uint8_t> room_online_map;
    
   public:
    Application(const std::string &name);
    virtual void enter() {
    }
    virtual void exit() {
    }
    virtual void tick() {
    }
    void make_room(uint8_t room);
    uint8_t recv_msg();
    uint8_t send_msg(const char* msg, uint8_t room, uint8_t sev);

};

class Menu : public Application {
    friend class ApplicationFSM;
    void register_application(const std::string &name);
    int app_num = 0;
    lv_obj_t *info_label;

   public:
    Menu();
};

class ApplicationFSM {
    std::unordered_map<std::string, Application *> applications;
    Application *cur_app = nullptr;

    ApplicationFSM();

    lv_obj_t *_title_bar, *_back_btn, *_title;

   public:
    void init();
    Menu *menu = nullptr;
    void register_application(Application *app);
    void switch_to(const std::string &name);
    void tick();
    static ApplicationFSM &instance() {
        static ApplicationFSM instance;
        return instance;
    }
};
