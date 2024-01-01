#include "calc.h"

Calculator *Calculator::_instance;

void calc_answer_cb(const char *msg) {
    if (Calculator::instance()) {
        lv_label_set_text(Calculator::instance()->label, (std::string(lv_label_get_text(Calculator::instance()->label)) + std::string(msg)).c_str());
    }
}

void Calculator::clear_event_handler(lv_event_t *e) {
    Calculator *self = (Calculator *)e->user_data;
    lv_textarea_set_text(self->ta, "");
}

void Calculator::calc_event_handler(lv_event_t *e) {
    Calculator *self = (Calculator *)e->user_data;
    char *msg = (char *)lv_textarea_get_text(self->ta);
    nrf_send_msg(msg, SERVER_ADDR, CALC_REQUEST, 2550);
    lv_label_set_text(self->label, "");
}

Calculator::Calculator()
    : Application("Calculator") {
    _instance = this;

    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(label, 200, 50);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_label_set_text(label, "Welcome to Calculator!");

    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_set_size(ta, 165, 90);
    lv_textarea_set_placeholder_text(ta, "114*514+1919");
    lv_obj_add_event_cb(ta, calc_event_handler, LV_EVENT_READY, this);
    
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
