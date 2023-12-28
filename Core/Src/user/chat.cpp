#include "chat.h"
#include <string>
const char* table_chat = "0123456789abcedf";

void Chat::textarea_event_handler(lv_event_t * e){
    Chat *self = (Chat *)e->user_data;
    char* msg = (char*)lv_textarea_get_text(self->ta);
    if (self->room_id){
            self->send_msg(msg, self->room_id, 0);
    } else {
        for (int i=1;i!=device_name[3]&&i<4;i++){
            for (int retry=10;retry;retry--){
                if (self->send_msg(msg, i, 0)==0){
                    break;
                }
            }
        }
    }

    self->room_msg_map[self->room_id].push_back("Me:"+std::string(msg));
    self->room_msg_idx_map[self->room_id] = self->room_msg_map[self->room_id].size()-1;
    lv_textarea_set_text(self->ta, "");
}

void Chat::left_event_handler(lv_event_t * e){
    Chat *self = (Chat *)e->user_data;
    if (self->room_msg_idx_map[self->room_id] > 0){
        self->room_msg_idx_map[self->room_id]--;
    } else {
        self->room_msg_idx_map[self->room_id] = self->room_msg_map[self->room_id].size()-1;
    }
}

void Chat::right_event_handler(lv_event_t * e){
    Chat *self = (Chat *)e->user_data;
    if (self->room_msg_idx_map[self->room_id] < self->room_msg_map[self->room_id].size()-1){
        self->room_msg_idx_map[self->room_id]++;
    } else {
        self->room_msg_idx_map[self->room_id] = 0;
    }
}

void Chat::drop_event_handler(lv_event_t * e) {
    Chat *self = (Chat *)e->user_data;
    lv_obj_t * obj = lv_event_get_target(e);
    uint8_t new_room = 0;
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    if (buf[0] >= '0' && buf[0] <= '9'){
        new_room = buf[0] - '0';
    } else {
        new_room = buf[0] - 'a' + 10;
    }
    new_room = new_room << 4;
    if (buf[1] >= '0' && buf[1] <= '9'){
        new_room |= buf[1] - '0';
    } else {
        new_room |= buf[1] - 'a' + 10;
    }
    if (self->room_online_map.count(new_room)){
        self->room_id = new_room;
    } else{
        lv_dropdown_set_selected(obj, self->room_id);
    }
}

Chat::Chat() : Application("Chat") {
    lv_obj_t *btn_left = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_left, left_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_left, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(btn_left, 35, 30);
    lv_obj_t *bl_text = lv_label_create(btn_left);
    lv_label_set_text(bl_text, "Prev");
    lv_obj_center(bl_text);

    lv_obj_t *btn_right = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_right, right_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_right, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_size(btn_right, 35, 30);
    bl_text = lv_label_create(btn_right);
    lv_label_set_text(bl_text, "Next");
    lv_obj_center(bl_text);

    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_set_size(label, 220, 50);
    
    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 90);
    lv_obj_set_size(ta, 220, 40);
    lv_textarea_set_placeholder_text(ta, "Hello");

    lv_obj_t *kb = lv_keyboard_create(_bg);
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, this);

    dd = lv_dropdown_create(_bg);
    lv_dropdown_set_options(dd, "00 Public room\n01\n02\n03\n");
    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(dd, drop_event_handler, LV_EVENT_VALUE_CHANGED, this);
}


void Chat::tick() {    
    lv_label_set_text(label, room_msg_map[room_id][room_msg_idx_map[room_id]].c_str());
}
