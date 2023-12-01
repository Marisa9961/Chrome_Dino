#ifndef __GAME_H
#define __GAME_H

#include <stdint.h>

void OLED_ShowGameBegin(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OLED_ShowGameOver(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void ClearPicture(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OLED_ShowGround(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint16_t Number);
void OLED_ShowCloud(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OELD_ShowDino(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                   uint8_t Number);
void OELD_ShowDino_Jump(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                        uint8_t Number);
void OLED_ShowCactus1(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OLED_ShowCactus2(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OLED_ShowCactus3(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

#endif
