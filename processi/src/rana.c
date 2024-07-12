#include "../hdr/rana.h"

// funzione che si occupa di avviare il processo rana e restituirne il pid
pid_t avviaRana(int* pipe_fd){
	pid_t move_pid = fork(); // pid che contiente il pid della rana
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProcess(pipe_fd);
      exit(0);
  }
	return move_pid;
}

// il main del processo rana
void moveProcess(int* pipe_fd) {
	// inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x=0;
	pipeData.y=0;
	pipeData.type='X';
	pipeData.id=0;
		
	// Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
    
	noecho();
	int ch='D';
    bool change=false; // flag tasto premuto/non premuto
	// loop principale del processo rana
	while (1) {
		
    	change=false; // reset flag tasto premuto
    	pipeData.type='X'; // resetta il normale carattere della rana
       	
        ch = getch(); // Leggi il carattere dall'input
        
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
			case 'w':
			case 'W':
				pipeData.y=-2;
            	change=true;
            break;
          case KEY_DOWN:
		  case 's':
		  case 'S':
            	pipeData.y=+2;
            	change=true;
            break;
          case KEY_LEFT:
		  case 'a':
		  case 'A':
            	pipeData.x=-1;
            	change=true;  
            break;
          case KEY_RIGHT:
		  case 'd':
		  case 'D':
            	pipeData.x=1;
            	change=true;
            break;
          case 32: // KEY_SPACE 
          	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	pipeData.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
		
		if(change){
			// Invia le coordinate attraverso la pipe
        	write(pipe_fd[1], &pipeData, sizeof(PipeData));
			// reset coordinate e tipo
			pipeData.x=0;
			pipeData.y=0;
			pipeData.type='X';
		}
         
    }
    return;
}

// per riavviare il processo rana
void resetRana(GameData* gameData){
	kill(gameData->pids.pidRana, SIGKILL);
	waitpid(gameData->pids.pidRana, NULL,0);
	gameData->pids.pidRana = avviaRana(gameData->pipe);
	inizializzaPosRana(&(gameData->ranaAbsPos));
	return;
}

void inizializzaPosRana(RanaAbsPos* ranaPos){
	ranaPos->x=40;
	ranaPos->y=MARCIAPIEDEROWSTART;
	ranaPos->on_coccodrillo=false;
	ranaPos->id_coccodrillo=-1;
	ranaPos->offset_on_coccodrillo=0;
	return;
}