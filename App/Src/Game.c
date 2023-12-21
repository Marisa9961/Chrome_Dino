#include "Game.h"
#include "Key.h"

static struct sGrade {
    uint16_t num;
    uint16_t best;
    uint8_t count;
} grade;

// 仙人掌
uint16_t Cactus_CreatTime = 3000;
uint16_t Cactus_CreatTime_Multiplier = 1000;
uint8_t Cactus_CreatNumber = 0;
const int8_t Cactus_Length1 = 8;
int8_t Cactus_Position1 = 127;
uint8_t Cactus_Flag1 = 1;
const int8_t Cactus_Length2 = 16;
int8_t Cactus_Position2 = 127;
uint8_t Cactus_Flag2 = 1;
const int8_t Cactus_Length3 = 16;
int8_t Cactus_Position3 = 127;
uint8_t Cactus_Flag3 = 1;
uint16_t Cactus_Count = 0;

static struct sCactus {
    uint16_t createTime;
    uint8_t createNumber;
    uint16_t createTimeMultiplier;
    int8_t length[3];
    int8_t position[3];
    uint8_t flag[3];
    uint16_t count;
} cactus;

// Dino
uint8_t Height = 0;
uint8_t Dino_Flag = 0;
uint8_t Dino_Jump_Key = 0;
uint8_t Dino_Jump_Flag = 0;
uint8_t Dino_Jump_Flag_Flag = 0;
uint8_t Dino_Count = 0;
uint8_t Jump_FinishFlag = 0;

static struct sDino {
    uint8_t flag;
    uint8_t height;
    uint8_t jumpKey;
    uint8_t jumpFlag;
    uint8_t jumpFlagFlag;
    uint8_t count;
    uint8_t finishFlag;
} dino;

// Cloud
const uint8_t Cloud_Length = 27;
int8_t Cloud_Positon_1 = 100;
int8_t Cloud_Positon_2 = 0;

static struct sCloud {
    uint8_t length;
    int8_t position[2];
} cloud;

// Ground
uint8_t OLED_Slow = 0;
uint16_t Ground_Move_Number = 0;
uint8_t Speed = 3;

static struct sGround {
    uint8_t speed;
    uint8_t OLED_slow;
    uint16_t moveNumber;
} ground;

void gameInit(void) {
    grade.num = 0;
    grade.best = 0;
    grade.count = 0;

    cactus.createTime = 3000;
    cactus.createTimeMultiplier = 1000;
    cactus.createNumber = 0;
    cactus.length[0] = 8;
    cactus.length[1] = 16;
    cactus.length[2] = 16;
    cactus.position[0] = 127;
    cactus.position[1] = 127;
    cactus.position[2] = 127;
    cactus.flag[0] = 1;
    cactus.flag[1] = 1;
    cactus.flag[2] = 1;
    cactus.count = 0;

    dino.flag = 0;
    dino.count = 0;
    dino.height = 0;
    dino.jumpKey = 0;
    dino.jumpFlag = 0;
    dino.finishFlag = 0;
    dino.jumpFlagFlag = 0;

    cloud.length = 27;
    cloud.position[0] = 100;
    cloud.position[1] = 0;

    ground.speed = 3;
    ground.OLED_slow = 0;
    ground.moveNumber = 0;
}

// 游戏绘制
void Show_GameBegin(void) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetCursor((0 + i), 0);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(GameBegin[i][j]);
        }
    }
}

void Show_GameOver(void) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetCursor((0 + i), 0);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(GameOver[i][j]);
        }
    }
}

void gameMenu(uint8_t menu[][128]){
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetCursor((0 + i), 0);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(menu[i][j]);
        }
    }
}

void Show_ClearPicture(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (x1 > 119) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < 127 - x1; j++) OLED_WriteData(0x00);
        }
        if (x1 >= 0 && x1 <= 119) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(0x00);
        }
        if (x1 < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -x1; j < (x2 - x1 + 1); j++) OLED_WriteData(0x00);
        }
    }
}

void Show_Ground(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                 uint16_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++)
            OLED_WriteData(Ground[i][j + Number]);
    }
}

void Show_Cloud(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (x1 > 100) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < 127 - x1; j++) OLED_WriteData(Cloud[i][j]);
        }
        if (x1 >= 0 && x1 <= 100) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Cloud[i][j]);
        }
        if (x1 < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -x1; j < (x2 - x1 + 1); j++) OLED_WriteData(Cloud[i][j]);
        }
    }
}

void Show_Dino(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Dino[Number][i][j]);
    }
}

void Show_Dino_Jump(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                    uint8_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++)
            OLED_WriteData(Dino_Jump[Number][i][j]);
    }
}

void Show_Cactus1(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (x1 > 119) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < 127 - x1; j++) OLED_WriteData(Cactus1[i][j]);
        }
        if (x1 >= 0 && x1 <= 119) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus1[i][j]);
        }
        if (x1 < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -x1; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus1[i][j]);
        }
    }
}

void Show_Cactus2(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (x1 > 111) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < 127 - x1; j++) OLED_WriteData(Cactus2[i][j]);
        }
        if (x1 >= 0 && x1 <= 111) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus2[i][j]);
        }
        if (x1 < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -x1; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus2[i][j]);
        }
    }
}

void Show_Cactus3(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (x1 > 111) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < 127 - x1; j++) OLED_WriteData(Cactus3[i][j]);
        }
        if (x1 >= 0 && x1 <= 111) {
            OLED_SetCursor((y1 + i), x1);
            for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus3[i][j]);
        }
        if (x1 < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -x1; j < (x2 - x1 + 1); j++) OLED_WriteData(Cactus3[i][j]);
        }
    }
}

static void Game_Restart(void) {
    // Grade
    grade.count = 0;
    grade.num = 0;

    // Cactus
    Cactus_CreatTime = 3000;
    Cactus_CreatTime_Multiplier = 1000;
    Cactus_CreatNumber = 0;
    Cactus_Position1 = 127;
    Cactus_Flag1 = 1;
    Cactus_Position2 = 127;
    Cactus_Flag2 = 1;
    Cactus_Position3 = 127;
    Cactus_Flag3 = 1;
    Cactus_Count = 0;

    // Dino
    Height = 0;
    Dino_Flag = 0;
    Dino_Jump_Key = 0;
    Dino_Jump_Flag = 0;
    Dino_Jump_Flag_Flag = 0;
    Dino_Count = 0;
    Jump_FinishFlag = 0;

    // Cloud
    Cloud_Positon_1 = 100;
    Cloud_Positon_2 = 0;

    // Ground
    OLED_Slow = 0;
    Ground_Move_Number = 0;
    Speed = 3;

    Key_Slow = 0;
    Key_Value = 0;
    Key_Old = 0;
    Key_Down = 0;
    Key_Test = 0;
}

// 游戏进程
void Game_Proc(void) {
    if (OLED_Slow) {
        return;
    }
    OLED_Slow = 1;

    OLED_ShowNum(1, 12, grade.num, 5);

    // 地面
    Ground_Move_Number += Speed;
    Ground_Move_Number %= 560;
    Show_Ground(0, 6, 127, 7, Ground_Move_Number);

    // 云1
    Show_ClearPicture(Cloud_Positon_1 + 1, 3,
                      Cloud_Positon_1 + Cloud_Length - 1, 3);
    Cloud_Positon_1 -= (Speed - 1);
    if (Cloud_Positon_1 < -27)
        Cloud_Positon_1 = 127;
    Show_Cloud(Cloud_Positon_1, 3, Cloud_Positon_1 + Cloud_Length - 1, 3);

    // 云2
    Show_ClearPicture(Cloud_Positon_2 + 1, 2,
                      Cloud_Positon_2 + Cloud_Length - 1, 2);
    Cloud_Positon_2 -= (Speed - 1);
    if (Cloud_Positon_2 < -27)
        Cloud_Positon_2 = 127;
    Show_Cloud(Cloud_Positon_2, 2, Cloud_Positon_2 + Cloud_Length - 1, 2);

    // 小恐龙
    if (Dino_Jump_Key == 0) {
        Show_Dino(0, 5, 15, 6, Dino_Flag);
    } else {
        if (Jump_FinishFlag == 1)
            Jump_FinishFlag = 2;
        Show_Dino_Jump(0, 2, 15, 6, Dino_Jump_Flag);
        if (Jump_FinishFlag == 2 && Dino_Jump_Flag == 0) {
            Jump_FinishFlag = 0;
            Dino_Jump_Key = 0;
        }
    }

    // 仙人掌1
    if (Cactus_Flag1 == 0) {
        Show_ClearPicture(Cactus_Position1 + 3, 5,
                          Cactus_Position1 + Cactus_Length1 - 1, 6);
        Cactus_Position1 -= Speed;
        if (Cactus_Position1 < -8) {
            Cactus_Flag1 = 1;
            Cactus_Position1 = 127;
        }
        Show_Cactus1(Cactus_Position1, 5, Cactus_Position1 + Cactus_Length1 - 1,
                     6);
    }

    // 仙人掌2
    if (Cactus_Flag2 == 0) {
        Show_ClearPicture(Cactus_Position2 + 8, 5,
                          Cactus_Position2 + Cactus_Length2 - 1, 6);
        Cactus_Position2 -= Speed;
        if (Cactus_Position2 < -16) {
            Cactus_Flag2 = 1;
            Cactus_Position2 = 127;
        }
        Show_Cactus2(Cactus_Position2, 5, Cactus_Position2 + Cactus_Length2 - 1,
                     6);
    }

    // 仙人掌3
    if (Cactus_Flag3 == 0) {
        Show_ClearPicture(Cactus_Position3 + 8, 6,
                          Cactus_Position3 + Cactus_Length3 - 1, 6);
        Cactus_Position3 -= Speed;
        if (Cactus_Position3 < -16) {
            Cactus_Flag3 = 1;
            Cactus_Position3 = 127;
        }
        Show_Cactus3(Cactus_Position3, 6, Cactus_Position3 + Cactus_Length3 - 1,
                     6);
    }

    // Game Over
    if (Cactus_Position3 + Cactus_Length3 - 1 <= 26 &&
            Cactus_Position3 + Cactus_Length3 - 1 >= 0 && Height <= 6 ||
        Cactus_Position2 + Cactus_Length2 - 1 <= 26 &&
            Cactus_Position2 + Cactus_Length2 - 1 >= 0 && Height <= 14 ||
        Cactus_Position1 + Cactus_Length1 - 1 <= 24 &&
            Cactus_Position1 + Cactus_Length1 - 1 >= 0 && Height <= 14) {
        if (grade.num > grade.best) {
            grade.best = grade.num;
        }

        Led_Stop_On();
        Sound_Stop();
        HAL_Delay(1000);
        
        gameMenu(GameOver);

        OLED_ShowString(2, 4, "Best:");
        OLED_ShowNum(2, 9, grade.best, 5);

        while (1) {
            if (Get_Start())
                break;
        }
        OLED_Clear();
        Led_Stop_Off();
        Sound_Start();
        Game_Restart();
    }
}

void timPeriodElapsedCallback() {
    if (++OLED_Slow == 40)
        OLED_Slow = 0;
    if (++Key_Slow == 10)
        Key_Slow = 0;

    // 处理小恐龙的奔跑跳跃
    Dino_Count++;
    if (Dino_Count == 50) {
        Dino_Flag ^= 1;

        if (Dino_Jump_Key == 1) {
            if (Dino_Jump_Flag_Flag == 0 && Jump_FinishFlag == 0) {
                Dino_Jump_Flag++;
                if (Dino_Jump_Flag == 8)
                    Dino_Jump_Flag_Flag = 1;
            } else if (Dino_Jump_Flag_Flag == 1) {
                Dino_Jump_Flag--;
                if (Dino_Jump_Flag == 0) {
                    Dino_Jump_Flag_Flag = 0;
                    Jump_FinishFlag = 1;
                }
            }
        }

        switch (Dino_Jump_Flag) {
            case 0:
                Height = 0;
                break;
            case 1:
                Height = 6;
                break;
            case 2:
                Height = 10;
                break;
            case 3:
                Height = 15;
                break;
            case 4:
                Height = 18;
                break;
            case 5:
                Height = 21;
                break;
            case 6:
                Height = 23;
                break;
            case 7:
                Height = 25;
                break;
            case 8:
                Height = 25;
                break;
        }

        Dino_Count = 0;
    }

    // 生成仙人掌
    Cactus_Count++;
    if (Cactus_Count >= Cactus_CreatTime) {
        Cactus_CreatTime = rand() % 3;
        Cactus_CreatTime += 1;
        Cactus_CreatTime *= Cactus_CreatTime_Multiplier;

        Cactus_CreatNumber = rand() % 3;
        switch (Cactus_CreatNumber) {
            case 0:
                Cactus_Flag1 = 0;
                break;
            case 1:
                Cactus_Flag2 = 0;
                break;
            case 2:
                Cactus_Flag3 = 0;
                break;
        }
        Cactus_Count = 0;
    }

    grade.count++;
    if (grade.count == 200) {
        grade.num++;
        if (grade.num == 50) {
            Speed++;
        }
        if (grade.num == 100) {
            Speed++;
            Cactus_CreatTime_Multiplier = 500;
        }
        if (grade.num == 150) {
            Speed++;
            Cactus_CreatTime_Multiplier = 800;
        }
        if (grade.num == 200) {
            Speed++;
        }
        grade.count = 0;
    }
}
