#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAX_BALAS 5  

int x = 2, y = 22;             
int prevx, prevy;              
int inimigoX = 30, inimigoY = 5;   
int inimigoVivo = 1;            

typedef struct {
    int balaX;
    int balaY;
    int ativa;
} Bala;

Bala balas[MAX_BALAS];  

void imprimeJogador();
void criaBala();
void moveBalas();

int main() {
    int tecla = 0;
    screenInit(1);
    keyboardInit();
    timerInit(150);

    for (int i = 0; i < MAX_BALAS; i++) {
        balas[i].ativa = 0;
    }

    while (tecla != 10) {  
        if (keyhit()) {
            tecla = readch();
        }

        if (timerTimeOver() == 1) {
            if (tecla == 'a' || tecla == 'A') {
                if (x > 0) {
                    x -= 2;
                }
                tecla = 0;
            } else if (tecla == 'd' || tecla == 'D') {
                if (x < 38) {
                    x += 2;
                }
                tecla = 0;
            }

            if (tecla == ' ' ) {
                criaBala();
                tecla = 0;
            }

            imprimeJogador();
            moveBalas();

            if (inimigoVivo) {
                screenSetColor(GREEN, DARKGRAY);
                screenGotoxy(inimigoX, inimigoY);
                printf(" X ");
            }

            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

void imprimeJogador() {
    screenGotoxy(prevx, prevy);
    printf("   ");
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(x, y);
    printf(" △ ");
    prevx = x;
    prevy = y;
}

void criaBala() {
    for (int i = 0; i < MAX_BALAS; i++) {
        if (!balas[i].ativa) {
            balas[i].balaX = x;
            balas[i].balaY = y - 1;
            balas[i].ativa = 1;
            break;
        }
    }
}

void moveBalas() {
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i].ativa) {
            screenGotoxy(balas[i].balaX, balas[i].balaY);
            printf("   ");

            balas[i].balaY -= 1;

            if (inimigoVivo && balas[i].balaY == inimigoY && balas[i].balaX == inimigoX) {
                balas[i].ativa = 0;
                screenGotoxy(inimigoX, inimigoY);
                printf("   ");
                inimigoVivo = 0;
                continue;
            }

            if (balas[i].balaY <= 1) {
                balas[i].ativa = 0;
            } else {
                screenSetColor(YELLOW, DARKGRAY);
                screenGotoxy(balas[i].balaX, balas[i].balaY);
                printf("^");
            }
        }
    }
}