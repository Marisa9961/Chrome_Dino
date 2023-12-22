#include "Sound.h"

void soundInit() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,
                      SET);  // 有PA3作为无源蜂鸣器的电源
                             // 防止还没初始化完成蜂鸣器就开始响
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

void soundOn() { __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1380); }

void soundOff() { __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1724); }

void soundStart() {
    soundOn();
    HAL_Delay(300);
    soundOff();
}

void soundStop() {
    for (uint8_t i = 0; i < 3; i++) {
        soundOn();
        HAL_Delay(300);
        soundOff();
        HAL_Delay(100);
    }
}
