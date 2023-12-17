#include "application.h"

extern ApplicationFSM applicationFSM;

static void event_handler(lv_event_t *e) {
    ApplicationFSM::instance().switch_to("Calc");
}

struct Menu : Application {
    lv_obj_t *ta;
    Menu()
        : Application() {
        lv_obj_t *label;

        lv_obj_t *btn1 = lv_btn_create(_bg.get());
        lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_CLICKED, NULL);
        lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

        label = lv_label_create(btn1);
        lv_label_set_text(label, "Calc");
        lv_obj_center(label);
    }
    void enter() override {
    }

    void exit() override {
    }
};