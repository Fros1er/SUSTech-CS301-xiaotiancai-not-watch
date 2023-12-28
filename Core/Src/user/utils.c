#include "utils.h"

uint8_t tmp_buf[33];
uint8_t work_mode = NA_MODE;

uint8_t nrf_send_msg(char* msg_ptr) {
    int msg_len = strlen(msg_ptr);

    for (int i = 0; i < msg_len; i+=30) {
        tmp_buf[0]='a';
        tmp_buf[1]='b';
        strncpy((char*)tmp_buf+2, msg_ptr, 30);
        tmp_buf[32] = 0;
        msg_ptr += 30;
        
        if (work_mode != TX_MODE) {
            nrf24l01_tx_mode();
            work_mode = TX_MODE;
        }
        
        for (int retry_cnt = 0; retry_cnt < 10; retry_cnt++){
            if(nrf24l01_tx_packet(tmp_buf)==0) {
                return 0;
            }else {
                DWT_Delay_us(897);
            }
        }
    }
    return 1;
}

uint8_t nrf_recv_msg(uint8_t* _buf){
    if (work_mode != RX_MODE) {
        nrf24l01_rx_mode();
        work_mode = RX_MODE;
    }
    return nrf24l01_rx_packet(_buf);
}