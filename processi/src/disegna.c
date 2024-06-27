#include "../hdr/disegna.h"

void drawProcess(int *pipe_fd)
{
	// la struttura che contiene i dati per la gestione del gioco
	GameData *gameData = (GameData *)malloc(sizeof(GameData));

	avviaProcessiBase(pipe_fd, &(gameData->pids));

	// inizializza i dati del gioco
	inizializza(gameData, pipe_fd);
	gameData->contatori.contNemici = 4;

	// prima stampa dello schermo
	printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
	stampaMatrice(gameData->schermo.screenMatrix);

	int sec;
	int contatore_pari=0;
	int contatore_dispari=0;

	
	

	// loop principale di gioco
	while (!isGameOver(gameData))
	{

		read(pipe_fd[0], &(gameData->pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe

		// per debug
		mvprintw(0, 106, "                                   ");
		mvprintw(0, 106, "pipeData type:%c id:%d x:%d y:%d", gameData->pipeData.type, gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		refresh();
		// fine debug

		aggiorna(gameData); // aggiorna stato del gioco
		// printTempo(gameData); // aggiorna hud del tempo
		// stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		// refresh(); // Aggiorna la finestra

		sec = gameData->gameInfo.tempo.milliseconds / 1000;
		
		if(sec%2==0){
			contatore_dispari=0;
			contatore_pari++;
		}
		else{
			contatore_pari=0;
			contatore_dispari++;
		}

		
			if (thereIsSpaceForCoccodrilloOnFila(gameData, 1) && sec%7==0 && contatore_dispari==1)
			{
				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = FIRSTGAMECOL - 9;
					coccodrillo_init.y = FILA_UNO;
					coccodrillo_init.type = 'C';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}



	
			if (thereIsSpaceForCoccodrilloOnFila(gameData, 3) && sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = FIRSTGAMECOL - 9;
					coccodrillo_init.y = FILA_TRE;
					coccodrillo_init.type = 'C';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 5) && sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = FIRSTGAMECOL - 9;
					coccodrillo_init.y = FILA_CINQUE;
					coccodrillo_init.type = 'C';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 7) && sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = FIRSTGAMECOL - 9;
					coccodrillo_init.y = FILA_SETTE;
					coccodrillo_init.type = 'C';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 8)&& sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = LASTGAMECOL;
					coccodrillo_init.y = FILA_OTTO;
					coccodrillo_init.type = 'c';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=-1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 6)&& sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = LASTGAMECOL;
					coccodrillo_init.y = FILA_SEI;
					coccodrillo_init.type = 'c';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=-1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 4)&& sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = LASTGAMECOL;
					coccodrillo_init.y = FILA_QUATTRO;
					coccodrillo_init.type = 'c';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=-1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}

			if (thereIsSpaceForCoccodrilloOnFila(gameData, 2) && sec%7==0 && contatore_dispari==1)
			{

				// avvia coccodrillo
				int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
				if (id != -1)
				{
					PipeData coccodrillo_init;
					coccodrillo_init.x = LASTGAMECOL;
					coccodrillo_init.y = FILA_DUE;
					coccodrillo_init.type = 'c';
					coccodrillo_init.id = id;
					int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
					gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
					gameData->contatori.contCoccodrilli++;
					gameData->controlloCoccodrilli[id].direction=-1;
					gameData->controlloCoccodrilli[id].id=id;
					gameData->controlloCoccodrilli[id].offset_deep=0;
					gameData->controlloCoccodrilli[id].is_buono=false;
					gameData->controlloCoccodrilli[id].is_going_deep=false;
					gameData->controlloCoccodrilli[id].is_going_up=false;
					gameData->controlloCoccodrilli[id].passi=0;
				}
			}
	

		
		mvprintw(2, 106, "                                            ");
		mvprintw(2, 106, "ctPN: %d, ctN: %d, ctP: %d, ctC: %d", gameData->contatori.contProiettiliN, gameData->contatori.contNemici, gameData->contatori.contProiettili, gameData->contatori.contCoccodrilli);

		mvprintw(3, 105, "--------------------------------------------|");

		mvprintw(4, 106, "pid pN");
		mvprintw(5, 106, "                                ");
		for (int i = 0; i < MAXNPROIETTILINEMICI; i++)
		{
			mvprintw(5, 106 + (i * 8), "%d:%d ", i, gameData->pids.pidProiettiliNemici[i]);
		}

		mvprintw(6, 105, "--------------------------------------------|");
		mvprintw(7, 106, "pid p");
		mvprintw(8, 106, "                                      ");
		for (int i = 0; i < MAXNPROIETTILI; i++)
		{
			mvprintw(8, 106 + (i * 8), "%d:%d ", i, gameData->pids.pidProiettili[i]);
		}
		mvprintw(9, 105, "--------------------------------------------|");

		mvprintw(10, 106, "pid coccodrilli");

		// pulizia righe pid coccodrilli
		mvprintw(11, 106, "                                            ");
		mvprintw(12, 106, "                                            ");
		mvprintw(13, 106, "                                            ");
		mvprintw(14, 106, "                                            ");
		mvprintw(15, 106, "                                            ");
		mvprintw(16, 106, "                                            ");
		mvprintw(17, 106, "                                            ");
		int off_riga = 0;
		int off_colonna = 0;

		for (int i = 0; i < MAXNCOCCODRILLI; i++)
		{

			if (i % 4 == 0)
			{
				off_riga++;
			}
			if (off_colonna >= 4)
			{
				off_colonna = 0;
			}
			mvprintw(11 + off_riga, 106 + (off_colonna * 8), "%d:%d ", i, gameData->pids.pidCoccodrilli[i]);
			off_colonna++;
		}
		mvprintw(24,106,"tempo di gioco: %d",gameData->gameInfo.tempo.milliseconds);
		mvprintw(25,106,"sec: %d",sec);
		mvprintw(26,106,"                                            ");
		mvprintw(27,106,"                                            ");
		mvprintw(26,106,"contatore pari: %d",contatore_pari);
		mvprintw(27,106,"contatore dispari: %d",contatore_dispari);

		mvprintw(29,106,"                      ");
		mvprintw(29,106,"passi c id 0: %d",gameData->controlloCoccodrilli[0].passi);
		
		
		refresh();
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
	refresh();
	usleep(30000000);
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
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 3:
			begin_row = FILA_TRE;
			break;
		case 4:
			begin_row = FILA_QUATTRO;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 5:
			begin_row = FILA_CINQUE;
			break;
		case 6:
			begin_row = FILA_SEI;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 7:
			begin_row = FILA_SETTE;
			break;
		case 8:
			begin_row = FILA_OTTO;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		default:
			break;
		}

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