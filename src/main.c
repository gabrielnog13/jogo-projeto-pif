#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAX_BALAS 20
#define NUM_INIMIGOS 20 
#define MAX_BALAS_INIMIGAS 40

int x = 2, y = 22;  
int Pontuacao = 0;
int prevx, prevy;              
int inimigoX[NUM_INIMIGOS], inimigoY[NUM_INIMIGOS];   
int inimigoVivo[NUM_INIMIGOS]; 
int gameOver = 0;

typedef struct {
    int balaX;
    int balaY;
    int ativa;
} Bala;

Bala balas[MAX_BALAS];  
Bala balasInimigas[MAX_BALAS_INIMIGAS];

void imprimeJogador();
void criaBala();
void moveBalas();
void criaInimigos();
void MostrarPontuacao();
void criaBalasInimigas();
void moveBalasInimigas();
void verificaColisaoJogador();

int main() {
    int tecla = 0;
    screenInit(1);
    keyboardInit();
    timerInit(150);

    srand(time(NULL));

    for (int i = 0; i < MAX_BALAS; i++) {
        balas[i].ativa = 0;
    }

    for (int i = 0; i < MAX_BALAS_INIMIGAS; i++) {
        balasInimigas[i].ativa = 0;
    }

    criaInimigos();

    while (tecla != 10 && !gameOver) {  
        if (keyhit()) {
            tecla = readch();
        }
        MostrarPontuacao();

        if (timerTimeOver() == 1) {
            if (tecla == 'a' || tecla == 'A') {
                if (x > 2) {
                    x -= 2;
                }
                tecla = 0;
            } else if (tecla == 'd' || tecla == 'D') {
                if (x < 76) {
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
            criaBalasInimigas();
            moveBalasInimigas();
            verificaColisaoJogador();

            for (int i = 0; i < NUM_INIMIGOS; i++) {
                if (inimigoVivo[i]) {
                    screenSetColor(GREEN, DARKGRAY);
                    screenGotoxy(inimigoX[i], inimigoY[i]);
                    printf(" 👾 ");
                }
            }

            screenUpdate();
        }
    }

    if (gameOver) {
        screenClear();
        screenGotoxy(35, 12);
        printf("Game Over!");
        screenGotoxy(30, 14);
        printf("Pontuação Final: %d", Pontuacao);
        screenUpdate();
        sleep(3);
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
    printf(" 🛸 ");
    prevx = x;
    prevy = y;
}

void criaBala() {
    for (int i = 0; i < MAX_BALAS; i++) {
        if (!balas[i].ativa) {
            balas[i].balaX = x + 1;
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

            for (int j = 0; j < NUM_INIMIGOS; j++) {
                if (inimigoVivo[j] && balas[i].balaY == inimigoY[j] && 
                    balas[i].balaX >= inimigoX[j] && balas[i].balaX <= inimigoX[j] + 2) {
                    balas[i].ativa = 0; 
                    screenGotoxy(inimigoX[j], inimigoY[j]);
                    printf("   ");
                    inimigoVivo[j] = 0; 
                    Pontuacao += 10;
                    break; 
                }
            }

            if (balas[i].balaY <= 1) {
                balas[i].ativa = 0; 
            }

            if (balas[i].ativa) {
                screenSetColor(YELLOW, DARKGRAY);
                screenGotoxy(balas[i].balaX, balas[i].balaY);
                printf("^");
            }
        }
    }
}

void criaInimigos() {
    int inicioX = 25;
    int inicioY = 5;  
    int espaco = 3;  
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        inimigoX[i] = inicioX + (i % 10) * espaco; 
        inimigoY[i] = inicioY + (i / 10) * 2;    
        inimigoVivo[i] = 1; 
    }
}

void MostrarPontuacao() {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(2, 0);
    printf("Pontuação: %d ", Pontuacao);
}

void criaBalasInimigas() {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        if (inimigoVivo[i] && rand() % 100 < 2) { 
            for (int j = 0; j < MAX_BALAS_INIMIGAS; j++) {
                if (!balasInimigas[j].ativa) {
                    balasInimigas[j].balaX = inimigoX[i] + 1;
                    balasInimigas[j].balaY = inimigoY[i] + 1;
                    balasInimigas[j].ativa = 1;
                    break;
                }
            }
        }
    }
}

void moveBalasInimigas() {
    for (int i = 0; i < MAX_BALAS_INIMIGAS; i++) {
        if (balasInimigas[i].ativa) {
            screenGotoxy(balasInimigas[i].balaX, balasInimigas[i].balaY);
            printf("   ");

            balasInimigas[i].balaY += 1;

            if (balasInimigas[i].balaY >= 24) {
                balasInimigas[i].ativa = 0;
            }

            if (balasInimigas[i].ativa) {
                screenSetColor(RED, DARKGRAY);
                screenGotoxy(balasInimigas[i].balaX, balasInimigas[i].balaY);
                printf("v");
            }
        }
    }
}

void verificaColisaoJogador() {
    for (int i = 0; i < MAX_BALAS_INIMIGAS; i++) {
        if (balasInimigas[i].ativa) {
            if (balasInimigas[i].balaY == y && 
                balasInimigas[i].balaX >= x && balasInimigas[i].balaX <= x + 2) {
                gameOver = 1;
                return;
            }
        }
    }
}