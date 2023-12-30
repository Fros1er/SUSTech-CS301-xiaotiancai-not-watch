#include "album.h"

#include "remote.hpp"

#define HANDLE_ERR(x) \
    if ((err = (x)) != FR_OK) goto ERR_HANDLE

Album * Album::_instance;

Album::Album()
    : Application("Album") {
    _instance = this;
    FRESULT err;
    std::string prefix = "0:/PICTURE/";

    HANDLE_ERR(f_opendir(&dir, "0:/PICTURE"));
    FILINFO info;
    while (true) {
        HANDLE_ERR(f_readdir(&dir, &info));
        if (info.fname[0] == '\0') {
            break;
        }
        files.push_back(prefix + info.fname);
    }
    files.shrink_to_fit();
    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -10);
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

    img = lv_img_create(_bg);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -40);

    filename_label = lv_label_create(_bg);
    lv_obj_align(filename_label, LV_ALIGN_BOTTOM_MID, 0, -45);

    return;
ERR_HANDLE:
    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    std::string err_text = "errcode: " + std::to_string(err);
    lv_label_set_text(label, err_text.c_str());
    return;
}

void Album::enter() {
    change_page(this);
}

void Album::go_left(lv_event_t *e) {
    Album *self = Album::instance();
    self->cur--;
    if (self->cur < 0) self->cur = self->files.size() - 1;
    Album::change_page(self);
}

void Album::go_right(lv_event_t *e) {
    Album *self = Album::instance();
    self->cur++;
    if (self->cur >= self->files.size()) self->cur = 0;
    Album::change_page(self);
}

void Album::change_page(Album *self) {
    std::snprintf(self->paginator_buf, 16, "%d / %d", self->cur + 1, self->files.size());
    lv_label_set_text_static(self->label, self->paginator_buf);
    const char *path = self->files[self->cur].c_str();
    lv_img_set_src(self->img, path);
    lv_label_set_text_static(self->filename_label, path);
}

void remote_recv_cb(RemoteKey key) {
    Album *self = Album::instance();
    if (!self) {
        return;
    }
    switch (key) {
    case RIGHT:
        self->go_right(nullptr);
        break;
    case LEFT:
        self->go_left(nullptr);
        break;
    default:
        break;
    }
}