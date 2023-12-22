#include <stdint.h>

#define SPI_SPEED_2 0
#define SPI_SPEED_4 1
#define SPI_SPEED_8 2
#define SPI_SPEED_16 3
#define SPI_SPEED_32 4
#define SPI_SPEED_64 5
#define SPI_SPEED_128 6
#define SPI_SPEED_256 7

#ifdef __cplusplus
extern "C" {
#endif

void spi1_set_speed(uint8_t speed);
uint8_t spi1_read_write_byte(uint8_t txdata);

#ifdef __cplusplus
}
#endif