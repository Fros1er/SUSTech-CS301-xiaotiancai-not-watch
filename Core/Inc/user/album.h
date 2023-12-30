#include "application.h"
#include "ff.h"
#include <vector>

class Album : public Application {
    DIR dir;
    lv_obj_t *label, *btn_left, *btn_right, *img, *filename_label;
    int cur = 0;
    char paginator_buf[16];
    std::vector<std::string> files;

    static void change_page(Album *self);
    static Album *_instance;

   public:
    Album();
    static void go_left(lv_event_t *e);
    static void go_right(lv_event_t *e);
    void enter() override;

    static Album *instance() {
        return _instance;
    }
};