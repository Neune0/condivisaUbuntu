#include "../hdr/disegna.h"

int main()
{
    int pipe_fd[2];       // Pipe file descriptors
    creaPipe(pipe_fd);    // Crea la pipe
                          
    inizializzaNcurses(); // inizializzazione della libreria ncurses + inizializzazione seme per random

    //avviaLogo();   // visualizza il logo ed aspetta che l'utente prema un tasto qualsiasi

    //int scelta = avviaMenuIniziale(); // fa partire il processo per il menu iniziale, aspetta che termini e poi prosegue

    avviaDrawProcess(pipe_fd); // avvia il processo che gestisce la stampa a schermo

    // Chiudi le estremità della pipe
    // close(pipe_fd[0]);
    // close(pipe_fd[1]);

    // Aspetta che il processo figlio termini
    wait(NULL);

    endwin(); // Termina ncurses

    printf("\n FROGGER: CLOSED \n\n");

    return 0;
}