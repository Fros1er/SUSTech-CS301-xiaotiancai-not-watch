#include "application.h"


struct Server : public Application {
    lv_obj_t *label;
    Server();

    void tick() override;
    void enter() override;
    void exit() override;
};