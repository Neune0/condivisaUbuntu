#ifndef COCCODRILLO_HE
#define COCCODRILLO_HE
#include "./utilities.h"

pid_t avviaCoccodrillo(int* pipe_fd, PipeData* coccodrillo_init,int id);

void coccodrillo(int* pipe_fd, PipeData* coccodrillo_init,int id);

void uccidiCoccodrillo(pid_t *pids_coccodrilli, int id_coccodrillo);
#endif
