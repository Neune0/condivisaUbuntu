#include "../hdr/coccodrillo.h"

pid_t avviaCoccodrillo(int *pipe_fd, PipeData* coccodrillo_init, int id)
{

    pid_t coccodrillo_pid = fork();

    if (coccodrillo_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (coccodrillo_pid == 0)
    {
        // Processo coccodrillo
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        
        coccodrillo(pipe_fd, coccodrillo_init, id);
        exit(0);
    }
    else
    {
        return coccodrillo_pid;
    }
}

void coccodrillo(int *pipe_fd, PipeData *coccodrillo_init, int id)
{
    PipeData coccodrillo;
    coccodrillo.x = coccodrillo_init->x; // le coordinate iniziali del coccodrillo sono quelle dell' oggetto che ha sparato
    coccodrillo.y = coccodrillo_init->y;
    coccodrillo.type = coccodrillo_init->type;
    coccodrillo.id = id;
    int dirX = 1;

    // Seme unico per ogni processo
    unsigned int seed = time(NULL) ^ (getpid() << 16); 

    // la direzione deve essere calcolata in base alla y iniziale
    switch (coccodrillo_init->y)
    {
    case FILA_UNO:
    case FILA_TRE:
    case FILA_CINQUE:
    case FILA_SETTE:
        // il coccodrillo si muove verso destra percio' la direzione e' 1
        dirX = 1;
        break;
    case FILA_DUE:
    case FILA_QUATTRO:
    case FILA_SEI:
    case FILA_OTTO:
        // il coccodrillo si muove verso sinistra percio' la direzione e' -1
        dirX = -1;
        break;
    default:
        break;
    }

    // numero randomico tra min e max compresi
        int randomico = generaRandom_r(0 , 200000*15,&seed);
        // piccola usleep
        usleep(randomico);
    
    while (1)
    {
        coccodrillo.x += dirX;
        write(pipe_fd[1], &coccodrillo, sizeof(PipeData));
        usleep(200000); // Aspetta un po' prima di generare nuove coordinate
    }
}

void uccidiCoccodrillo(pid_t *pids_coccodrilli, int id_coccodrillo)
{
    if ((id_coccodrillo != -1) && (pids_coccodrilli[id_coccodrillo] != 0))
    {
        kill(pids_coccodrilli[id_coccodrillo], SIGKILL);
        int err = waitpid(pids_coccodrilli[id_coccodrillo], NULL, 0);
        if (err == -1)
        {
            perror("Errore nella waitpid");
            exit(1);
        }
        pids_coccodrilli[id_coccodrillo] = 0;
    }
    return;
}
