#include "../hdr/disegna.h"

void drawProcess(int *pipe_fd)
{
	// la struttura che contiene i dati per la gestione del gioco
	GameData *gameData = (GameData *)malloc(sizeof(GameData));

	avviaProcessiBase(pipe_fd, &(gameData->pids));

	// inizializza i dati del gioco
	inizializza(gameData, pipe_fd);

	// prima stampa dello schermo
	printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
	stampaMatrice(gameData->schermo.screenMatrix);

	int sec;
	int contatore_pari = 0;
	int contatore_dispari = 0;

	// loop principale di gioco
	while (!isGameOver(gameData))
	{

		read(pipe_fd[0], &(gameData->pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe

		// per debug
		switch (gameData->pipeData.type)
		{
		case 'T': // tempo
			// non fa nulla
			mvprintw(8, 106, "                                            ");
			mvprintw(8, 106, "Last T: sec: %d", gameData->pipeData.x);
			break;
		case 'X': // rana
			mvprintw(0, 106, "                                            ");
			mvprintw(0, 106, "Last X: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'C':
			mvprintw(1, 106, "                                            ");
			mvprintw(1, 106, "Last C: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'c':
			mvprintw(2, 106, "                                            ");
			mvprintw(2, 106, "Last c: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'n':
			mvprintw(3, 106, "                                            ");
			mvprintw(3, 106, "Last n: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'p':
			mvprintw(4, 106, "                                            ");
			mvprintw(4, 106, "Last p: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'P':
			mvprintw(5, 106, "                                            ");
			mvprintw(5, 106, "Last P: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 'S': // proiettile sparato da rana
			mvprintw(6, 106, "                                            ");
			mvprintw(6, 106, "Last S: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		case 's':
			mvprintw(7, 106, "                                            ");
			mvprintw(7, 106, "Last s: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
			break;
		default:
			break;
		}
		// fine debug

		aggiorna(gameData); // aggiorna stato del gioco
		printVite(gameData);
		printManche(gameData);

		sec = gameData->gameInfo.secondi_di_gioco;

		if (sec % 2 == 0)
		{
			contatore_dispari = 0;
			contatore_pari++;
		}
		else
		{
			contatore_pari = 0;
			contatore_dispari++;
		}

		// questa parte va chiusa dentro una funzione a parte

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 1) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{
			beep();
			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[0].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_UNO;
				coccodrillo_init.type = gameData->flussi[0].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id, gameData->flussi[0].direction,gameData->flussi[0].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[0].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 2) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[1].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_DUE;
				coccodrillo_init.type = gameData->flussi[1].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id, gameData->flussi[1].direction,gameData->flussi[1].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[1].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 3) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[2].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_TRE;
				coccodrillo_init.type = gameData->flussi[2].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id, gameData->flussi[2].direction,gameData->flussi[2].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[2].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 5) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[4].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_CINQUE;
				coccodrillo_init.type = gameData->flussi[4].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id,gameData->flussi[4].direction,gameData->flussi[4].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[4].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 7) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[6].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_SETTE;
				coccodrillo_init.type = gameData->flussi[6].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id,gameData->flussi[6].direction,gameData->flussi[6].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[6].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 8) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[7].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_OTTO;
				coccodrillo_init.type =  gameData->flussi[7].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id,gameData->flussi[7].direction,gameData->flussi[7].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[7].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 6) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[5].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_SEI;
				coccodrillo_init.type = gameData->flussi[5].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id,gameData->flussi[5].direction,gameData->flussi[5].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[5].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 4) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[3].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_QUATTRO;
				coccodrillo_init.type = gameData->flussi[3].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id,gameData->flussi[3].direction,gameData->flussi[3].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[3].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		avviaNemici(gameData);
		for(int i=0;i<8;i++){
			mvprintw(20+i,106,"                            ");
			mvprintw(20+i,106,"flusso %d D:%d V:%d",i,gameData->flussi[i].direction,gameData->flussi[i].vel);
		}
		mvprintw(33,106,"                                         ");
		mvprintw(33,106,"old pos nemici[1]: x:%d y:%d id:%d ",gameData->oldPos.nemici[1].x,gameData->oldPos.nemici[1].y,gameData->oldPos.nemici[1].id);
		// per debug
		mvprintw(34, 106, "                                            ");
		mvprintw(34, 106, "Rana x: %d y: %d idC: %d offC: %d", gameData->ranaAbsPos.x, gameData->ranaAbsPos.y, gameData->ranaAbsPos.id_coccodrillo, gameData->ranaAbsPos.offset_on_coccodrillo);
		mvprintw(35, 106, "                                            ");
		mvprintw(35, 106, "ctPN: %d, ctN: %d, ctP: %d, ctC: %d", gameData->contatori.contProiettiliN, gameData->contatori.contNemici, gameData->contatori.contProiettili, gameData->contatori.contCoccodrilli);

		mvprintw(36, 106, "                                  ");
		mvprintw(36, 106, "tempo di gioco: %d secondi", sec);
		mvprintw(37, 106, "                                            ");
		mvprintw(37, 106, "contPari: %d contDispari: %d", contatore_pari, contatore_dispari);
		mvprintw(38, 106, "                                            ");
		mvprintw(38, 106, "livello: %d manche: %d", gameData->gameInfo.livello,gameData->gameInfo.manche);

		// se il tempo di gioco supera il max per manche la rana muore e la manche riparte
		// mi serve una funzione per lo start della manche
		if (gameData->gameInfo.secondi_di_gioco > TEMPOLVL1)
		{
			// uccidi rana e restart rana
			gameData->gameInfo.vite--;
			resetRana(gameData);
			gameData->ranaAbsPos.on_coccodrillo = false;
			gameData->ranaAbsPos.id_coccodrillo = -1;
			aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);

			// termino e faccio ripartire il processo che gestisce il tempo
			resetTempo(gameData);
			gameData->gameInfo.secondi_di_gioco=0;
			// reinizializzo l'hud del tempo
			printInitTempo(gameData);
		}
		refresh();
		// fine debug
	}

	// if sul tipo di uscita dal gioco
	if (isWin(gameData))
	{
		stampaWin();
	}
	else
	{
		stampaGameOver();
	}

	free(gameData);
	return;
}

void avviaDrawProcess(int pipe_fd[2])
{
	pid_t draw_pid = fork();
	if (draw_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else if (draw_pid == 0)
	{
		// Processo "disegna"
		drawProcess(pipe_fd);
		exit(0); // TODO: è davvero utile?
	}
}

void stampaWin()
{
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU, STARTCOLLOGOMENU);
	mvprintw(15, 80, "you win!!!!!!!!!!!!!");
	refresh();
	usleep(30000000);
	return;
}

void stampaGameOver()
{
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU, STARTCOLLOGOMENU);
	mvprintw(15, 80, "Game Over!");
	mvprintw(17,80,"premi un tasto per continuare");
	refresh();
	nodelay(stdscr, FALSE);
	getch();
	return;
}

void printRana(PipeData *datiRana, Sprite *sprite, GameData *gameData)
{
	int startRow = datiRana->y;
	int startCol = datiRana->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	int row = 0, col = 0;

	Schermo *schermo = &(gameData->schermo);

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
			schermo->screenMatrix[row][col].color = sprite->color;
			schermo->screenMatrix[row][col].is_changed = true;
			schermo->screenMatrix[row][col].id = datiRana->id;
		}
	}
	return;
}

bool thereIsSpaceForCoccodrilloOnFila(GameData *gameData, int fila)
{
	int begin_row = 0;
	int begin_col = FIRSTGAMECOL;
	bool space = false;
	if (gameData->contatori.contCoccodrilli < MAXNCOCCODRILLI)
	{

		switch (fila)
		{
		case 1:
			begin_row = FILA_UNO;
			break;
		case 2:
			begin_row = FILA_DUE;
			
			break;
		case 3:
			begin_row = FILA_TRE;
			break;
		case 4:
			begin_row = FILA_QUATTRO;
			
			break;
		case 5:
			begin_row = FILA_CINQUE;
			break;
		case 6:
			begin_row = FILA_SEI;
			
			break;
		case 7:
			begin_row = FILA_SETTE;
			break;
		case 8:
			begin_row = FILA_OTTO;
			
			break;
		default:
			break;
		}

		begin_col = gameData->flussi[fila-1].direction==1 ? FIRSTGAMECOL :  INITCOCCODRILLOSXCOL;

		for (int i = begin_row; i < begin_row + COCCODRILLO_H; i++)
		{
			for (int j = begin_col; j < begin_col + COCCODRILLO_W; j++)
			{
				if (gameData->schermo.screenMatrix[i][j].tipo == FIUME_OBJ)
				{
					space = true;
				}
			}
		}
	}
	return space;
}

void avviaNemici(GameData* gameData){
	int id = id_disponibile(gameData->pids.pidNemici,MAXNNEMICI);
	if(id!=-1){
		gameData->pids.pidNemici[id]=avviaNemico(gameData->pipe,id);
	}
}