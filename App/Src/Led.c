#include "Led.h"

void ledInit(void) { HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); }

void ledOn(void) { __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 20); }

void ledOff(void) { __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); }
