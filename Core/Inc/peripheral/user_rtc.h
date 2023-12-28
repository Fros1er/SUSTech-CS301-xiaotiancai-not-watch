/**
 ****************************************************************************************************
 * @file        rtc.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2020-04-22
 * @brief       RTC 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200422
 * 第一次发布
 * V1.1 20221222
 * 修正rtc_init小问题(添加备份区域软复位代码),避免RTC初始化报错
 *
 ****************************************************************************************************
 */

#include <stdint.h>

#ifndef __RTC_H
#define __RTC_H

/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour; /* 时 */
    uint8_t min;  /* 分 */
    uint8_t sec;  /* 秒 */
    /* 公历年月日周 */
    uint16_t year; /* 年 */
    uint8_t month; /* 月 */
    uint8_t date;  /* 日 */
    uint8_t week;  /* 周 */
} rtc_calendar_t;
extern rtc_calendar_t rtc_calendar; /* 时间结构体 */

#ifdef __cplusplus
extern "C" {
#endif

/* 接口函数 */
void rtc_get_time(void);                                                                                   /* 获取RTC时间信息 */
uint16_t rtc_read_bkr(uint32_t bkrx);                                                                      /* 读取后备寄存器 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data);                                                          /* 写后备寄存器 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);                                           /* 根据年月日获取星期几 */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);  /* 设置时间 */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec); /* 设置闹钟时间 */
void rtc_alarm_cb();
void rtc_sec_cb();

#ifdef __cplusplus
}
#endif

#endif
