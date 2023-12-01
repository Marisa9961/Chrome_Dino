#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>

extern uint8_t Key_Slow;
extern uint8_t Key_Value;
extern uint8_t Key_Old;
extern uint8_t Key_Down;
extern uint8_t Key_Test;

inline uint8_t Get_Jump(void);

void Key_Proc(void);

#endif
