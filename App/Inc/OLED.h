#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteData(uint8_t Data);
void OLED_WriteCommand(uint8_t Command);
void OLED_SetCursor(uint8_t Y, uint8_t X);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif