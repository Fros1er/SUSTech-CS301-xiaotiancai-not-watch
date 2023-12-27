#include "chat.h"

Chat::Chat()
    : Application("Chat") {
    lv_obj_t *kb = lv_keyboard_create(_bg);

    ta = lv_textarea_create(_bg);
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_textarea_set_placeholder_text(ta, "Hello");
    lv_obj_set_size(ta, 140, 80);
    lv_keyboard_set_textarea(kb, ta);
}


void Chat::tick() {
    tmp_buf[0] = 'Q';
    tmp_buf[1] = 'u';
    tmp_buf[2] = 'i';
    tmp_buf[3] = 'c';
    tmp_buf[4] = 'k';
    tmp_buf[5] = 'f';
    tmp_buf[6] = 'o';
    tmp_buf[7] = 'x';
    tmp_buf[8] = '\0';

    nrf24l01_tx_mode();
    work_mode = TX_MODE;
    DWT_Delay_us(10);
    
    for(int i=0;i<5;i++){
        if(nrf24l01_tx_packet(tmp_buf)==0) {
            lv_textarea_set_placeholder_text(ta, "Yes sent");
            break;
        }else {
            lv_textarea_set_placeholder_text(ta, "No msg");
            DWT_Delay_us(500);
        }
    }


    nrf24l01_rx_mode();
    work_mode = RX_MODE;
    
    // for(int i=0;i<5000;i++){
    //     if(nrf24l01_rx_packet(tmp_buf)==0) {
    //         lv_textarea_set_placeholder_text(ta, "Success");
    //     }else {
    //         lv_textarea_set_placeholder_text(ta, "No res");
    //     }
    // }
}
