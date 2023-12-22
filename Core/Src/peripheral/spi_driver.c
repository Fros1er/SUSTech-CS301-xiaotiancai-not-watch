#include "spi_driver.h"
#include "spi.h"

void spi1_set_speed(uint8_t speed) {
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* 判断有效性 */
    __HAL_SPI_DISABLE(&hspi1);                      /* 关闭SPI */
    hspi1.Instance->CR1 &= 0XFFC7;                  /* 位3-5清零，用来设置波特率 */
    hspi1.Instance->CR1 |= speed << 3;              /* 设置SPI速度 */
    __HAL_SPI_ENABLE(&hspi1);                       /* 使能SPI */
}

/**
 * @brief       SPI1读写一个字节数据
 * @param       txdata  : 要发送的数据(1字节)
 * @retval      接收到的数据(1字节)
 */
uint8_t spi1_read_write_byte(uint8_t txdata) {
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 1000);
    return rxdata; /* 返回收到的数据 */
}