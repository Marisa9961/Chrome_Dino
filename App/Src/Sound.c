#include "Sound.h"

void Sound_On() { __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1380); }

void Sound_Off() { __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1724); }

void Sound_Start() {
    Sound_On();
    HAL_Delay(300);
    Sound_Off();
}

void Sound_Stop(){
    for(uint8_t i = 0; i < 3; i++){
        Sound_On();
        HAL_Delay(300);
        Sound_Off();
        HAL_Delay(100);
    }
}
