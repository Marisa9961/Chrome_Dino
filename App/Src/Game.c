#include "Game.h"

#include "Key.h"

static struct sGrade {
    uint16_t num;
    uint16_t best;
    uint8_t count;
} grade;

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

    Key_Slow = 0;
    Key_Value = 0;
    Key_Old = 0;
    Key_Down = 0;
    Key_Test = 0;
}

static void gameMenu(uint8_t menu[][128]) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetCursor((0 + i), 0);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(menu[i][j]);
        }
    }
}

static void gameDrawBlank(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
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

static void gameDrawCactus(uint8_t symbol) {
    uint8_t **Cactus;
    uint8_t x1_max = 0;
    uint8_t y1_max = 0;
    uint8_t y2_max = 6;
    int8_t position_max;
    uint8_t position_move;

    switch (symbol) {
        case 0:
            x1_max = 119;
            y1_max = 5;
            Cactus = Cactus1;
            position_move = 3;
            position_max = -8;
            break;
        case 1:
            x1_max = 111;
            y1_max = 5;
            Cactus = Cactus2;
            position_move = 8;
            position_max = -16;
            break;
        case 2:
            x1_max = 111;
            y1_max = 6;
            Cactus = Cactus3;
            position_move = 8;
            position_max = -16;
            break;
    }

    gameDrawBlank(cactus.position[symbol] + position_move, y1_max,
                  cactus.position[symbol] + cactus.length[symbol] - 1, y2_max);
    cactus.position[symbol] -= Speed;
    if (cactus.position[symbol] < position_max) {
        cactus.flag[symbol] = 1;
        cactus.position[symbol] = 127;
    }

    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2_max - y1_max + 1); i++) {
        if (cactus.position[symbol] > x1_max) {
            OLED_SetCursor((y1_max + i), cactus.position[symbol]);
            for (j = 0; j < 127 - cactus.position[symbol]; j++)
                OLED_WriteData(Cactus[i][j]);
        }
        if (cactus.position[symbol] >= 0 && cactus.position[symbol] <= x1_max) {
            OLED_SetCursor((y1_max + i), cactus.position[symbol]);
            for (j = 0; j < (cactus.length[symbol] + 1); j++)
                OLED_WriteData(Cactus[i][j]);
        }
        if (cactus.position[symbol] < 0) {
            OLED_SetCursor((y1_max + i), 0);
            for (j = -cactus.position[symbol]; j < (cactus.length[symbol] + 1);
                 j++)
                OLED_WriteData(Cactus[i][j]);
        }
    }
}

uint8_t gameIsLose(void) {
    uint8_t i = 0;
    uint8_t x_max = 0;
    uint8_t height_max = 0;
    do {
        switch (i) {
            case 0:
                x_max = 26;
                height_max = 6;
                break;
            case 1:
                x_max = 26;
                height_max = 14;
                break;
            case 2:
                x_max = 24;
                height_max = 14;
                break;
        }
        if (cactus.position[i] + cactus.length[i] - 1 <= x_max &&
            cactus.position[i] + cactus.length[i] - 1 >= 0 &&
            dino.height <= height_max) {
            return 1;
        }
        i++;
    } while (i < 3);
    return 0;
}

static uint8_t gameRestart(void) {
    // 物理反馈
    Led_Stop_On();
    Sound_Stop();
    HAL_Delay(1000);

    // 结算
    if (grade.num > grade.best) {
        grade.best = grade.num;
    }
    gameMenu(GameOver);
    OLED_ShowString(2, 4, "Best:");
    OLED_ShowNum(2, 9, grade.best, 5);

    // 重启
    while (1) {
        if (Get_Start()) {
            uint16_t temp = grade.best;
            gameInit();
            grade.best = temp;
        }
        break;
    }

    // 关闭反馈
    Led_Stop_Off();
    Sound_Start();
    OLED_Clear();
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

    //Cactus
    for (uint8_t i = 0; i < 3; i++) {
        if (cactus.flag[i] == 0) {
            gameDrawCactus(i);
        }
    }

    // Game Over
    if (gameIsLose()) {
        gameRestart();
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
    cactus.count++;
    if (cactus.count >= cactus.createTime) {
        cactus.createTime = rand() % 3;
        cactus.createTime += 1;
        cactus.createTime *= cactus.createTimeMultiplier;

        cactus.createNumber = rand() % 3;
        cactus.flag[cactus.createNumber] = 0;
        cactus.count = 0;
    }

    grade.count++;
    if (grade.count == 200) {
        grade.num++;
        if (grade.num == 50) {
            ground.speed++;
        }
        if (grade.num == 100) {
            ground.speed++;
            cactus.createTimeMultiplier = 500;
        }
        if (grade.num == 150) {
            ground.speed++;
            cactus.createTimeMultiplier = 800;
        }
        if (grade.num == 200) {
            ground.speed++;
        }
        grade.count = 0;
    }
}
