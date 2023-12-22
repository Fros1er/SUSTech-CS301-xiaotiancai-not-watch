/**
 ****************************************************************************************************
 * @file        remote.c
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

#include "remote.hpp"

#include "tim.h"

/**
 * @brief       红外遥控初始化
 *   @note      设置IO以及定时器的输入捕获
 * @param       无
 * @retval      无
 */
#define REMOTE_TIM5_CH2_Pin GPIO_PIN_1
#define REMOTE_TIM5_CH2_GPIO_Port GPIOA

uint8_t is_remote_on() {
    // return HAL_GPIO_
}

void remote_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = REMOTE_TIM5_CH2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(REMOTE_TIM5_CH2_GPIO_Port, &GPIO_InitStruct);

    /* TIM5 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);

    HAL_TIM_IC_Start_IT(&htim5, REMOTE_IN_TIMX_CHY);               /* 开始捕获TIM5的通道2 */
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE); /* 使能更新中断 */ /* 使能更新中断 */
}

/* 遥控器接收状态
 * [7]  : 收到了引导码标志
 * [6]  : 得到了一个按键的所有信息
 * [5]  : 保留
 * [4]  : 标记上升沿是否已经被捕获
 * [3:0]: 溢出计时器
 */
uint8_t g_remote_sta = 0;
uint32_t g_remote_data = 0; /* 红外接收到的数据 */
uint8_t g_remote_cnt = 0;   /* 按键按下的次数 */

void remote_tim5_period_elapsed_cb() {
    if (g_remote_sta & 0x80) /* 上次有数据被接收到了 */
    {
        g_remote_sta &= ~0X10; /* 取消上升沿已经被捕获标记 */

        if ((g_remote_sta & 0X0F) == 0X00) {
            g_remote_sta |= 1 << 6; /* 标记已经完成一次按键的键值信息采集 */
            uint8_t sta = 0;
            uint8_t t1, t2;
            t1 = g_remote_data;               /* 得到地址码 */
            t2 = (g_remote_data >> 8) & 0xff; /* 得到地址反码 */

            if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID) /* 检验遥控识别码(ID)及地址 */
            {
                t1 = (g_remote_data >> 16) & 0xff;
                t2 = (g_remote_data >> 24) & 0xff;

                if (t1 == (uint8_t)~t2) {
                    sta = t1; /* 键值正确 */
                }
            }

            if ((sta == 0) || ((g_remote_sta & 0X80) == 0)) /* 按键数据错误/遥控已经没有按下了 */
            {
                g_remote_sta &= ~(1 << 6); /* 清除接收到有效按键标识 */
                g_remote_cnt = 0;          /* 清除按键次数计数器 */
            } else {
                remote_recv_cb(static_cast<RemoteKey>(sta));
            }
        }

        if ((g_remote_sta & 0X0F) < 14) {
            g_remote_sta++;
        } else {
            g_remote_sta &= ~(1 << 7); /* 清空引导标识 */
            g_remote_sta &= 0XF0;      /* 清空计数器 */
        }
    }
}

void remote_tim5_ic_capture_cb() {
    uint16_t dval; /* 下降沿时计数器的值 */

    if (RDATA) /* 上升沿捕获 */
    {
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim5, REMOTE_IN_TIMX_CHY, TIM_INPUTCHANNELPOLARITY_FALLING); /* CC2P=1 设置为下降沿捕获 */
        __HAL_TIM_SET_COUNTER(&htim5, 0);                                                            /* 清空定时器计数器值 */
        g_remote_sta |= 0X10;                                                                        /* 标记上升沿已经被捕获 */
    } else                                                                                           /* 下降沿捕获 */
    {
        dval = HAL_TIM_ReadCapturedValue(&htim5, REMOTE_IN_TIMX_CHY);                               /* 读取CCR1也可以清CC1IF标志位 */
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim5, REMOTE_IN_TIMX_CHY, TIM_INPUTCHANNELPOLARITY_RISING); /* 设置为上升沿捕获 */

        if (g_remote_sta & 0X10) /* 完成一次高电平捕获 */
        {
            if (g_remote_sta & 0X80) /* 接收到了引导码 */
            {
                if (dval > 300 && dval < 800) /* 560为标准值,560us */
                {
                    g_remote_data >>= 1;               /* 右移一位 */
                    g_remote_data &= ~(0x80000000);    /* 接收到0 */
                } else if (dval > 1400 && dval < 1800) /* 1680为标准值,1680us */
                {
                    g_remote_data >>= 1;               /* 右移一位 */
                    g_remote_data |= 0x80000000;       /* 接收到1 */
                } else if (dval > 2000 && dval < 3000) /* 得到按键键值增加的信息 2500为标准值2.5ms */
                {
                    g_remote_cnt++;       /* 按键次数增加1次 */
                    g_remote_sta &= 0XF0; /* 清空计时器 */
                }
            } else if (dval > 4200 && dval < 4700) /* 4500为标准值4.5ms */
            {
                g_remote_sta |= 1 << 7; /* 标记成功接收到了引导码 */
                g_remote_cnt = 0;       /* 清除按键次数计数器 */
            }
        }

        g_remote_sta &= ~(1 << 4);
    }
}
