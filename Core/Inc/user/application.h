#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "lv_port_disp.h"
#include "lvgl.h"

class ApplicationFSM;

class Application {
    friend class ApplicationFSM;

   protected:
    std::unique_ptr<lv_obj_t> _bg;

   public:
    Application();
    virtual void enter() = 0;
    virtual void exit() = 0;
};

class ApplicationFSM {
    std::unordered_map<std::string, Application *> applications;
    Application *cur_app = nullptr;
    ApplicationFSM() = default;

   public:
    void register_application(const std::string &name, Application *app);
    void switch_to(const std::string &name);
    static ApplicationFSM &instance() {
        static ApplicationFSM instance;
        return instance;
    }
};
