#ifndef __GAME_H
#define __GAME_H

#include <stdint.h>

void ShowGameBegin(void);
void ShowGameOver(void);
void ClearPicture(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void ShowGround(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                uint16_t Number);
void ShowCloud(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void ShowDino(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t Number);
void ShowDino_Jump(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                   uint8_t Number);
void ShowCactus1(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void ShowCactus2(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void ShowCactus3(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

#endif
