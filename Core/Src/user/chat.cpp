#include "chat.h"

#include <string>

#include "nrf_protocol.hpp"

Chat *Chat::_instance;
std::string user_names[]= {"Public", "Alice", "Steve", "Creeper"};

void Chat::textarea_event_handler(lv_event_t *e) {
    Chat *self = (Chat *)e->user_data;
    char *msg = (char *)lv_textarea_get_text(self->ta);
    if (self->room_id) {
        nrf_send_msg(msg, self->room[self->room_id].room_addr, CHAT_MSG);
    } else {
        for (int i = 1; i < 3; i++) {
            if (self->room[i].online){
                nrf_send_msg(msg, self->room[i].room_addr, CHAT_BROADCAST);
            }
        }
    }

    self->room[self->room_id].msg.push_back("Me: " + std::string(msg));
    self->room[self->room_id].msg_id = self->room[self->room_id].msg.size() - 1;
    lv_textarea_set_text(self->ta, "");
}

void Chat::invite_event_handler(lv_event_t *e) {
    Chat *self = (Chat *)e->user_data;
    if (self->room_id) {
        nrf_send_msg("Invite", self->room[self->room_id].room_addr, CHAT_INVITE);
    }
}

void Chat::left_event_handler(lv_event_t *e) {
    Chat *self = (Chat *)e->user_data;
    if (self->room[self->room_id].msg_id > 0) {
        self->room[self->room_id].msg_id--;
    } else {
        self->room[self->room_id].msg_id = self->room[self->room_id].msg.size() - 1;
    }
}

void Chat::right_event_handler(lv_event_t *e) {
    Chat *self = (Chat *)e->user_data;
    if (self->room[self->room_id].msg_id < self->room[self->room_id].msg.size() - 1) {
        self->room[self->room_id].msg_id++;
    } else {
        self->room[self->room_id].msg_id = 0;
    }
}

void Chat::drop_event_handler(lv_event_t *e) {
    Chat *self = (Chat *)e->user_data;
    lv_obj_t *obj = lv_event_get_target(e);
    uint8_t new_room = lv_dropdown_get_selected(obj);
    if (self->room[new_room].online) {
        self->room_id = new_room;
    } else {
        ApplicationFSM::alert_cb("User offline!");
        lv_dropdown_set_selected(obj, self->room_id);
    }
}

Chat::Chat()
    : Application("Chat") {
    _instance = this;

    for (int i = 0, index=0; i < ROOM_NUMBER + 1; i++) {
        if (i != device_name){
            room[index].msg_id = 0;
            room[index].room_addr = i;
            room_table[i] = index;
            room[index].online = 0;
            room[index].msg = std::vector<std::string>();
            room[index++].msg.push_back("Chat Begins...");  // 😁😄😝😜🤩🥺🤔😚
        }
    }
    room[0].online = 100;

    lv_obj_t *btn_left = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_left, left_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_left, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(btn_left, 35, 30);
    lv_obj_t *bl_text = lv_label_create(btn_left);
    lv_label_set_text_static(bl_text, "Prev");
    lv_obj_center(bl_text);

    lv_obj_t *btn_right = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_right, right_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_right, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_size(btn_right, 35, 30);
    bl_text = lv_label_create(btn_right);
    lv_label_set_text_static(bl_text, "Next");
    lv_obj_center(bl_text);

    label = lv_label_create(_bg);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_set_size(label, 220, 50);

    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 90);
    lv_obj_set_size(ta, 165, 40);
    lv_textarea_set_placeholder_text(ta, "Hello");

    lv_obj_t *btn_invite = lv_btn_create(_bg);
    lv_obj_add_event_cb(btn_invite, invite_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_align(btn_invite, LV_ALIGN_TOP_RIGHT, 0, 90);
    lv_obj_set_size(btn_invite, 50, 40);
    bl_text = lv_label_create(btn_invite);
    lv_label_set_text_static(bl_text, "Invite");
    lv_obj_center(bl_text);

    lv_obj_t *kb = lv_keyboard_create(_bg);
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, this);

    dd = lv_dropdown_create(_bg);
    std::string options = "Public";
    for (int i = 1; i < 3; i++) {
        options += std::string("\n") + user_names[room[i].room_addr];
    }
    lv_dropdown_set_options(dd, options.c_str());
    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(dd, drop_event_handler, LV_EVENT_VALUE_CHANGED, this);
}

void Chat::tick() {
    lv_label_set_text_static(label, room[room_id].msg[room[room_id].msg_id].c_str());
}

void nrf24l01_msg_receive_cb() {
    auto self = Chat::instance();
    if (!self) {
        return;
    }
    if (RX_ADDRESS[1] == 0xff) {  // I am server
        uart_transmit_message("[Server A]\n");
        uart_transmit_dma(nrf_buf, 32);
        uart_transmit_message("[Server B]\n");
    } else{
        switch(nrf_buf[1]){
            case CHAT_MSG:
                if (nrf_buf[0] && nrf_buf[0] <= ROOM_NUMBER){
                    uint8_t _room = self->room_table[nrf_buf[0]];
                    self->room[_room].msg.push_back(user_names[nrf_buf[0]] + std::string(": ") + std::string((char *)nrf_buf + 2));
                    self->room[_room].msg_id = self->room[_room].msg.size() - 1;
                }
                break;
            case CHAT_BROADCAST:
                self->room[0].msg.push_back(user_names[nrf_buf[0]] + std::string(": ") + std::string((char *)nrf_buf + 2));
                self->room[0].msg_id = self->room[0].msg.size() - 1;
                break;
            case NRF_PING:
                if (nrf_buf[0] && nrf_buf[0] <= ROOM_NUMBER){
                    uint8_t _room = self->room_table[nrf_buf[0]];
                    if (!self->room[_room].online){
                        ApplicationFSM::alert_cb((std::string(user_names[nrf_buf[0]]) + std::string(" online!")).c_str());
                    }
                    self->room[_room].online = 23;
                }
                break;
            case CHAT_INVITE:
                if (nrf_buf[0] && nrf_buf[0] <= ROOM_NUMBER){
                    ApplicationFSM::alert_cb((std::string(user_names[nrf_buf[0]]) + std::string(" Invited U")).c_str());
                }
                break;
        }
    }
}

void nrf24l01_msg_tick_cb(){
    auto self = Chat::instance();
    if (!self) {
        return;
    }
    for (int i = 1; i < ROOM_NUMBER; i++) {
        if (self->room[i].online) {
            self->room[i].online--;
            if (self->room[i].online == 0) {
                ApplicationFSM::alert_cb((std::string(user_names[self->room[i].room_addr]) + std::string(" offline.")).c_str());
            }
        }
    }
}