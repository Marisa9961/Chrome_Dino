#include "prf.h"

extern HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    extern timPeriodElapsedCallback();
    timPeriodElapsedCallback();
}
