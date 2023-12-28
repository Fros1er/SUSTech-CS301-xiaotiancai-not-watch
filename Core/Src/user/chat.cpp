#include "chat.h"

std::vector<std::string> msg_list={};
uint32_t msg_idx = 0;

static void textarea_event_handler(lv_event_t * e){
    lv_obj_t * ta = lv_event_get_target(e);
    char* msg = (char*)lv_textarea_get_text(ta);
    msg_list.push_back(std::string("Me:")+std::string(msg));
    msg_idx = msg_list.size()-1;
    nrf_send_msg(msg);
    lv_textarea_set_text(ta, "");
}

static void left_event_handler(lv_event_t * e){
    if(msg_idx>0) msg_idx--;
    else msg_idx = msg_list.size()-1;
}

static void right_event_handler(lv_event_t * e){
    if(msg_idx<msg_list.size()-1) msg_idx++;
    else msg_idx = 0;
}


Chat::Chat() : Application("Chat") {
    lv_obj_t *kb = lv_keyboard_create(_bg);
    lv_obj_t *btn_left = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_left, left_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn_left, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(btn_left, 50, 25);
    lv_obj_t *bl_text = lv_label_create(btn_left);
    lv_label_set_text(bl_text, "Prev msg");
    lv_obj_center(bl_text);

    lv_obj_t *btn_right = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_right, right_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn_right, LV_ALIGN_TOP_RIGHT, -50, 0);
    lv_obj_set_size(btn_right, 50, 25);
    bl_text = lv_label_create(btn_right);
    lv_label_set_text(bl_text, "Next msg");
    lv_obj_center(bl_text);

    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 30);
    lv_obj_set_size(label, 220, 60);
    
    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 90);
    lv_obj_set_size(ta, 220, 40);
    lv_textarea_set_placeholder_text(ta, "Hello");
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);

    
    msg_list.push_back("A:Embedded System!");
    msg_list.push_back("B:Embedded System!");
    msg_list.push_back("A:SUSTech is worse than THU!");
    msg_list.push_back("B:Yes, I agree!");
    msg_list.push_back("A:When SUSTech is still SUSTC, it is better than THU!");
}


void Chat::tick() {    
    lv_label_set_text(label, msg_list[msg_idx].c_str());
    // recv event
}
