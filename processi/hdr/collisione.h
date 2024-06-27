#ifndef COLLISIONE_H
#define COLLISIONE_H
#include "./utilities.h"
#include "./rana.h"
#include "./schermo.h"

Collisione detectCollisione(GameData* gameData);
void handleCollisione(GameData* gameData,Collisione collisione);
#endif // COLLISIONE_H