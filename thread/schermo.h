#ifndef SCHERMO_H
#define SCHERMO_H
#include "utilities.h"
#include "proiettile.h"
#include "collisione.h"
#include "rana.h"

void aggiorna(GameData* gameData, Semaphore* semafori, PipeData* buffer);

void cancellaOggetto(GameData* gameData, PipeData *old_pos, TipoSprite tipoSprite);

void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite);

void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite,GameData* gameData);

void pulisciSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, GameData* gameData);

void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);

void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]);

void stampaTanaChiusa(Tana tana, GameData* gameData);

void aggiornaTempo(GameData* gameData);

void normalUpdate(GameData* gameData, Semaphore* semafori, PipeData* buffer);

#endif