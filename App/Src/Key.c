#include "Key.h"

#include "Game.h"

uint8_t Key_Slow = 0;
uint8_t Key_Value = 0;
uint8_t Key_Old = 0;
uint8_t Key_Down = 0;
uint8_t Key_Test = 0;

uint8_t Get_Start(void) {
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == SET) {
        return 1;
    }
    return 0;
}

static uint8_t Get_Jump(void) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == SET) {
        return 1;
    }
    return 0;
}

void Key_Proc(void) {
    if (Key_Slow)
        return;
    Key_Slow = 1;

    Key_Value = Get_Jump();
    Key_Down = Key_Value & (Key_Value ^ Key_Old);
    Key_Old = Key_Value;

    if (Get_Jump()) {
        Dino_Jump_Key = 1;
    }
}
