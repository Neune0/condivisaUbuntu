#ifndef RANA_HE
#define RANA_HE

#include "utilities.h"

/** @brief Lancia il thread Rana e restituisce il thread_id
 * @param p: struttura con i semafori e buffer
*/
pthread_t avviaRanaThread(ParamsOggetti *p);

/** @brief Thread principale della Rana
 * 
*/
void *moveRanaThread(void *param);
#endif

