#include "tim.h"

void Led_Stop_On(void) { __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 20); }

void Led_Stop_Off(void) { __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); }
