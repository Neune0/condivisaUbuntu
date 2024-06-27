#ifndef PROIETTILE_HE
#define PROIETTILE_HE

#include "utilities.h"

/** @brief Funzione del thread proiettile
 * 
*/
void *moveProiettileThread(void *param);

/** @brief Avvia il thread proiettile con i parametri 
 * @param thread_arg: Puntatore alla struttura con i thread_arguments
 * @param id: ID dell'oggetto proiettile
 * @return (pthread_t) Ritorna il thread_id del proiettile
*/
pthread_t avviaProiettileThread(ParamsOggetti *thread_arg);
#endif
