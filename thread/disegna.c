#include "disegna.h"

#define DEBUG

void *drawThread (void *param){
	// recupero dei parametri passati al thread
	ParamsDisegna* parametri = (ParamsDisegna* ) param; 
	
	ParamsDisegna argomentiPerDebug = *parametri;
	PipeData bufferDebug = *(argomentiPerDebug.buffer);
	Semaphore semaforiDebug = *(argomentiPerDebug.semafori);

	int controlla =0;
	// scompone i parametri
	GameData *gameData = parametri->gameData; // recupera gameData
	Semaphore *semafori = parametri->semafori; // recupera semafori
	PipeData *buffer = parametri->buffer; // recupera buffer
	
	inizializza(gameData); // inizializza i dati del gioco

	// inizializza la rana
	PipeData init = {0,0,'X',0,0};
	ParamsOggetti arg_rana = {semafori, buffer, &(gameData->allTCB.tcb_rana), &init};
	gameData->allTCB.tcb_rana.thread_id = avviaRanaThread(&arg_rana);
	
	printRana(&(gameData->oldPos.rana),&(gameData->sprites[S_RANA]),gameData); // prima stampa della rana
	
	stampaMatrice(gameData->schermo.screenMatrix); // prima stampa dello schermo N.B tutti i flag adesso sono a false
	refresh();
	
	// loop principale di gioco
	while (!isGameOver(gameData)) {

		// legge dal buffer ed inserisce i dati in pipedata all'interno di gamedata
		leggiDaBuffer(semafori,buffer,&(gameData->pipeData)); 
		
		// stampe di debug
		#ifdef DEBUG // per debug
		
		mvprintw(0,106,"                                      ");
		mvprintw(0,106,"pipeData: %c x: %d y: %d id: %d",gameData->pipeData.type,gameData->pipeData.x,gameData->pipeData.y,gameData->pipeData.id);

		mvprintw(1,106,"                                      ");
		mvprintw(1,106,"RANA tid: %ld",gameData->allTCB.tcb_rana.thread_id);

		mvprintw(2,106,"               ");
		mvprintw(2,106,"Pos rana x: %d y: %d",gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);

		mvprintw(4,106,"Da disegna:");
		mvprintw(5,106,"                                      ");
		mvprintw(5,106,"punt gameData: %p",gameData);
		mvprintw(6,106,"                                      ");
		mvprintw(6,106,"punt semafori: %p",semafori);
		mvprintw(7,106,"                                      ");
		mvprintw(7,106,"punt buffer: %p",buffer);

		mvprintw(20,106,"TCB PROIETTILI PRE UPDATE per indice");
		mvprintw(21,106,"                                          ");
		mvprintw(21,106,"0: %ld",gameData->allTCB.tcb_proiettili[0].thread_id);
		mvprintw(22,106,"                                          ");
		mvprintw(22,106,"1: %ld",gameData->allTCB.tcb_proiettili[1].thread_id);
		mvprintw(23,106,"                                          ");
		mvprintw(23,106,"2: %ld",gameData->allTCB.tcb_proiettili[2].thread_id);

		mvprintw(14,106,"                                ");
		mvprintw(14,106,"CONT PROIETTILI PRE UPDATE: %d",gameData->contatori.contProiettili);
		
		#endif // fine debug

		aggiorna(gameData,semafori,buffer); // aggiorna stato del gioco e gestisce collisioni
		printTempo(gameData); // aggiorna hud del tempo
		
		aggiornaHud(gameData); // aggiorna hud del gioco
		
		// stampa puntuale della matrice
		stampaMatrice(gameData->schermo.screenMatrix);

		// stampe di debug 2
		#ifdef DEBUG // per debug

		mvprintw(16,106,"                                ");
		mvprintw(16,106,"CONT PROIETTILI POST UPDATE: %d",gameData->contatori.contProiettili);

		mvprintw(25,106,"TCB PROIETTILI POST UPDATE per indice");
		mvprintw(26,106,"                                          ");
		mvprintw(26,106,"0: %ld",gameData->allTCB.tcb_proiettili[0].thread_id);
		mvprintw(27,106,"                                          ");
		mvprintw(27,106,"1: %ld",gameData->allTCB.tcb_proiettili[1].thread_id);
		mvprintw(28,106,"                                          ");
		mvprintw(28,106,"2: %ld",gameData->allTCB.tcb_proiettili[2].thread_id);

		mvprintw(31,106,"                         ");
		mvprintw(31,106,"ViteRana da gameData: %d",gameData->gameInfo.vite);

		#endif // fine debug
		
		// Aggiorna la finestra di ncurses
		refresh();
	}

	// if sul tipo di uscita dal gioco
	if(isWin(gameData)){
		stampaWin();
	}
	else{
		stampaGameOver();
	}
	
	pthread_exit(NULL);
}

pthread_t avviaDrawThread(ParamsDisegna *thread_args){
	pthread_t tid_draw;
	if( pthread_create(&tid_draw, NULL, &drawThread, (void*)thread_args) != 0){
		perror("ERR: Avvio DrawThread Fallito!");
		_exit(1);
	}
	return tid_draw;
}

void stampaWin(){
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"you win!!!!!!!!!!!!!");
	refresh();
	usleep(30000000);
	return;
}

void stampaGameOver(){
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"Game Over!");
	refresh();
	usleep(3000000);
	return;
}

void printRana(PipeData* datiRana, Sprite* sprite,GameData* gameData){
	int startRow=datiRana->y;
    int startCol=datiRana->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    int row=0, col=0;

	Schermo* schermo = &(gameData->schermo);
	
    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            row = startRow + i;
            col = startCol + j;
		
            schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
            schermo->screenMatrix[row][col].color = sprite->color;
            schermo->screenMatrix[row][col].is_changed = true;
            schermo->screenMatrix[row][col].id=datiRana->id;
    	}
    }
	return;
}