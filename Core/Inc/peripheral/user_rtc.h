#pragma once

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern RTC_TimeTypeDef rtc_time;   //时间结构体参数
extern RTC_DateTypeDef rtc_date;   //日期结构体参数

void rtc_update_time(void);
void rtc_sec_cb();
void rtc_alarm_cb();

#ifdef __cplusplus
}
#endif
