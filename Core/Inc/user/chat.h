#include "application.h"
#include "nrf24l01.h"
#include "dwt_delay.h"

typedef enum {
    NA_MODE = 0x0,
    RX_MODE = 0x1,
    TX_MODE = 0x2
} chat_room_nrf24l01_mode_t;

struct Chat : public Application {
    lv_obj_t *ta;
	uint8_t tmp_buf[33];
    uint8_t work_mode = NA_MODE;
    Chat();

    void exit() override {
        lv_textarea_set_text(ta, "");
        work_mode = NA_MODE;
    }
    void tick() override;
};