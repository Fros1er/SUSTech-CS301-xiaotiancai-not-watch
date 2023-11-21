#include "remote.hpp"
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == REMOTE_IN_TIMX) {
        remote_tim5_period_elapsed_cb();
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == REMOTE_IN_TIMX) {
        remote_tim5_ic_capture_cb();
    }
}

#ifdef __cplusplus
}
#endif
