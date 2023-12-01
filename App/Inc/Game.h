#ifndef __GAME_H
#define __GAME_H

#include <stdint.h>

// 分数
extern uint8_t Grade_Count;
extern uint16_t Grade;

// 仙人掌
extern uint16_t Cactus_CreatTime;
extern uint16_t Cactus_CreatTime_Multiplier;
extern uint8_t Cactus_CreatNumber;
extern const int8_t Cactus_Length1;
extern int8_t Cactus_Position1;
extern uint8_t Cactus_Flag1;
extern const int8_t Cactus_Length2;
extern int8_t Cactus_Position2;
extern uint8_t Cactus_Flag2;
extern const int8_t Cactus_Length3;
extern int8_t Cactus_Position3;
extern uint8_t Cactus_Flag3;
extern uint16_t Cactus_Count;

// Dino
extern uint8_t Height;
extern uint8_t Dino_Flag;
extern uint8_t Dino_Jump_Key;
extern uint8_t Dino_Jump_Flag;
extern uint8_t Dino_Jump_Flag_Flag;
extern uint8_t Dino_Count;
extern uint8_t Jump_FinishFlag;

// Cloud
extern const uint8_t Cloud_Length;
extern int8_t Cloud_Positon_1;
extern int8_t Cloud_Positon_2;

// Ground
extern uint8_t OLED_Slow;
extern uint16_t Ground_Move_Number;
extern uint8_t Speed;

void Show_GameBegin(void);
void Show_GameOver(void);
void Show_ClearPicture(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Show_Ground(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                 uint16_t Number);
void Show_Cloud(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Show_Dino(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t Number);
void Show_Dino_Jump(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                    uint8_t Number);
void Show_Cactus1(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Show_Cactus2(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Show_Cactus3(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

void Game_Proc(void);

#endif
