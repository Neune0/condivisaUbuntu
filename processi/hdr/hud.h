#ifndef DEBUG_H
#define DEBUG_H
#include "./utilities.h"

/***/
void printVite(GameData* GameData);

/***/
void printInitTempo(GameData* gameData);

/***/
void printTempo(GameData* gameData);

/***/
void printDefaultHudSup(GameData* gameData);

/***/
void printLvl(GameData* gameData);

/***/
void printManche(GameData* gameData);

/***/
void printScore(GameData* gameData);

/***/
void aggiornaHud(GameData* gameData);

void printViteMinus();

void printDigit(Schermo *schermo, char digit[CIFRA_H][CIFRA_W], int startRow, int startCol, int color);

void clearScreenArea(Schermo *schermo, int startRow, int startCol, int width, int height);

char (*getDigits())[CIFRA_H][CIFRA_W];
#endif
