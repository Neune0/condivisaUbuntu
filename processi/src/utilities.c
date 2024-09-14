#include "../hdr/utilities.h"

void inizializzaNcurses()
{
	srand(time(NULL));
	initscr();			   // Inizializza ncurses
	curs_set(FALSE);	   // Nasconde il cursore
	nodelay(stdscr, TRUE); // Abilita l'input non bloccante
	keypad(stdscr, TRUE);  // Abilita il keypad mode
	inizializzaColorazione();
	return;
}

void inizializzaColorazione()
{
	start_color();
	init_pair(RANA_COL, COLOR_GREEN, COLOR_BLACK);			   // colore rana
	init_pair(SFONDO_COL, COLOR_WHITE, COLOR_BLACK);		   // colore sfondo
	init_pair(FIUME_COL, COLOR_WHITE, COLOR_BLUE);			   // colore fiume
	init_pair(TANE_COL, COLOR_WHITE, COLOR_YELLOW);			   // colore tane
	init_pair(ARGINE_COL, COLOR_BLACK, COLOR_GREEN);		   // colore argine
	init_pair(MARCIAPIEDE_COL, COLOR_BLACK, COLOR_GRAY);	   // colore marciapiede
	init_pair(MENUBOX_COL, COLOR_GREEN, COLOR_BLACK);		   // colore box del menu
	init_pair(PROIETTILE_COL, COLOR_WHITE, COLOR_BLACK);	   // colore proiettile
	init_pair(BORDER_COL, COLOR_GREEN, COLOR_BLACK);		   // colore del bordo del gioco
	init_pair(LAVA_COL, COLOR_BLACK, COLOR_RED);			   // colore lava
	init_pair(LVL_COL, COLOR_BLACK, COLOR_RED);				   // colore hud livello
	init_pair(TEMPO_COL, COLOR_GREEN, COLOR_GREEN);			   // colore hud tempo
	init_pair(PIANTA_COL, COLOR_BLACK, COLOR_GREEN);		   // colore pianta
	init_pair(COCCODRILLOBUONO_COL, COLOR_GREEN, COLOR_WHITE); // colore coccodrillo
	init_pair(COCCODRILLOCATTIVO_COL, COLOR_RED, COLOR_BLACK); // colore coccodrillo cattivo
	init_pair(LAMPEGGIA, COLOR_GREEN, COLOR_WHITE);			   // colore coccodrillo che lampeggia
	return;
}

void creaPipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Pipe creation failed");
		exit(1);
	}
}

void uccidiProcesso(pid_t target_pid)
{
	kill(target_pid, SIGKILL);

	int err = waitpid(target_pid, NULL, 0);
	if (err == -1)
	{
		perror("Errore nella waitpid");
		exit(1);
	}

	return;
}

void targetUccidiProcesso(pid_t *array_pid, int id_processo)
{
	if ((id_processo != -1) && (array_pid[id_processo] != NOPID))
	{
		uccidiProcesso(array_pid[id_processo]);
		array_pid[id_processo] = NOPID;
	}
}

int id_disponibile(pid_t *array_pid, int lunghezza)
{
	for (int i = 0; i < lunghezza; i++)
	{
		if (array_pid[i] == NOPID)
			return i;
	}
	return NOID;
}

void aggiornaOldPos(PipeData *old_pos, PipeData *pipeData)
{
	setPipeData(old_pos, pipeData->type, pipeData->id, pipeData->x, pipeData->y);
	return;
}

bool isGameOver(GameData *gameData)
{
	return gameData->gameInfo.vite == 0 || gameData->gameInfo.manche == 6;
}

bool isWin(GameData *gameData)
{
	return gameData->gameInfo.manche == 6;
}

bool isFrogMoveLecit(int newX, int newY, RanaAbsPos ranaPos, PipeData pipeData)
{
	if (ranaPos.on_coccodrillo && pipeData.x != 0)
	{
		// la rana è su un coccodrillo e sto cercando di muovermi lungo il coccodrillo
		int new_offset = ranaPos.offset_on_coccodrillo + pipeData.x;
		if (new_offset <= 0 || new_offset > COCCODRILLO_W - RANA_W - 1)
		{
			return false;
		}
	}
	return newX <= LASTGAMECOL - RANA_W + 1 && newX >= FIRSTGAMECOL && newY > FIRSTGAMEROW + RANA_H && newY < LASTGAMEROW;
}

void stampaBox(int option)
{
	int delay = 0;
	if (option == 1)
	{
		delay = SUPERFAST;
	}

	attron(COLOR_PAIR(BORDER_COL));
	// stampa il bordo superiore
	for (int col = BORDERCOLSX; col <= BORDERCOLDX; col++)
	{
		mvaddch(BORDERROWSUP, col, '='); // parte superiore
		refresh();
		usleep(delay);
	}
	// stampa il bordo destro
	for (int row = BORDERROWSUP + 1; row <= BORDERROWINF - 1; row++)
	{
		mvaddch(row, BORDERCOLDX, '|'); // parte destra
		refresh();
		usleep(delay);
	}
	// stampa il bordo inferiore da destra a sinistra
	for (int col = BORDERCOLDX; col >= BORDERCOLSX; col--)
	{
		mvaddch(BORDERROWINF, col, '='); // parte superiore
		refresh();
		usleep(delay);
	}
	// stampa il bordo sinistro dal basso verso l'alto
	for (int row = BORDERROWINF - 1; row >= BORDERROWSUP + 1; row--)
	{
		mvaddch(row, BORDERCOLSX, '|'); // parte destra
		refresh();
		usleep(delay);
	}
	attroff(COLOR_PAIR(BORDER_COL));
	return;
}

int generaRandom_r(int min, int max, unsigned int *seed)
{
	return (rand_r(seed) % (max - min + 1)) + min;
}

int fromFilaToRow(int fila)
{
	int row = 0;
	switch (fila)
	{
	case 1:
		row = FILA_UNO;
		break;
	case 2:
		row = FILA_DUE;
		break;
	case 3:
		row = FILA_TRE;
		break;
	case 4:
		row = FILA_QUATTRO;
		break;
	case 5:
		row = FILA_CINQUE;
		break;
	case 6:
		row = FILA_SEI;
		break;
	case 7:
		row = FILA_SETTE;
		break;
	case 8:
		row = FILA_OTTO;
		break;
	default:
		break;
	}
	return row;
}

void setScreenCell(ScreenCell *screenCell, TipoObj tipo, char ch, int id, int color, bool changed)
{
	screenCell->ch = ch;
	screenCell->color = color;
	screenCell->tipo = tipo;
	screenCell->id = id;
	screenCell->is_changed = changed;
	return;
}

void setPipeData(PipeData *pipeData, char type, int id, int x, int y)
{
	pipeData->type = type;
	pipeData->id = id;
	pipeData->x = x;
	pipeData->y = y;
	return;
}

// DEBUG FUNCTIONS TODO chiudere in file a parte
void debugPrintContatori(int fila, GameData *gameData)
{
	Cont cont = gameData->contatori;
	mvprintw(fila, DEBUGINITCOLUMN, "                                            ");
	mvprintw(fila, DEBUGINITCOLUMN, "ctPN: %d, ctN: %d, ctP: %d, ctC: %d", cont.contProiettiliN, cont.contNemici, cont.contProiettili, cont.contCoccodrilli);
	return;
}

void debugPrintFlussi(int fila, GameData *gameData)
{
	Flusso *flussi = &(gameData->flussi);
	for (int i = 0; i < NUMFLUSSI; i++)
	{
		mvprintw(fila + i, DEBUGINITCOLUMN, "                                            ");
		mvprintw(fila + i, DEBUGINITCOLUMN, "%d| dir-> %d n_c_attivi->: %d vel->: %d", i, flussi[i].direction, flussi[i].n_coccodrilli_attivi, flussi[i].vel);
	}
	return;
}

void debugPrintControlloCoccodrilli(int fila, GameData *gameData)
{
	CocodrileControl *contrCocc = &(gameData->controlloCoccodrilli);
	for (int i = 0; i < 4; i++)
	{
		mvprintw(fila + i, DEBUGINITCOLUMN, "                                            ");
		mvprintw(fila + i, DEBUGINITCOLUMN, "%d| id-> %d passi->: %d", i, contrCocc[i].id, contrCocc[i].passi);
	}
	return;
}

void debugPrintPidNemici(int fila, GameData *gameData)
{
	pid_t *pids = gameData->pids.pidNemici;
	mvprintw(fila, DEBUGINITCOLUMN, "                                            ");
	mvprintw(fila, DEBUGINITCOLUMN, "Pid nemici: 0:%d 1:%d 2:%d 3:%d", (int)pids[0], (int)pids[1], (int)pids[2], (int)pids[3]);
	return;
}

void debugPrintOldPosNemici(int fila, GameData *gameData)
{
	PipeData *oldPosNemici = gameData->oldPos.nemici;
	for (int i = 0; i < MAXNNEMICI; i++)
	{
		mvprintw(fila + i, DEBUGINITCOLUMN, "                                            ");
		mvprintw(fila + i, DEBUGINITCOLUMN, "%d| x-> %d y->: %d id->: %d type-> %c", i, oldPosNemici[i].x, oldPosNemici[i].y, oldPosNemici[i].id, oldPosNemici[i].type);
	}
	return;
}

void debugPrintOldPosCoccodrilli(int fila, GameData *gameData)
{
	PipeData *coccodrilli = &(gameData->oldPos.coccodrilli);
	for (int i = 0; i < 4; i++)
	{
		mvprintw(fila + i, DEBUGINITCOLUMN, "                                            ");
		mvprintw(fila + i, DEBUGINITCOLUMN, "%d| x-> %d y-> %d id-> %d type-> %c", i, coccodrilli[i].x, coccodrilli[i].y, coccodrilli[i].id, coccodrilli[i].type);
	}
	return;
}

void debugPrintLastPipeData(GameData *gameData)
{
	switch (gameData->pipeData.type)
	{
	case 'X': // rana
		mvprintw(0, DEBUGINITCOLUMN, "                                            ");
		mvprintw(0, DEBUGINITCOLUMN, "Last X: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'C': // coccodrillo verso destra
		mvprintw(1, DEBUGINITCOLUMN, "                                            ");
		mvprintw(1, DEBUGINITCOLUMN, "Last C: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'c': // coccodrillo verso sinistra
		mvprintw(2, DEBUGINITCOLUMN, "                                            ");
		mvprintw(2, DEBUGINITCOLUMN, "Last c: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'n': // spawn nemico
		mvprintw(3, DEBUGINITCOLUMN, "                                            ");
		mvprintw(3, DEBUGINITCOLUMN, "Last n: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'p': // movimento proiettile nemico
		mvprintw(4, DEBUGINITCOLUMN, "                                            ");
		mvprintw(4, DEBUGINITCOLUMN, "Last p: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'P': // movimento proiettle
		mvprintw(5, DEBUGINITCOLUMN, "                                            ");
		mvprintw(5, DEBUGINITCOLUMN, "Last P: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'S': // proiettile sparato da rana
		mvprintw(6, DEBUGINITCOLUMN, "                                            ");
		mvprintw(6, DEBUGINITCOLUMN, "Last S: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 's': // proiettile sparato da nemico
		mvprintw(7, DEBUGINITCOLUMN, "                                            ");
		mvprintw(7, DEBUGINITCOLUMN, "Last s: id: %d x: %d y: %d", gameData->pipeData.id, gameData->pipeData.x, gameData->pipeData.y);
		break;
	case 'T': // tempo
		mvprintw(8, DEBUGINITCOLUMN, "                                            ");
		mvprintw(8, DEBUGINITCOLUMN, "Last T: sec: %d", gameData->pipeData.x);
		break;
	default:
		break;
	}

	return;
}

void debugPrintCollision(GameData *gameData, Collisione collisione)
{
	if (collisione.tipoCollisione != NO_COLLISIONE)
	{
		mvprintw(15, 106, "                                            ");
		mvprintw(16, 106, "                                            ");
		mvprintw(17, 106, "                                            ");
		mvprintw(18, 106, "                                            ");

		// print per debug di last tipo di collisione
		switch (collisione.tipoCollisione)
		{

		case RANA_FIUME:
			mvprintw(15, 106, "RANA-FIUME");
			break;
		case RANA_TANA_APERTA:
			mvprintw(15, 106, "RANA-tana aperta");
			break;
		case RANA_TANA_CHIUSA:
			mvprintw(15, 106, "rana tana chiusa");
			break;
		case PROIETTILE_COCCODRILLO_CATTIVO:
			mvprintw(15, 106, "proiettile coccodrillo cattivo");
			break;
		case RANA_COCCODRILLO_BUONO:
			mvprintw(15, 106, "rana coccodrillo buono");
			break;
		case PROIETTILE_NEMICO:
			mvprintw(15, 106, "proiettile- nemico");
			break;
		case PROIETTILENEMICO_RANA:
			mvprintw(15, 106, "RANA-proiettile nemico");
			break;
		case PROIETTILE_PROIETTILENEMICO:
			mvprintw(15, 106, "proiettile proiettile nemico");
			break;
		case NEMICO_RANA:
			mvprintw(15, 106, "RANA-nemico");
			break;
		case RANA_LAVA:
			mvprintw(15, 106, "RANA-lava");
			break;
		case RANA_COCCODRILLO_CATTIVO:
			mvprintw(15, 106, "RANA-coccodrillo cattivo");
			break;
		case RANA_MARCIAPIEDE:
			mvprintw(15, 106, "RANA-marciapiede");
			break;
		case RANA_ARGINE:
			mvprintw(15, 106, "RANA-argine");
			break;
		case PROIETTILE_COCCODRILLO_BUONO:
			mvprintw(15, 106, "proiettile-coccodrillo buono");
			break;
		default:
			break;
		}

		// print per debug di oggetto attivo di last collisione
		switch (collisione.oggetto_attivo)
		{
		case TANA_OPEN_OBJ:
			mvprintw(16, 106, "ATTIVO: Tana open: %d", collisione.id_oggetto_attivo);
			break;
		case TANA_CLOSE_OBJ:
			mvprintw(16, 106, "ATTIVO: Tana close: %d", collisione.id_oggetto_attivo);
			break;
		case FIUME_OBJ:
			mvprintw(16, 106, "ATTIVO: fiume: %d", collisione.id_oggetto_attivo);
			break;
		case ARGINE_OBJ:
			mvprintw(16, 106, "ATTIVO: argine: %d", collisione.id_oggetto_attivo);
			break;
		case MARCIAPIEDE_OBJ:
			mvprintw(16, 106, "ATTIVO: marciapiede: %d", collisione.id_oggetto_attivo);
			break;
		case COCCODRILLO_CATTIVO_OBJ:
			mvprintw(16, 106, "ATTIVO: cocc cattivo: %d", collisione.id_oggetto_attivo);
			break;
		case COCCODRILLO_BUONO_OBJ:
			mvprintw(16, 106, "ATTIVO: cocc buono: %d", collisione.id_oggetto_attivo);
			break;
		case PN_OBJ:
			mvprintw(16, 106, "ATTIVO: pro nemico: %d", collisione.id_oggetto_attivo);
			break;
		case N_OBJ:
			mvprintw(16, 106, "ATTIVO: nemico: %d", collisione.id_oggetto_attivo);
			break;
		case P_OBJ:
			mvprintw(16, 106, "ATTIVO: pro amico: %d", collisione.id_oggetto_attivo);
			break;
		case RANA_OBJ:
			mvprintw(16, 106, "ATTIVO: rana: %d", collisione.id_oggetto_attivo);
			break;
		case SFONDO_OBJ:
			mvprintw(16, 106, "ATTIVO: sfondo: %d", collisione.id_oggetto_attivo);
			break;
		case LAVA_OBJ:
			mvprintw(16, 106, "ATTIVO: lava: %d", collisione.id_oggetto_attivo);
			break;
		case HUD_OBJ:
			mvprintw(16, 106, "ATTIVO: hud: %d", collisione.id_oggetto_attivo);
			break;

		default:
			break;
		}

		// print per debug di oggetto passivo di last collisione
		switch (collisione.oggetto_passivo)
		{
		case TANA_OPEN_OBJ:
			mvprintw(17, 106, "PASSIVO: Tana open: %d", collisione.id_oggetto_passivo);
			break;
		case TANA_CLOSE_OBJ:
			mvprintw(17, 106, "PASSIVO: Tana close: %d", collisione.id_oggetto_passivo);
			break;
		case FIUME_OBJ:
			mvprintw(17, 106, "PASSIVO: fiume: %d", collisione.id_oggetto_passivo);
			break;
		case ARGINE_OBJ:
			mvprintw(17, 106, "PASSIVO: argine: %d", collisione.id_oggetto_passivo);
			break;
		case MARCIAPIEDE_OBJ:
			mvprintw(17, 106, "PASSIVO: marciapiede: %d", collisione.id_oggetto_passivo);
			break;
		case COCCODRILLO_CATTIVO_OBJ:
			mvprintw(17, 106, "PASSIVO: cocc cattivo: %d", collisione.id_oggetto_passivo);
			break;
		case COCCODRILLO_BUONO_OBJ:
			mvprintw(17, 106, "PASSIVO: cocc buono: %d", collisione.id_oggetto_passivo);
			break;
		case PN_OBJ:
			mvprintw(17, 106, "PASSIVO: pro nemico: %d", collisione.id_oggetto_passivo);
			break;
		case N_OBJ:
			mvprintw(17, 106, "PASSIVO: nemico: %d", collisione.id_oggetto_passivo);
			break;
		case P_OBJ:
			mvprintw(17, 106, "PASSIVO: pro amico: %d", collisione.id_oggetto_passivo);
			break;
		case RANA_OBJ:
			mvprintw(17, 106, "PASSIVO: rana: %d", collisione.id_oggetto_passivo);
			break;
		case SFONDO_OBJ:
			mvprintw(17, 106, "PASSIVO: sfondo: %d", collisione.id_oggetto_passivo);
			break;
		case LAVA_OBJ:
			mvprintw(17, 106, "PASSIVO: lava: %d", collisione.id_oggetto_passivo);
			break;
		case HUD_OBJ:
			mvprintw(17, 106, "PASSIVO: hud: %d", collisione.id_oggetto_passivo);
			break;

		default:
			break;
		}

		mvprintw(18, 106, "hit point x: %d y: %d", collisione.hit_point_x, collisione.hit_point_y);
	}
	return;
}

void debugPrintAllPids(GameData *gameData, int fila)
{
	mvprintw(fila, DEBUGINITCOLUMN, "pid rana: %d", (int)gameData->pids.pidRana);
	mvprintw(fila + 1, DEBUGINITCOLUMN, "pid tempo: %d", (int)gameData->pids.pidTempo);
	mvprintw(fila + 2, DEBUGINITCOLUMN, "pid musica: %d", (int)gameData->pids.pidMusica);
	mvprintw(fila + 3, DEBUGINITCOLUMN, "pid soundPlayer: %d", (int)gameData->pids.soundPlayer);
	int row = fila + 4;
	for (int i = 0; i < MAXNNEMICI; i++)
	{
		mvprintw(row + i, DEBUGINITCOLUMN, "pid nemici[%d]: %d", i, (int)gameData->pids.pidNemici[i]);
	}
	row += MAXNNEMICI;
	for (int i = 0; i < MAXNPROIETTILI; i++)
	{
		mvprintw(row + i, DEBUGINITCOLUMN, "pid proiettili[%d]: %d", i, (int)gameData->pids.pidProiettili[i]);
	}
	row += MAXNPROIETTILINEMICI;
	for (int i = 0; i < MAXNPROIETTILINEMICI; i++)
	{
		mvprintw(row + i, DEBUGINITCOLUMN, "pid proiettiliNemici[%d]: %d", i, (int)gameData->pids.pidProiettiliNemici[i]);
	}
	refresh();
	debugBlock();
	clearPortionScreen(0, 106, 30, 40);
	refresh();

	for (int i = 0; i < MAXNCOCCODRILLI; i++)
	{
		mvprintw(i, DEBUGINITCOLUMN, "pid coccodrilli[%d]: %d", i, (int)gameData->pids.pidCoccodrilli[i]);
	}
}

void debugBlock()
{
	nodelay(stdscr, FALSE); // riabilita l'input bloccante
	getch();				// Aspetta che l'utente prema un tasto qualsiasi

	nodelay(stdscr, TRUE); // riabilita l'input non bloccante
}

void clearPortionScreen(int startRow, int startCol, int altezza, int larghezza)
{
	for (int row = startRow; row < startRow + altezza; row++)
	{
		for (int col = startCol; col < startCol + larghezza; col++)
		{
			attron(COLOR_PAIR(SFONDO_COL));
			mvaddch(row, col, ' ');
			attroff(COLOR_PAIR(SFONDO_COL));
		}
	}
	refresh();
}

void debugPrintOldPosCoccodrilli2(int fila,GameData* gameData){
	PipeData* coccodrilli= gameData->oldPos.coccodrilli;
	for(int i=0;i<MAXNCOCCODRILLI;i++){
		mvprintw(fila+i,DEBUGINITCOLUMN,"oldPosCoc[%d]: ID %d X %d Y %d t %c ",i,coccodrilli[i].id,coccodrilli[i].x,coccodrilli[i].y,coccodrilli[i].type);
	}
	refresh();
}