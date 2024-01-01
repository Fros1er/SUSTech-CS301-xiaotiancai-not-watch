#include "user_rtc.h"
#include "rtc.h"

void rtc_update_time(void) {
    HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, (uint16_t)(rtc_date.Year));
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, (uint16_t)(rtc_date.Month));
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, (uint16_t)(rtc_date.Date));
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, (uint16_t)(rtc_date.WeekDay));
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc) {
    if (__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_SEC) != RESET) /* 秒中断 */
    {
        rtc_update_time(); /* 更新时间 */
        rtc_sec_cb();
    }
    if (__HAL_RTC_ALARM_GET_FLAG(hrtc, RTC_FLAG_ALRAF) != RESET) /* 闹钟标志 */
    {
        rtc_alarm_cb();
    }
}
