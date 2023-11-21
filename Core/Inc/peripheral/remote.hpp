/**
 ****************************************************************************************************
 * @file        remote.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-25
 * @brief       红外遥控解码 驱动代码
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
 * V1.0 20200425
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __REMOTE_H
#define __REMOTE_H

#include "stm32f1xx_hal.h"
#include "main.h"

/******************************************************************************************/
/* 红外输入引脚及定时器 定义 */

#define REMOTE_IN_GPIO_PORT REMOTE_TIM5_CH2_GPIO_Port
#define REMOTE_IN_GPIO_PIN REMOTE_TIM5_CH2_Pin

#define REMOTE_IN_TIMX TIM5
#define REMOTE_IN_TIMX_CHY TIM_CHANNEL_2 /* 通道Y,  1<= Y <=2*/

/******************************************************************************************/

#define RDATA HAL_GPIO_ReadPin(REMOTE_IN_GPIO_PORT, REMOTE_IN_GPIO_PIN) /* 红外数据输入脚 */

/* 红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
 * 我们选用的遥控器识别码为0
 */
#define REMOTE_ID 0

enum RemoteKey : uint8_t {
    POWER = 69,
    UP = 70,
    ALIENTEK = 71,
    LEFT = 68,
    PLAY = 64,
    RIGHT = 67,
    VOLMINUS = 7,
    DOWN = 21,
    VOLADD = 9,
    ONE = 22,
    TWO = 25,
    THREE = 13,
    FOUR = 12,
    FIVE = 24,
    SIX = 94,
    SEVEN = 8,
    EIGHT = 28,
    NINE = 90,
    ZERO = 66,
    BACK = 74
};

void remote_recv_cb(RemoteKey sta);

extern uint8_t g_remote_cnt; /* 按键按下的次数 */

void remote_init(void); /* 红外传感器接收头引脚初始化 */
void remote_tim5_period_elapsed_cb();
void remote_tim5_ic_capture_cb();

#endif
