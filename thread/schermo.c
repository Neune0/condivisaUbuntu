#include "schermo.h"

void aggiorna(GameData *gameData, Semaphore *semafori, PipeData *buffer)
{	
	// individuazione della possibile collisione
	Collisione collisione = detectCollisione(gameData);

	if (collisione.tipoCollisione == NO_COLLISIONE)
	{
		// aggiornamento normale se no collisione
		normalUpdate(gameData, semafori, buffer);
	}
	else
	{
		// se collisione aggiornamento particolareggiato
		handleCollisione(gameData, collisione, semafori);
	}
	// qui aggiornamento del tempo(calcolo)
	aggiornaTempo(gameData);

	return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{

		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);

		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);

		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}

void stampaSpriteInMatrice(PipeData *datiNuovi, Sprite *sprite, GameData *gameData)
{
	int startRow = datiNuovi->y;
	int startCol = datiNuovi->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	int row = 0, col = 0;

	Schermo *schermo = &(gameData->schermo);
	PipeData *pipeData = &(gameData->pipeData);

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
			schermo->screenMatrix[row][col].color = sprite->color;
			schermo->screenMatrix[row][col].is_changed = true;
			schermo->screenMatrix[row][col].id = pipeData->id;
			schermo->screenMatrix[row][col].tipo = sprite->tipo; // nuova
		}
	}
	return;
}

void pulisciSpriteInMatrice(PipeData *datiVecchi, Sprite *sprite, GameData *gameData)
{
	int row = datiVecchi->y;
	int col = datiVecchi->x;
	int maxRows = sprite->max_row; // 2
	int maxCols = sprite->max_col; // 3

	Schermo *schermo = &(gameData->schermo);

	if (row != -1)
	{
		for (int i = row; i < row + maxRows; i++)
		{
			for (int j = col; j < col + maxCols; j++)
			{

				schermo->screenMatrix[i][j].ch = schermo->staticScreenMatrix[i][j].ch;
				schermo->screenMatrix[i][j].color = schermo->staticScreenMatrix[i][j].color;
				schermo->screenMatrix[i][j].is_changed = true;
				schermo->screenMatrix[i][j].tipo = schermo->staticScreenMatrix[i][j].tipo;
				schermo->screenMatrix[i][j].id = schermo->staticScreenMatrix[i][j].id;
			}
		}
	}
}

//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE (è stato modificato)
			if (screenMatrix[i][j].is_changed)
			{
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				// move(i,j);
				// addch(screenMatrix[i][j].ch);
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
				fflush(stdout);
				fflush(stdin);
			}
		}
	}
	return;
}

void stampaRefreshMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	clear();
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			fflush(stdout);
			fflush(stdin);
		}
	}
	refresh();
	return;
}

// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedente
	int id = gameData->pipeData.id;
	if (id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		datiVecchi[id].type = ' ';
		datiVecchi[id].x = 0;
		datiVecchi[id].y = 0;
	}
	return;
}

void stampaTanaChiusa(Tana tana, GameData *gameData)
{
	int y = tana.info.y;
	int x = tana.info.x;
	// sprite da disegnare

	int startRow = y;
	int startCol = x;
	int maxRows = gameData->sprites[8].max_row;
	int maxCols = gameData->sprites[8].max_col;
	int row = 0, col = 0;

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			gameData->schermo.screenMatrix[row][col].tipo = TANA_CLOSE_OBJ;
			gameData->schermo.screenMatrix[row][col].ch = gameData->sprites[8].sprite[i][j];
			gameData->schermo.screenMatrix[row][col].color = gameData->sprites[8].color;
			gameData->schermo.screenMatrix[row][col].is_changed = true;
			gameData->schermo.screenMatrix[row][col].id = tana.info.id;

			gameData->schermo.staticScreenMatrix[row][col].tipo = TANA_CLOSE_OBJ;
			gameData->schermo.staticScreenMatrix[row][col].ch = gameData->sprites[8].sprite[i][j];
			gameData->schermo.staticScreenMatrix[row][col].color = gameData->sprites[8].color;

			gameData->schermo.staticScreenMatrix[row][col].id = tana.info.id;
		}
	}
	gameData->gameInfo.manche++;
}

void aggiornaTempo(GameData *gameData)
{
	gameData->gameInfo.tempo.current = time(NULL);
	gameData->gameInfo.tempo.secondi = difftime(gameData->gameInfo.tempo.current, gameData->gameInfo.tempo.start);
	return;
}

void normalUpdate(GameData *gameData, Semaphore *semafori, PipeData *buffer)
{
	// stampe di debug per puntatori
	mvprintw(8,106,"Da Normal update:");
	mvprintw(9,106,"                                      ");
	mvprintw(9,106,"punt gameData: %p",gameData);
	mvprintw(10,106,"                                      ");
	mvprintw(10,106,"punt semafori: %p",semafori);
	mvprintw(11,106,"                                      ");
	mvprintw(11,106,"punt buffer: %p",buffer);
	switch (gameData->pipeData.type)
	{
	case 'X': // movimento rana
		// controlli su spostamento rana
		int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
		int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
		// se lo spostamento è lecito
		if (isFrogMoveLecit(newPosAbsRanaX, newPosAbsRanaY))
		{
			gameData->pipeData.x = newPosAbsRanaX;
			gameData->pipeData.y = newPosAbsRanaY;
			// normale aggiornamento
			aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
			gameData->ranaAbsPos.x = gameData->pipeData.x;
			gameData->ranaAbsPos.y = gameData->pipeData.y;
		}
		break;
	case 'S': // sparo proiettile da rana
		// se si hanno ancora munizioni
		if (gameData->contatori.contProiettili < MAXNPROIETTILI)
		{
			// ricerca di un indice disponibile
			int id = id_disponibile(gameData->allTCB.tcb_proiettili, MAXNPROIETTILI);

			if (id != -1)
			{
				// inizializzazione dati proiettile
				//PipeData init = {gameData->ranaAbsPos.x + 1, gameData->ranaAbsPos.y - 1, 'P', id, 0};
				PipeData init = {30, 30, 'P', id, 0}; // per test
				// parametri per il thread proiettile
				ParamsOggetti parametri = {semafori, buffer, &(gameData->allTCB.tcb_proiettili[id]), &init};

				// avvio del proiettile
				gameData->allTCB.tcb_proiettili[id].thread_id = avviaProiettileThread(&parametri);

				// incremento del contatore dei proiettili in gioco
				gameData->contatori.contProiettili++;
			}
		}
		break;
	case 'P': // nuove coordinate proiettile
		// se il proiettile ha sforato
		if (gameData->pipeData.y == FULLTANAROWEND)
		{
			// devo targettizzare il proiettile affinche termini
			int err = impostaThreadTarget(&(gameData->allTCB.tcb_proiettili[gameData->pipeData.id]), semafori->tcb_mutex);
		}
		else
		{
			aggiornaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
		}
		break;
	case 'Y': // proiettile terminato o in procinto di terminare
		// join sul thread proiettile
		if (pthread_join(gameData->pipeData.thread_id, NULL) == 0)
		{ // la join è riuscita
			// reset del tcb del proiettile
			gameData->allTCB.tcb_proiettili[gameData->pipeData.id].thread_id = 0;
			gameData->allTCB.tcb_proiettili[gameData->pipeData.id].is_target = false;
			gameData->allTCB.tcb_proiettili[gameData->pipeData.id].is_terminated = false;

			// decremento del contatore dei proiettili in gioco
			gameData->contatori.contProiettili--;
			// cancellazione del proiettile dalla matrice
			cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
		}
		else
		{
			perror("Join Thread Fallita");
		}
		break;
	case 'x': // rana terminata o in procinto di terminare
		// join sul thread rana
		if (pthread_join(gameData->allTCB.tcb_rana.thread_id, NULL) == 0)
		{ // la join è riuscita
			// reset del tcb della rana
			gameData->allTCB.tcb_rana.thread_id = 0;
			gameData->allTCB.tcb_rana.is_target = false;
			gameData->allTCB.tcb_rana.is_terminated = false;
			// cancellazione della rana dalla matrice
			cancellaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
			// decrementa le vite della rana
			gameData->gameInfo.vite--;
			// se le vite non sono finite allora si fa ripartire la rana
			if (gameData->gameInfo.vite > 0)
			{
				// inizializzazione dati rana
				PipeData init = {0, 0, 'X', 0, 0};
				// parametri per il thread rana
				ParamsOggetti arg_rana = {semafori, buffer, &(gameData->allTCB.tcb_rana), &init};
				// avvio della rana
				gameData->allTCB.tcb_rana.thread_id = avviaRanaThread(&arg_rana);
				// stampa della rana
				printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
			}
		}
		break;
	default:
		perror("tipo di oggetto sconosciuto in pipeData");
		break;
	}
	return;
}