#include "album.h"

#define HANDLE_ERR(x) \
    if ((err = (x)) != FR_OK) goto ERR_HANDLE

Album::Album()
    : Application("Album") {
    FRESULT err;

    HANDLE_ERR(f_opendir(&dir, "0:/PICTURE"));
    FILINFO info;
    while (true) {
        HANDLE_ERR(f_readdir(&dir, &info));
        if (info.fname[0] == '\0') {
            break;
        }
        total++;
    }
    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -10);
    change_page(this);
    btn_left = lv_btn_create(_bg);
    btn_right = lv_btn_create(_bg);
    lv_obj_align(btn_left, LV_ALIGN_BOTTOM_MID, -50, 0);
    lv_obj_align(btn_right, LV_ALIGN_BOTTOM_MID, 50, 0);
    lv_obj_add_event_cb(btn_left, go_left, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(btn_right, go_right, LV_EVENT_CLICKED, this);
    {
        lv_obj_t *label = lv_label_create(btn_left);
        lv_label_set_text(label, "<");
        lv_obj_center(label);
        label = lv_label_create(btn_right);
        lv_label_set_text(label, ">");
        lv_obj_center(label);
    }

    return;
ERR_HANDLE:
    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    std::string err_text = "errcode: " + std::to_string(err);
    lv_label_set_text(label, err_text.c_str());
    return;
}

void Album::go_left(lv_event_t *e) {
    Album *self = (Album *)e->user_data;
    self->cur--;
    if (self->cur < 0) self->cur = self->total - 1;
    Album::change_page(self);
}

void Album::go_right(lv_event_t *e) {
    Album *self = (Album *)e->user_data;
    self->cur++;
    if (self->cur >= self->total) self->cur = 0;
    Album::change_page(self);
}

void Album::change_page(Album *self) {
    std::snprintf(self->paginator_buf, 16, "%d / %d", self->cur + 1, self->total);
    lv_label_set_text_static(self->label, self->paginator_buf);
}