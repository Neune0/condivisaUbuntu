#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "utilities.h"
#include "rana.h"
#include "hud.h"

/** @brief inizializza la struttura dati del gioco
 * @param gameData : la struttura dati del gioco
*/
void inizializza(GameData* gameData);

/** @brief Inizializza valori per i TCB
*/
void inizializzaTCB(AllTCB *tcbs);

/***/
void inizializzaGameInfo(GameData* gamedata);

/***/
void inizializzaContatori(GameData* gameData);

/***/
void inizializzaSchermo(GameData* gameData);

/***/
void inizializzaOldPos(OldPos *old_pos);

/***/
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaArgine(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaTane(GameData* gameData);

/***/
void inizializzaHUD(GameData* gameData);

/***/
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

/** @brief Imposta i valori iniziali della Rana nella matrice Schermo
 * @param ranaPos :  puntatore alle coordinate assolute della Rana nella matrice Schermo 
*/
void inizializzaPosRana(RanaAbsPos* ranaPos);

/***/
void inizializzaOggettiTane(Tana* tane);

/***/
void inizializzaSprites(Sprite* sprites);

Sprite inizializzaSprite(int rows, int cols, char sprite[rows][cols], int color, TipoObj tipo_oggetto);

/***/
void inizializzaBox(GameData* gameData);

/***/
void inizializzaTempo(GameData* gameData);

#endif
