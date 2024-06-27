#include "../hdr/schermo.h"

void aggiorna(GameData *gameData)
{
	
	// individuazione della possibile collisione
	Collisione collisione = detectCollisione(gameData);

	if (collisione.tipoCollisione == NO_COLLISIONE)
	{
		// aggiornamento normale se no collisione
		normalUpdate(gameData);
	}
	else
	{
		// se collisione aggiornamento particolareggiato
		handleCollisione(gameData, collisione);
	}
	
	// qui aggiornamento del tempo(calcolo)
	// aggiornaTempo(gameData);

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
		stampaMatrice(gameData->schermo.screenMatrix);
		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
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
			// controllo che col non sfori
			if(col<=LASTGAMECOL && col>=FIRSTGAMECOL){
				schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
				schermo->screenMatrix[row][col].color = sprite->color;
				schermo->screenMatrix[row][col].is_changed = true;
				schermo->screenMatrix[row][col].id = pipeData->id;
				schermo->screenMatrix[row][col].tipo = sprite->tipo;
			}
		}
	}
	return;
}

void pulisciSpriteInMatrice(PipeData *datiVecchi, Sprite *sprite, GameData *gameData)
{
	// nel caso del coccodrillo avro che maxRow 2 e macCol 9
	// la row non sfora mai percio posso anche non controllarla ma la col si

	int row = datiVecchi->y;
	int col = datiVecchi->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	Schermo *schermo = &(gameData->schermo);

	if (row != -1)
	{
		for (int i = row; i < row + maxRows; i++)
		{
			for (int j = col; j < col + maxCols; j++)
			{
				// controllo che j non sfori
				if(j<=LASTGAMECOL && j>=FIRSTGAMECOL){
					schermo->screenMatrix[i][j].is_changed = true;
					schermo->screenMatrix[i][j].ch = schermo->staticScreenMatrix[i][j].ch;
					schermo->screenMatrix[i][j].color = schermo->staticScreenMatrix[i][j].color;
					schermo->screenMatrix[i][j].tipo = schermo->staticScreenMatrix[i][j].tipo;
					schermo->screenMatrix[i][j].id = schermo->staticScreenMatrix[i][j].id;
				}
				
			}
		}
	}
}

//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	int contatore = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE (è stato modificato)
			if (screenMatrix[i][j].is_changed)
			{
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
				contatore++;
			}
		}
	}
	refresh();

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
		stampaMatrice(gameData->schermo.screenMatrix);
		datiVecchi->type = ' ';
		datiVecchi->x = 0;
		datiVecchi->y = 0;
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

void normalUpdate(GameData *gameData)
{
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
		// proiettile sparato da utente avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
		if (gameData->contatori.contProiettili < MAXNPROIETTILI) // se si hanno ancora munizioni
		{
			// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
			int id = id_disponibile(gameData->pids.pidProiettili, MAXNPROIETTILI);
			if (id != -1)
			{
				gameData->pids.pidProiettili[id] = avviaProiettile(gameData->pipe, &(gameData->ranaAbsPos), id);

				gameData->contatori.contProiettili++;
			}
		}
		break;
	case 's':															// sparo proiettile da pianta
		if (gameData->contatori.contProiettiliN < MAXNPROIETTILINEMICI) // se si hanno ancora munizioni
		{
			// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
			int id = id_disponibile(gameData->pids.pidProiettiliNemici, MAXNPROIETTILINEMICI);
			if (id != -1)
			{
				gameData->pids.pidProiettiliNemici[id] = avviaProiettileNemico(gameData->pipe, &(gameData->pipeData), id);

				gameData->contatori.contProiettiliN++;
			}
		}
		break;
	case 'P':
		// nuove coordinate proiettile se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
		if (gameData->pipeData.y < FULLTANAROWEND)
		{
			uccidiProiettile(gameData->pids.pidProiettili, gameData->pipeData.id); // uccide il processo proiettile
			// ucciso processo proiettile e impostato a zero il pid in array pid proiettili
			cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
			gameData->contatori.contProiettili--;
		}
		else
		{
			aggiornaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
		}
		break;
	case 'p': // nuove coord proiettile nemico
		if (gameData->pipeData.y + 1 == LASTGAMEROW)
		{
			uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
			gameData->contatori.contProiettiliN--;
		}
		else
		{

			aggiornaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
		}
		break;
	case 'n':
		// prima print del nemico
		printPianta(gameData, gameData->oldPos.nemici, S_PIANTA);
		break;
	case 'C':
		// controllo se il coccodrillo sfora come faccio a capire verso che direzione va?
		if (gameData->pipeData.x > LASTGAMECOL)
		{
			uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi=0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione=0;
			gameData->contatori.contCoccodrilli--;
		}
		else
		{
			aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
		}
		break;
	case 'c':
		// controllo che il coccodrillo non abbia sforato a sinistra
		if(gameData->pipeData.x<FIRSTGAMECOL - 9){
			uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi=0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione=0;
			gameData->contatori.contCoccodrilli--;
		}
		else{
			aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
		}
		break;
	case 'T':
		// arrivato tempo di gioco
		gameData->gameInfo.tempo.milliseconds = gameData->pipeData.x;
		
	default:
		break;
	}
	return;
}

void printPianta(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{
		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}

