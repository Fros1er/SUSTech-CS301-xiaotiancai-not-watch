#include "balc.h"

Balculator *Balculator::_instance;

void balc_answer_cb(const char *msg) {
    if (Balculator::instance()) {
        lv_label_set_text_fmt(Balculator::instance()->label, "%s%s", lv_label_get_text(Balculator::instance()->label), msg);
        // lv_label_set_text_fmt(Balculator::instance()->label, msg);
    }
}

void Balculator::clear_event_handler(lv_event_t *e) {
    Balculator *self = (Balculator *)e->user_data;
    lv_textarea_set_text(self->ta, "");
}

void Balculator::balc_event_handler(lv_event_t *e) {
    Balculator *self = (Balculator *)e->user_data;
    char *msg = (char *)lv_textarea_get_text(self->ta);
    nrf_send_msg(msg, SERVER_ADDR, CALC_BINARY_REQUEST, 255);
    lv_label_set_text(self->label, "Wait..\n");
}

Balculator::Balculator()
    : Application("Balculator") {
    _instance = this;

    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(label, 200, 50);
    lv_label_set_text(label, "Welcome to Balculator!");

    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_set_size(ta, 165, 90);
    lv_textarea_set_placeholder_text(ta, "114*514+1919");
    lv_obj_add_event_cb(ta, balc_event_handler, LV_EVENT_READY, this);
    
    lv_obj_t *kb = lv_keyboard_create(_bg);
    lv_keyboard_set_textarea(kb, ta);

    lv_obj_t *btn_clear = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_clear, clear_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_clear, LV_ALIGN_TOP_RIGHT, 0, 90);
    lv_obj_set_size(btn_clear, 45, 40);
    lv_obj_t *bl_text = lv_label_create(btn_clear);
    lv_label_set_text_static(bl_text, "Clear");
    lv_obj_center(bl_text);

}
