#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void eeprom_init();

void eeprom_write_byte(uint8_t addr, uint8_t data);
void eeprom_write_bytes(uint32_t addr, uint8_t *data, uint8_t len);
uint8_t eeprom_read_byte(uint8_t addr);
void eeprom_read_bytes(uint8_t addr, uint8_t *dst, uint8_t len);

void eeprom_save_touch_calib(uint8_t *data);
uint8_t eeprom_load_touch_calib(uint8_t *data);

#ifdef __cplusplus
}
#endif
