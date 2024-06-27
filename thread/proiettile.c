#include "proiettile.h"

pthread_t avviaProiettileThread(ParamsOggetti *thread_arg){
	pthread_t tid_proiettile;

	// crea il thread proiettile e controlla.
	if(pthread_create(&tid_proiettile, NULL, &moveProiettileThread,thread_arg) != 0){
		perror("ERR: Avvio Proiettile Fallito!");
		_exit(3);
	}
	else{
		return tid_proiettile;
	
	}
	return tid_proiettile;
}

void *moveProiettileThread(void *param)
{
	// cast parametri
	ParamsOggetti *parametri = (ParamsOggetti *)param;

	int controlla=0;

	int dirY = -1; // direzione del movimento del proiettile
	
	ThreadControlBlock *my_tcb = parametri->thread_tcb;
	
	PipeData proiettile;
	proiettile.x = parametri->init->x;
	proiettile.y = parametri->init->y;
	proiettile.type = 'P';
	proiettile.id = parametri->init->id;
	proiettile.thread_id = my_tcb->thread_id;

	ThreadControlBlock deb = *(my_tcb);
	int aspetta=0;
	my_tcb->thread_id = pthread_self(); // ci salvo il suo thread_id

	while (1)
	{
		if(my_tcb->is_target){
			break;
		}
		proiettile.y = proiettile.y + dirY;
		scriviSuBuffer(parametri, proiettile, my_tcb, false);
		usleep(100000);
	}

	// indica l'ultima scrittura su buffer del proiettile
	proiettile.type='Y';
	scriviSuBuffer(parametri, proiettile, my_tcb, true);
	
	pthread_exit(NULL);
	return NULL;
}