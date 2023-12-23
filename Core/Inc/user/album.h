#include "application.h"
#include "ff.h"

class Album : public Application {
    DIR dir;
    lv_obj_t *label, *btn_left, *btn_right;
    int cur = 0, total = 0;
    char paginator_buf[16];

    static void change_page(Album *self);

   public:
    Album();
    static void go_left(lv_event_t *e);
    static void go_right(lv_event_t *e);
    void enter() override {
    }

    void exit() override {
    }
};