#ifndef RANA_HE
#define RANA_HE

#include "./utilities.h"

pid_t avviaRana(int* pipe_fd);
void moveProcess(int* pipe_fd);
void resetRana(GameData* gameData);
void inizializzaPosRana(RanaAbsPos* ranaPos);
#endif

