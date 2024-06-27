#include "disegna.h"

// ultimo tocco: 19/02/2024 13:47

int main() {

    // inizializzazione della libreria ncurses + inizializzazione seme per random
	inizializzaNcurses(); 
    
    GameData* gameData = (GameData*)malloc(sizeof(GameData));
    PipeData* buffer = (PipeData*) malloc(sizeof(PipeData)); // per comunicazione tra threads
    Semaphore* semafori = (Semaphore*) malloc(sizeof(Semaphore)); // semafori per la sincronizzazione tra threads
    creaSemafori(semafori);
    
    //avviaLogo();   // visualizza il logo ed aspetta che l'utente prema un tasto qualsiasi
    //int scelta = avviaMenuIniziale(); // fa partire il processo per il menu iniziale, aspetta che termini e poi prosegue

    // inizializza parametri per thread disegna
    ParamsDisegna draw_arg = {semafori, buffer, gameData};
    pthread_t drawTid = avviaDrawThread(&draw_arg);
    
    int err = pthread_join(drawTid, NULL);
    if(err){
        perror("ERR: Join su Disegna Fallita!!");
        strerror(err);
        return err;
    }else{
        printf("\nMain Thread Exit Normally");
    }

    refresh();

    distruggiSemafori(semafori);
    free(gameData);
    free(buffer);
    free(semafori);

    endwin(); // Termina ncurses
    printf("\n FROGGER: CLOSED \n\n");
    return 0;
}