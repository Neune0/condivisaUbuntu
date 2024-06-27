#include "rana.h"

pthread_t avviaRanaThread(ParamsOggetti *thread_args){
	pthread_t tid_rana;
	
	if( pthread_create(&tid_rana, NULL, &moveRanaThread, thread_args) != 0){
		perror("ERR: Avvio Rana Fallito!");
		_exit(2); 
	}
	else{
		return tid_rana;
	}
	return tid_rana;
}

void *moveRanaThread(void *param){
	ParamsOggetti *p = (ParamsOggetti*) param;
	
	// ThreadControlBlock interno del thread Rana preso direttamente da gamedata e collegato ad esso
	ThreadControlBlock *my_tcb = p->thread_tcb;
	my_tcb->thread_id = pthread_self(); // ci salvo il suo thread_id
	
	// inizializzazione pipeData per comunicazione
	PipeData new_pos;
	// coordinate relative iniziali nulle
	new_pos.x=p->init->x;
	new_pos.y=p->init->y;
	new_pos.type=p->init->type;
	new_pos.id=p->init->id;
	new_pos.thread_id = pthread_self();
	
	noecho();
	int ch='D';
    bool change=false; // flag tasto premuto/non premuto

	// loop principale del processo rana
	while (1) {
    	change=false; // reset flag tasto premuto
    	new_pos.type='X'; // resetta il normale carattere della rana
       	
		if(my_tcb->is_target){
			break;
		}
        
        ch = getch(); // Leggi il carattere dall'input
		
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
				new_pos.y=-2;
            	change=true;
            break;
          case KEY_DOWN:
            	new_pos.y=+2;
            	change=true;
            break;
          case KEY_LEFT:
            	new_pos.x=-1;
            	change=true;  
            break;
          case KEY_RIGHT:
            	new_pos.x=1;
            	change=true;
            break;
          case 32: // KEY_SPACE 
          	new_pos.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	new_pos.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
		
		if(change){
			// Invia le coordinate sul buffer
			scriviSuBuffer(p, new_pos, my_tcb, false);

			// reset coordinate e tipo
			new_pos.x=0;
			new_pos.y=0;
			new_pos.type='X';
		}

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(1000);
    }
	new_pos.type='x'; // carattere di fine rana
	scriviSuBuffer(p, new_pos, my_tcb, true);	// ultima scrittura sul buffer, segna su TCB che ha terminato

    pthread_exit(NULL);
	return NULL;

}
