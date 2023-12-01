#include "main.h"
#include "Key.h"

uint8_t Get_KeyNumber(void){
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12 == 0)){
        return Jump;
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 0){
        return Start;
    }
}
