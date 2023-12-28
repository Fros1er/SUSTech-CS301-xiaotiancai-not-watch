#include "server.h"

void uart_transmit_message(const char* msg){
    uart_transmit_dma((uint8_t*)msg, strlen(msg));
}

Server::Server() : Application("Server") {
    label = lv_label_create(_bg);
    lv_obj_center(label);
    uart_transmit_message("[Server] start work.\n");
}

extern uint8_t uart_buf[UART_BUF_SIZE];
uint8_t _uart_tx_tmp_buf[33];

void Server::tick() {
    lv_label_set_text_fmt(label, "Tick No. %ld\nPacket send: %ld\nPacket recv: %ld\n", tick_cnt++, packet_send_cnt, packet_recv_cnt);
    uint8_t working_flag = 1;
    while(working_flag){
        working_flag = 0;
        for(int i=0;i<273;i++){
            if(nrf_recv_msg(_uart_tx_tmp_buf)==0) {
                uart_transmit_message("[Server] msg comming.\n");
                uart_transmit_dma(_uart_tx_tmp_buf, 32);
                uart_transmit_message("[Server] msg end.\n");
                packet_recv_cnt++;
                working_flag = 1;
                break;
            }else {
                DWT_Delay_us(100);
            }
        }
    }
    
    working_flag = 1;
    while (working_flag){
        working_flag=0;
        // uart_buf[:8] == "[PACKET]"
        if (!strncmp((char*)uart_buf, "[PACKET]", 8)) {
            memset(uart_buf, 0, 8);
            packet_send_cnt+=uart_buf[8];
            working_flag=1;
        }
        // if (work_mode != SERVER_TX_MODE) {
        //     nrf24l01_tx_mode();
        //     work_mode = SERVER_TX_MODE;
        // }
        // if(nrf24l01_tx_packet(tmp_buf)==0) {
        //     lv_textarea_set_placeholder_text(ta, "Yes sent");
        // }else {
        //     lv_textarea_set_placeholder_text(ta, "No msg");
        // }
    }

}
