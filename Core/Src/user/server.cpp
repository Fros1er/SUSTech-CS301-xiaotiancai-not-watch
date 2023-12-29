#include "server.h"
#include "nrf_protocol.hpp"

extern uint8_t TX_ADDRESS[TX_ADR_WIDTH];
extern uint8_t RX_ADDRESS[RX_ADR_WIDTH];

Server::Server() : Application("Server") {
    label = lv_label_create(_bg);
    lv_obj_center(label);
}

void Server::enter() {
    RX_ADDRESS[1] = 0xff;
}

void Server::exit() {
    RX_ADDRESS[1] = device_name[3];
}

void Server::tick() {
    lv_label_set_text_fmt(label, "Server status\nChip ID A:%p\nChip ID B:%p\nChip ID C:%p\nPacket send: %ld\nPacket recv: %ld\n", 
    *(uint32_t*)0x1FFFF7E8, 
    *(uint32_t*)0x1FFFF7EC, 
    *(uint32_t*)0x1FFFF7F0, 
    packet_send_cnt, 
    packet_recv_cnt);
}
