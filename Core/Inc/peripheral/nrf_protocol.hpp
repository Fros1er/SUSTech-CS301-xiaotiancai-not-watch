#include "nrf24l01.h"

extern uint8_t TX_ADDRESS[TX_ADR_WIDTH];  //  = {0x34, 0xde, 0x10, 0x10, 0x01}; /* 发送地址 */
extern uint8_t RX_ADDRESS[RX_ADR_WIDTH];
extern char device_name[4];
extern uint8_t nrf_buf[33];
extern const char *table;
extern int packet_send_cnt; 
extern int packet_recv_cnt; 

void nrf_protocol_init();
void nrf_protocol_tick();
uint8_t nrf_send_msg(const char *msg_ptr, uint8_t addr, uint8_t server);
void nrf24l01_msg_receive_cb();