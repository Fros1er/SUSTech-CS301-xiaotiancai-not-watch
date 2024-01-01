#include "server.h"
#include "nrf_protocol.hpp"

uint8_t old_name = 0;

Server::Server() : Application("Server") {
    label = lv_label_create(_bg);
    lv_obj_center(label);
}

void Server::enter() {
    old_name = device_name;
    device_name = SERVER_ADDR;
    nrf24l01_rx_mode();
}

void Server::exit() {
    device_name = old_name;
}

void Server::tick() {
    lv_label_set_text_fmt(label, "Server(%p->%p) status\nPacket send: %d\nPacket recv: %d\n", device_name, old_name,
    packet_send_cnt, 
    packet_recv_cnt);
}
