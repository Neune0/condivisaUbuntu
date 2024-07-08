#include "../hdr/nemico.h"
pid_t avviaNemico(int *pipe_fd, int id)
{
	pid_t nemico_pid = fork();
	if (nemico_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else
	{
		if (nemico_pid == 0)
		{
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			nemico(pipe_fd, id);
			exit(0);
		}
		else
		{
			return nemico_pid;
		}
	}
	return nemico_pid;
}

void nemico(int *pipe_fd, int id)
{ // l'id fa riferimento all'id del tronco su cui il processo nemico sta
	PipeData nemico;
	nemico.id = id;
	int initialX = 0;
	nemico.y = ARGINEROWSTART + 1;
	// switch su id in modo da asseganre coordinate iniziali
	switch (id)
	{
	case 0:
		initialX = PLANT0STARTCOL;
		break;
	case 1:
		initialX = PLANT1STARTCOL;
		break;
	case 2:
		initialX = PLANT2STARTCOL;
		break;
	case 3:
		initialX = PLANT3STARTCOL;
		break;

	default:
		break;
	}
	nemico.x = initialX;
	nemico.type = 'n';
	write(pipe_fd[1], &nemico, sizeof(PipeData)); // prima scrittura


	// cambio tipo d'ora in poi lancerà soltanto proiettili
	nemico.type = 's';
	int contatore = 0;
	while (1)
	{	
		// parte relativa al lancio dei proiettili
		if (contatore % 100 == 0)
		{
			if(nemico.x-initialX>=3){
				nemico.x=initialX;
			}
			if(nemico.x-initialX==1){
				nemico.y = ARGINEROWSTART + 2;
			}
			else{
				nemico.y= ARGINEROWSTART + 1;
			}
			write(pipe_fd[1], &nemico, sizeof(PipeData));
			nemico.x++;
		}
		contatore = (contatore + 1) % 50;
		usleep(20000); // Aspetta un po' prima di generare nuove coordinate
	}
}

void killNemico(pid_t pid_nemico)
{
	kill(pid_nemico, SIGKILL);
	waitpid(pid_nemico, NULL, 0);
	return;
}
