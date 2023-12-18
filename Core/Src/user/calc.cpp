#include "calc.h"

Calculator::Calculator()
    : Application("Calculator") {
    lv_obj_t *kb = lv_keyboard_create(_bg.get());

    ta = lv_textarea_create(_bg.get());
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_textarea_set_placeholder_text(ta, "Hello");
    lv_obj_set_size(ta, 140, 80);

    lv_keyboard_set_textarea(kb, ta);
}
