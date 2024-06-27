#ifndef COLLISIONE_H
#define COLLISIONE_H
#include "utilities.h"

Collisione detectCollisione(GameData* gameData);
void handleCollisione(GameData* gameData, Collisione collisione, Semaphore* semafori);
#endif // COLLISIONE_H