#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "lv_port_disp.h"
#include "lvgl.h"

constexpr auto MENU_BAR_HEIGHT = 30;

class ApplicationFSM;

class Application {
    friend class ApplicationFSM;
    std::string _name;

   protected:
    lv_obj_t *_bg;

   public:
    Application(const std::string &name);
    virtual void enter() = 0;
    virtual void exit() = 0;
};

class Menu : public Application {
    friend class ApplicationFSM;
    void register_application(const std::string &name);
    int app_num = 0;

   public:
    Menu();

    void enter() override {
    }

    void exit() override {
    }
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
    static ApplicationFSM &instance() {
        static ApplicationFSM instance;
        return instance;
    }
};
