#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

/* フィールド幅/高さ */
#define FIELD_WIDTH     (10)
#define FIELD_HEIGHT    (20)

/* テトリミノ幅/高さ */
#define SHAPE_WIDTH_MAX     (4)
#define SHAPE_HEIGHT_MAX    (4)

/* 落下速度 */
#define FPS         (1)
#define INTERVAL    (1000 / FPS)

/* テトリミノの形を定義 */
enum {
    SHAPE_I,
    SHAPE_O,
    SHAPE_S,
    SHAPE_Z,
    SHAPE_J,
    SHAPE_L,
    SHAPE_T,
    SHAPE_MAX
};

typedef struct {
    int width;
    int height;
    int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
} SHAPE;

typedef struct {
    int x, y;
    SHAPE shape;
} MINO;

SHAPE shapes[SHAPE_MAX] = {
    // SHAPE_I
    {
        4,4,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // SHAPE_O
    {
        2,2,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1},
            {1,1}
        }
    },
    // SHAPE_S
    {
        3,3,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,1},
            {1,1,0},
            {0,0,0}
        }
    },
    // SHAPE_Z
    {
        3,3,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,1,0},
            {0,1,1},
            {0,0,0}
        }
    },
    // SHAPE_J
    {
        3,3,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {1,0,0},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_L
        {
        3,3,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,0,1},
            {1,1,1},
            {0,0,0}
        }
    },
    // SHAPE_T
    {
        3,3,// int width, height;
        
        // int pattern[SHAPE_HEIGHT_MAX][SHAPE_WIDTH_MAX];
        {
            {0,1,0},
            {1,1,1},
            {0,0,0}
        }
    }
};

int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

MINO mino;

/* 関数プロトタイプ宣言 */
void DrawScreen(void);      /* 画面表示関数 */
void Init(void);            /* 初期化関数 */
void InitMino(void);        /* テトリミノ初期化関数 */
bool MinoIntersectField(void);   /* 当たり判定関数 */

/*---------------------------------
            メイン関数
---------------------------------*/
int main(void)
{
    /* 変数定義 */
    int x, y;
    int y2;
    clock_t lastClock;      /* 前回の更新時刻 */
    clock_t nowClock;       /* 現在時刻 */
    MINO lastMino;
    MINO newMino;       
    bool completed;     /* 列消去判定 */

    /* 乱数の種生成 */
    srand((unsigned int)time(NULL));
    Init();

    /* 前回の更新時刻 */
    lastClock = clock();
    /* ゲームのループ */
    while(1) {
        /* 現在時刻を更新 */
        nowClock = clock();
        /* 時間経過と共に落下 */
        if (nowClock >= lastClock + INTERVAL) {
            lastClock = nowClock;

            lastMino = mino;
            mino.y++;

            /* 落ちきったらフィールドと同化 */
            if (MinoIntersectField()) {
                mino = lastMino;

                for (y = 0; y < mino.shape.height; y++) {
                    for (x = 0; x < mino.shape.width; x++) {
                        if (mino.shape.pattern[y][x]) {
                            field[mino.y + y][mino.x + x] |= 1;
                        }
                    }
                }

                /* 列消去：揃っているか確認 */
                for (y = 0; y < FIELD_HEIGHT; y++) {
                    completed = true;
                    for(x = 0; x < FIELD_WIDTH; x++) {
                        if (!field[y][x]) {
                            completed = false;
                            break;
                        }
                    }
                    /* 列消去：揃っていたら消去 */
                    if (completed) {
                        for (x = 0; x < FIELD_WIDTH; x++) {
                            field[y][x] = 0;
                        }
                        /* 宙ぶらりんのフィールドを下にずらす */
                        for(y2 = y; y2 >= 1; y2--){
                            for(x = 0; x < FIELD_WIDTH; x++){
                                field[y2][x] = field[y2 - 1][x];
                                field[y2 - 1][x] = 0;
                            }
                        }
                    }
                }
                InitMino();
            }

            DrawScreen();
        }

        /* キーボードで操作 */
        if (_kbhit()) {
            lastMino = mino;
            switch (_getch())
            {
            case 'w':
                break;
            case 's':       /* 下 */
                mino.y++;
                break;
            case 'a':       /* 左 */
                mino.x--;
                break;
            case 'd':       /* 右 */
                mino.x++;
                break;
            default:        /* 回転 */
                {
                    newMino = mino;
                    for (y = 0; y < mino.shape.height; y++) {
                        for (x = 0; x < mino.shape.width; x++) {
                            newMino.shape.pattern[mino.shape.width-1-x][y] = mino.shape.pattern[y][x];          
                        }
                    }
                    mino = newMino;
                }
                break;
            }

            if (MinoIntersectField()) {
                mino = lastMino;
            }

            DrawScreen();
        }
    }

    return 0;
}

/*---------------------------------
            画面表示関数
---------------------------------*/
void DrawScreen(void)
{
    int x, y;
    /* screenにfieldをコピー */
    memcpy(screen, field, sizeof field);

    /* テトリミノ描画 */
    for (y = 0; y < mino.shape.height; y++) {
        for (x = 0; x < mino.shape.width; x++) {
            if (mino.shape.pattern[y][x] == 1) {
                screen[mino.y + y][mino.x + x] |= 1;
            }
        }
    }

    /* DOSコマンドで画面をクリア */
    system("cls"); 

    /* フィールドの描画 */
    for (y = 0; y < FIELD_HEIGHT; y++) {
        printf("口");
        for (x = 0; x < FIELD_WIDTH; x++) {
            printf("%s", screen[y][x] ? "口" : "　");
        }
        printf("口");
        printf("\n");
    }
    for (x = 0; x < FIELD_WIDTH + 2; x++) {
        printf("口");
    }    
}

/*---------------------------------
        テトリミノ初期化関数
---------------------------------*/
void InitMino(void) {
    /* テトリミノ選出 */
    mino.shape = shapes[rand() % SHAPE_MAX];
    /* テトリミノ初期位置設定 */
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
    mino.y = 0;
}

/*---------------------------------
            初期化関数
---------------------------------*/
void Init(void)
{
    /* フィールドをクリア */
    memset(field, 0,sizeof field); 

    InitMino();
    DrawScreen();
}

/*---------------------------------
            当たり判定関数
---------------------------------*/
bool MinoIntersectField(void)
{
    int x, y;
    for (y = 0; y < mino.shape.height; y++) {
        for (x = 0; x < mino.shape.width; x++) {
            if (mino.shape.pattern[y][x]) {
                if((mino.y + y < 0) || (mino.y + y >= FIELD_HEIGHT) 
                    || (mino.x + x < 0) || (mino.x + x >= FIELD_WIDTH)) {
                        return true;
                    }
                
                if ( field[mino.y + y][mino.x + x]){
                    return true;
                }
            }
        }
    }
    return false;
}