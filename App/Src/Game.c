#include "Game.h"

enum GAME_STATE { START, RUN, STOP } state;

static struct sKey {
    uint8_t old;
    uint8_t slow;
    uint8_t value;
    uint8_t down;
    uint8_t test;
} key;

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

static struct sDino {
    uint8_t flag;
    uint8_t count;
    uint8_t height;
    uint8_t jumpKey;
    uint8_t jumpFlag;
    uint8_t finishFlag;
    uint8_t jumpFlagFlag;
} dino;

const uint8_t DinoJumpHeightList[9] = {0, 6, 10, 15, 18, 21, 23, 25, 25};

static struct sCloud {
    uint8_t length;
    int8_t position[2];
} cloud;

static struct sGround {
    uint8_t speed;
    uint16_t moveNumber;
} ground;

uint8_t OLED_slow;

static uint8_t gameKeyStart(void) {
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == SET) {
        return 1;
    }
    return 0;
}

static uint8_t gameKeyJump(void) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == SET) {
        return 1;
    }
    return 0;
}

static void gameKeyProc(void) {
    if (key.slow)
        return;
    key.slow = 1;

    key.value = gameKeyJump();
    key.down = key.value & (key.value ^ key.old);
    key.old = key.value;

    if (gameKeyJump()) {
        dino.jumpKey = 1;
    }
}

static void gameInit(void) {
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
    ground.moveNumber = 0;

    OLED_slow = 0;

    key.slow = 0;
    key.value = 0;
    key.old = 0;
    key.down = 0;
    key.test = 0;
}

static void gameFeedInit(void) {
    ledInit();
    soundInit();
}

static void gameMenu(uint8_t menu[][128]) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_SetCursor((0 + i), 0);
        for (uint8_t j = 0; j < 128; j++) {
            OLED_WriteData(menu[i][j]);
        }
    }
}

static void gameStart(void) {
    gameMenu(GameBegin);
    while (1) {
        if (gameKeyStart()) {
            state = RUN;
            OLED_Clear();
            soundStart();                   // 蜂鸣器发信号
            HAL_TIM_Base_Start_IT(&htim4);  // 实时处理开始
            break;
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

static void gameDrawGround(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                           uint16_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++)
            OLED_WriteData(Ground[i][j + Number]);
    }
}

static void gameDrawCloud(uint8_t symbol) {
    uint8_t y1 = 0;
    uint8_t y2 = 0;
    switch (symbol) {
        case 0:
            y1 = 3;
            y2 = y1;
            break;
        case 1:
            y1 = 2;
            y2 = y1;
            break;
    }
    gameDrawBlank(cloud.position[symbol] + 1, y1,
                  cloud.position[symbol] + cloud.length - 1, y2);
    cloud.position[symbol] -= (ground.speed - 1);
    if (cloud.position[symbol] < -27)
        cloud.position[symbol] = 127;

    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (cloud.position[symbol] > 100) {
            OLED_SetCursor((y1 + i), cloud.position[symbol]);
            for (j = 0; j < 127 - cloud.position[symbol]; j++)
                OLED_WriteData(Cloud[i][j]);
        }
        if (cloud.position[symbol] >= 0 && cloud.position[symbol] <= 100) {
            OLED_SetCursor((y1 + i), cloud.position[symbol]);
            for (j = 0; j < (cloud.length); j++) OLED_WriteData(Cloud[i][j]);
        }
        if (cloud.position[symbol] < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -cloud.position[symbol]; j < (cloud.length); j++)
                OLED_WriteData(Cloud[i][j]);
        }
    }
}

void gameDrawDino(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                  uint8_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++) OLED_WriteData(Dino[Number][i][j]);
    }
}

void gameDrawDinoJump(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                      uint8_t Number) {
    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        OLED_SetCursor((y1 + i), x1);
        for (j = 0; j < (x2 - x1 + 1); j++)
            OLED_WriteData(Dino_Jump[Number][i][j]);
    }
}

static void gameDrawCactus(uint8_t symbol) {
    uint8_t y1 = 0;
    uint8_t y2 = 6;
    uint8_t x1_max = 0;
    int8_t position_max;
    uint8_t position_move;
    uint8_t **cactusResList;
    uint8_t cactusResListLength;

    switch (symbol) {
        case 0:
            y1 = 5;
            x1_max = 119;
            position_move = 3;
            position_max = -8;
            cactusResList = Cactus1;
            cactusResListLength = 8;
            break;
        case 1:
            y1 = 5;
            x1_max = 111;
            position_move = 8;
            position_max = -16;
            cactusResList = Cactus2;
            cactusResListLength = 16;
            break;
        case 2:
            y1 = 6;
            x1_max = 111;
            position_move = 8;
            position_max = -16;
            cactusResList = Cactus3;
            cactusResListLength = 16;
            break;
    }

    gameDrawBlank(cactus.position[symbol] + position_move, y1,
                  cactus.position[symbol] + cactus.length[symbol] - 1, y2);
    cactus.position[symbol] -= ground.speed;
    if (cactus.position[symbol] < position_max) {
        cactus.flag[symbol] = 1;
        cactus.position[symbol] = 127;
    }

    uint16_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < (y2 - y1 + 1); i++) {
        if (cactus.position[symbol] > x1_max) {
            OLED_SetCursor((y1 + i), cactus.position[symbol]);
            for (j = 0; j < 127 - cactus.position[symbol]; j++)
                OLED_WriteData(
                    *((uint8_t *)cactusResList + i * cactusResListLength + j));
        }
        if (cactus.position[symbol] >= 0 && cactus.position[symbol] <= x1_max) {
            OLED_SetCursor((y1 + i), cactus.position[symbol]);
            for (j = 0; j < (cactus.length[symbol]); j++)
                OLED_WriteData(
                    *((uint8_t *)cactusResList + i * cactusResListLength + j));
        }
        if (cactus.position[symbol] < 0) {
            OLED_SetCursor((y1 + i), 0);
            for (j = -cactus.position[symbol]; j < (cactus.length[symbol]); j++)
                OLED_WriteData(
                    *((uint8_t *)cactusResList + i * cactusResListLength + j));
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
    ledOn();
    soundStop();
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
        if (gameKeyStart()) {
            state = RUN;
            uint16_t temp = grade.best;
            gameInit();
            grade.best = temp;
            break;
        }
    }

    // 关闭反馈
    ledOff();
    soundStart();
    OLED_Clear();
}

// 游戏进程
static void gameProc(void) {
    if (OLED_slow) {
        return;
    }
    OLED_slow = 1;

    OLED_ShowNum(1, 12, grade.num, 5);

    // Ground
    ground.moveNumber += ground.speed;
    ground.moveNumber %= 560;
    gameDrawGround(0, 6, 127, 7, ground.moveNumber);

    // Cloud
    for (uint8_t i = 0; i < 2; i++) {
        gameDrawCloud(i);
    }

    // Dino
    if (dino.jumpKey == 0) {
        gameDrawDino(0, 5, 15, 6, dino.flag);
    } else {
        if (dino.finishFlag == 1) {
            dino.finishFlag = 2;
        }
        gameDrawDinoJump(0, 2, 15, 6, dino.jumpFlag);
        if (dino.finishFlag == 2 && dino.jumpFlag == 0) {
            dino.finishFlag = 0;
            dino.jumpKey = 0;
        }
    }

    // Cactus
    for (uint8_t i = 0; i < 3; i++) {
        if (cactus.flag[i] == 0) {
            gameDrawCactus(i);
        }
    }

    // Game Over
    if (gameIsLose()) {
        state = STOP;
    }
}

void gameEvents(void) {
    state = START;
    while (1) {
        switch (state) {
            case START:
                gameInit();
                gameFeedInit();
                gameStart();
                break;
            case RUN:
                gameProc();
                gameKeyProc();
                break;
            case STOP:
                gameRestart();
                break;
        }
    }
}

void timPeriodElapsedCallback() {
    if (++OLED_slow == 40)
        OLED_slow = 0;
    if (++key.slow == 10)
        key.slow = 0;

    // 处理小恐龙的奔跑跳跃
    dino.count++;
    if (dino.count == 50) {
        dino.flag ^= 1;

        if (dino.jumpKey == 1) {
            if (dino.jumpFlagFlag == 0 && dino.finishFlag == 0) {
                dino.jumpFlag++;
                if (dino.jumpFlag == 8)
                    dino.jumpFlagFlag = 1;
            } else if (dino.jumpFlagFlag == 1) {
                dino.jumpFlag--;
                if (dino.jumpFlag == 0) {
                    dino.jumpFlagFlag = 0;
                    dino.finishFlag = 1;
                }
            }
        }

        dino.height = DinoJumpHeightList[dino.jumpFlag];
        dino.count = 0;
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

    // 加速
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
