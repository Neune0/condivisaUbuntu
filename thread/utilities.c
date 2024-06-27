#include "utilities.h"
#define SEMAPHORE

void scriviSuBuffer(ParamsOggetti *p, PipeData new_item, ThreadControlBlock *my_tcb, bool terminated_value){
	Semaphore *semafori = p->semafori;
    Semaphore semDeb = *semafori;
    /*	sez. di ingresso (nella sez.critica)	*/
	#if defined(SEMAPHORE)
    sem_wait( semafori->disponibili);
    sem_wait( semafori->mutex);
    #endif // SEMAPHORE
    
	/* 	sez. critica 	-	scrittura sul buffer */
    p->buffer->id = new_item.id;
    p->buffer->type = new_item.type;
    p->buffer->x = new_item.x;
    p->buffer->y = new_item.y;
    p->buffer->thread_id = new_item.thread_id;
    
	/*	sez. di uscita (dalla sez.critica)	*/
    #if defined(SEMAPHORE)
	sem_post( semafori->mutex);
	sem_post( semafori->presenti);
    #endif // SEMAPHORE
    
    // scrive sul tcb --- si può mettere in funzione separata
    sem_wait(semafori->tcb_mutex);
    my_tcb->is_terminated = terminated_value;
    sem_post(semafori->tcb_mutex);

	return;
}

void leggiDaBuffer(Semaphore *semafori,PipeData* buffer, PipeData *readed){
	
    /*	sez. di ingresso (nella sez.critica)	*/
    #if defined(SEMAPHORE)
    sem_wait(semafori->presenti);
    sem_wait(semafori->mutex);
    #endif
    /* sez. critica 	-	legge/rimuove elemento dal buffer */
    // se faccio cosi sto assegnando puntatori il che non è buono
    //*(readed) = *(buffer);
    // cosi faccio un assegnamento per valori
    readed->id= buffer->id;
    readed->type= buffer->type;
    readed->x= buffer->x;
    readed->y= buffer->y;

    /*	sez. di uscita (dalla sez.critica)	*/
    #if defined(SEMAPHORE)
    sem_post(semafori->mutex);
    sem_post(semafori->disponibili);
    #endif
    return;
}

void inizializzaNcurses() {
    srand(time(NULL));
    initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    keypad(stdscr, TRUE); // Abilita il keypad mode
    inizializzaColorazione();
    return;
}

void inizializzaColorazione(){
	start_color();
    init_pair(RANA_COL, COLOR_GREEN, COLOR_BLACK); // colore rana
    init_pair(SFONDO_COL,COLOR_WHITE,COLOR_BLACK); // colore sfondo
    init_pair(FIUME_COL,COLOR_WHITE,COLOR_BLUE); // colore fiume 
    init_pair(TANE_COL,COLOR_WHITE,COLOR_YELLOW); // colore tane
    init_pair(ARGINE_COL,COLOR_BLACK,COLOR_GREEN); // colore argine
    init_pair(MARCIAPIEDE_COL,COLOR_BLACK,COLOR_GRAY); // colore marciapiede
    init_pair(MENUBOX_COL,COLOR_GREEN,COLOR_BLACK); // colore box del menu
    init_pair(PROIETTILE_COL,COLOR_WHITE,COLOR_BLACK); // colore proiettile
    init_pair(BORDER_COL, COLOR_GREEN, COLOR_BLACK); // colore del bordo del gioco
    init_pair(LAVA_COL, COLOR_BLACK,COLOR_RED); // colore lava
    init_pair(LVL_COL, COLOR_BLACK,COLOR_RED); // colore hud livello
    init_pair(TEMPO_COL, COLOR_GREEN, COLOR_GREEN); // colore hud tempo
    return;
}

/*  ----    FUNZ PER SEMAFORI   ----    */
void creaSemafori(Semaphore* allSem){
    // allocazione dei semafori
    allSem->mutex = malloc(sizeof(sem_t));
    allSem->presenti = malloc(sizeof(sem_t));
    allSem->disponibili = malloc(sizeof(sem_t));
    allSem->tcb_mutex = malloc(sizeof(sem_t));

    if (sem_init(allSem->mutex,0,1) != 0){ perror("Inizializzazione Semaforo mutex fallita"); exit(-1); }
    
    if (sem_init(allSem->presenti,0,0) != 0){ perror("Inizializzazione Semaforo presenti fallita"); exit(-1);}
    
    if (sem_init(allSem->disponibili,0,1) != 0) { perror("Inizializzazione Semaforo disponibili fallita"); exit(-1); }

    if (sem_init(allSem->tcb_mutex,0,1) != 0) { perror("Inizializzazione Semaforo tcb_mutex fallita"); exit(-1); }
    return;
}

void distruggiSemafori(Semaphore* allSem){
    if (sem_destroy(allSem->mutex) != 0){ perror("Distruzione Semaforo mutex fallita"); }
    
    if (sem_destroy(allSem->presenti) != 0){ perror("Distruzione Semaforo presenti fallita");  }
    
    if (sem_destroy(allSem->disponibili) != 0) { perror("Distruzione Semaforo disponibili fallita"); }

    if (sem_destroy(allSem->tcb_mutex) != 0) { perror("Inizializzazione Semaforo tcb_mutex fallita"); exit(-1); }
    // free dei semafori
    free(allSem->mutex);
    free(allSem->presenti);
    free(allSem->disponibili);
    free(allSem->tcb_mutex);
    return;
}

/*  ----    FUNZIONE PER TARGETTIZARE UN THREAD     ----  */
int impostaThreadTarget(ThreadControlBlock* thread_tcb,sem_t *semaforo){
    if(sem_wait(semaforo)!= 0){ return -1;}
    thread_tcb->is_target = true;
    if(sem_post(semaforo)!= 0){ return -1;}

    return 0;
}

// funzione usata per trovare l'indice del primo tcb libero
int id_disponibile(ThreadControlBlock* tcbs, int lunghezza){
	for(int i=0;i<lunghezza;i++){
		if(tcbs[i].thread_id==0)return i;
	}
	return -1;
}

void aggiornaOldPos(PipeData *dati_vecchi,PipeData *dati_nuovi)
{
    dati_vecchi->x=dati_nuovi->x;
    dati_vecchi->y=dati_nuovi->y;
    dati_vecchi->type=dati_nuovi->type;
    dati_vecchi->id=dati_nuovi->id;
    dati_vecchi->thread_id=dati_nuovi->thread_id;
    return;
}

bool isGameOver(GameData* gameData){
    return gameData->gameInfo.vite==0 || gameData->gameInfo.manche==3;
}

bool isWin(GameData* gameData){
    return gameData->gameInfo.manche==3;
}

bool isFrogMoveLecit(int newX, int newY){
	return newX <= LASTGAMECOL - RANA_W + 1 && newX >= FIRSTGAMECOL && newY>FIRSTGAMEROW + RANA_H && newY < LASTGAMEROW;
}

void stampaBox(int option){
    int delay=0;
    if(option==1){delay=SUPERFAST;}

    attron(COLOR_PAIR(BORDER_COL));
    // stampa il bordo superiore
    for(int col = BORDERCOLSX; col <= BORDERCOLDX; col++){
        mvaddch(BORDERROWSUP,col,'='); // parte superiore
        refresh();
        usleep(delay);
    }
    // stampa il bordo destro
    for(int row = BORDERROWSUP + 1; row<= BORDERROWINF - 1; row++){
        mvaddch(row,BORDERCOLDX,'|'); // parte destra
        refresh();
        usleep(delay);
    }
    // stampa il bordo inferiore da destra a sinistra
    for(int col = BORDERCOLDX; col >= BORDERCOLSX; col--){
        mvaddch(BORDERROWINF,col,'='); // parte superiore
        refresh();
        usleep(delay);
    }
    // stampa il bordo sinistro dal basso verso l'alto
    for(int row = BORDERROWINF - 1; row >= BORDERROWSUP + 1; row--){
        mvaddch(row,BORDERCOLSX,'|'); // parte destra
        refresh();
        usleep(delay);
    }
    attroff(COLOR_PAIR(BORDER_COL));
    return;
}




// BLOCCO COMMENTATO PERCHE' NON UTILIZZATO
/*    ----      FUNZ PER TCB        ----       
bool isThreadValid(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    
    sem_wait(semaforo);
    if(thread_tcb != NULL){
        value = true;
    }else{
        //perror("TCB NULLO");
    }
    sem_post(semaforo);
    return value;
}

bool isThreadTerminated(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    sem_wait(semaforo);
    value = thread_tcb->is_terminated;
    sem_post(semaforo); 
    return value;
}

int resetTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    ThreadControlBlock tcbStandard = {0,false,false};
    if(sem_wait(semaforo) != 0){ return -1;};
    *(thread_tcb) = tcbStandard;
    if(sem_post(semaforo)!=0){return -1;}
    return 0;
}

bool isThreadTarget(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    sem_wait(semaforo);
    value = thread_tcb->is_target;
    sem_post(semaforo); 
    return value;
}

int copiaTCB(ThreadControlBlock* tcb_dest, ThreadControlBlock tcb_source, sem_t* semaforo){
    
    if(sem_wait(semaforo)){return -1;}
    *(tcb_dest) = tcb_source;
    if(sem_post(semaforo)){return -1;}
    return 0;
}

int scriviSuTCB(ParamsOggetti *param, ThreadControlBlock* dst_tcb, ThreadControlBlock new_item){
    if(sem_wait(param->semafori->tcb_mutex)!= 0){ return -1;}
    *(dst_tcb) = new_item;
    if(sem_post(param->semafori->tcb_mutex) != 0){return -1;}
    return 0;
}

pthread_t leggiTid(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    pthread_t tid = 0;
    sem_wait(semaforo);
    tid = thread_tcb->thread_id;
    sem_post(semaforo); 
    return tid;
}

ThreadControlBlock leggiTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    ThreadControlBlock letto;
    sem_wait(semaforo);
    letto = *thread_tcb;
    sem_post(semaforo);
    return letto;
}

int joinThreadMorto(ThreadControlBlock* deadThread, sem_t* semaforo){
    
    if(isThreadValid(deadThread, semaforo)){
    }else{ }//return -1;}
    
    if(isThreadTerminated(deadThread, semaforo)){
        //perror("TCB Terminato");
        if(pthread_join(deadThread->thread_id, NULL) == 0 ){    // la join è riuscita
            resetTCB(deadThread, semaforo);
            return 0;
        }else{
            perror("Join Thread Fallita");
        }
        return 0;
        //beep();
    }
    
    return -1; // return pthread_t thread_id;
}

int pulisciThreadMorti(Semaphore* allSem,  AllTCB* vettoreAllTCB){
    
    // NOTA: Tutte le funzioni interne usano semaforo mutex
    
    ThreadControlBlock localTCB;
    ThreadControlBlock *ptrTCB = NULL;

    //  RANA    
    bool flag = false;
    //sem_wait(&allSem->tcb_mutex);
    ptrTCB = &(vettoreAllTCB->tcb_rana);
    //if(ptrTCB->is_target){perror("la RANA è TARGET ");}     //per DEBUG
    //if(ptrTCB->is_terminated){perror("la RANA è MORTA ");}
    if(isThreadTarget(ptrTCB, allSem->tcb_mutex)){
        if(joinThreadMorto(ptrTCB, allSem->tcb_mutex) == 0){       // la RANA è terminata
            beep();
            return 0;
        }else{
            perror("Rana Join Missed!");
        }
    }
    
    
    //  PROIETTILI  
    
    for(int i=0; i<MAXNPROIETTILI; i++){                // scorre il vettore di tcb_proiettili
        ptrTCB = &(vettoreAllTCB->tcb_proiettili[i]);   // seleziona il tcb del singolo proiettile 
        if(joinThreadMorto(ptrTCB, allSem->tcb_mutex) == 0){           // la join è riuscita
                // tutto ok
            return 1;  // OR return tcb
        };
    }
    
    
    return -1;
}

// Cercun TCB in array per tid, ritorna TCB* o NULL
ThreadControlBlock* cercaThreadByTid(ThreadControlBlock* array_tcb, pthread_t thread_id, sem_t* semaforo, int dimVettore){
    
    ThreadControlBlock *ptrTCB = NULL;
    bool found=false;
    for (int i = 0; i < dimVettore; i++)    // scorre il vettore
    {
        sem_wait(semaforo);
        if(array_tcb[i].thread_id == thread_id){    // confronta i thread_id
            ptrTCB = &array_tcb[i];                 // salva l'indirizzo 
            found = true;
        }
        sem_post(semaforo);
        if(found) break;            // se trova qualcosa ferma il ciclo
    }
    return ptrTCB;
}

ThreadControlBlock* recuperaThreadTCB(ThreadControlBlock* array_tcb, pthread_t thread_id, int dimVettore){
    ThreadControlBlock* thread_tcb = NULL;
    for(int i=0; i<dimVettore; i++)
    {
        if(array_tcb[i].thread_id == thread_id){
            thread_tcb = &(array_tcb[i]);
        }
    }
    return thread_tcb;
}



*/
