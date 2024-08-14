#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

/* �t�B�[���h��/���� */
#define FIELD_WIDTH     (10)
#define FIELD_HEIGHT    (20)

/* �e�g���~�m��/���� */
#define SHAPE_WIDTH_MAX     (4)
#define SHAPE_HEIGHT_MAX    (4)

/* �������x */
#define FPS         (1)
#define INTERVAL    (1000 / FPS)

/* �e�g���~�m�̌`���` */
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

/* �֐��v���g�^�C�v�錾 */
void DrawScreen(void);      /* ��ʕ\���֐� */
void Init(void);            /* �������֐� */
void InitMino(void);        /* �e�g���~�m�������֐� */
bool MinoIntersectField(void);   /* �����蔻��֐� */

/*---------------------------------
            ���C���֐�
---------------------------------*/
int main(void)
{
    /* �ϐ���` */
    int x, y;
    int y2;
    clock_t lastClock;      /* �O��̍X�V���� */
    clock_t nowClock;       /* ���ݎ��� */
    MINO lastMino;
    MINO newMino;       
    bool completed;     /* ��������� */

    /* �����̎퐶�� */
    srand((unsigned int)time(NULL));
    Init();

    /* �O��̍X�V���� */
    lastClock = clock();
    /* �Q�[���̃��[�v */
    while(1) {
        /* ���ݎ������X�V */
        nowClock = clock();
        /* ���Ԍo�߂Ƌ��ɗ��� */
        if (nowClock >= lastClock + INTERVAL) {
            lastClock = nowClock;

            lastMino = mino;
            mino.y++;

            /* ������������t�B�[���h�Ɠ��� */
            if (MinoIntersectField()) {
                mino = lastMino;

                for (y = 0; y < mino.shape.height; y++) {
                    for (x = 0; x < mino.shape.width; x++) {
                        if (mino.shape.pattern[y][x]) {
                            field[mino.y + y][mino.x + x] |= 1;
                        }
                    }
                }

                /* ������F�����Ă��邩�m�F */
                for (y = 0; y < FIELD_HEIGHT; y++) {
                    completed = true;
                    for(x = 0; x < FIELD_WIDTH; x++) {
                        if (!field[y][x]) {
                            completed = false;
                            break;
                        }
                    }
                    /* ������F�����Ă�������� */
                    if (completed) {
                        for (x = 0; x < FIELD_WIDTH; x++) {
                            field[y][x] = 0;
                        }
                        /* ���Ԃ���̃t�B�[���h�����ɂ��炷 */
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

        /* �L�[�{�[�h�ő��� */
        if (_kbhit()) {
            lastMino = mino;
            switch (_getch())
            {
            case 'w':
                break;
            case 's':       /* �� */
                mino.y++;
                break;
            case 'a':       /* �� */
                mino.x--;
                break;
            case 'd':       /* �E */
                mino.x++;
                break;
            default:        /* ��] */
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
            ��ʕ\���֐�
---------------------------------*/
void DrawScreen(void)
{
    int x, y;
    /* screen��field���R�s�[ */
    memcpy(screen, field, sizeof field);

    /* �e�g���~�m�`�� */
    for (y = 0; y < mino.shape.height; y++) {
        for (x = 0; x < mino.shape.width; x++) {
            if (mino.shape.pattern[y][x] == 1) {
                screen[mino.y + y][mino.x + x] |= 1;
            }
        }
    }

    /* DOS�R�}���h�ŉ�ʂ��N���A */
    system("cls"); 

    /* �t�B�[���h�̕`�� */
    for (y = 0; y < FIELD_HEIGHT; y++) {
        printf("��");
        for (x = 0; x < FIELD_WIDTH; x++) {
            printf("%s", screen[y][x] ? "��" : "�@");
        }
        printf("��");
        printf("\n");
    }
    for (x = 0; x < FIELD_WIDTH + 2; x++) {
        printf("��");
    }    
}

/*---------------------------------
        �e�g���~�m�������֐�
---------------------------------*/
void InitMino(void) {
    /* �e�g���~�m�I�o */
    mino.shape = shapes[rand() % SHAPE_MAX];
    /* �e�g���~�m�����ʒu�ݒ� */
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
    mino.y = 0;
}

/*---------------------------------
            �������֐�
---------------------------------*/
void Init(void)
{
    /* �t�B�[���h���N���A */
    memset(field, 0,sizeof field); 

    InitMino();
    DrawScreen();
}

/*---------------------------------
            �����蔻��֐�
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