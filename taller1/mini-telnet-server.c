#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int
main(int argc, char **argv)
{
	char buf[MSGLEN + 1];
	struct sockaddr_in local, remote;
	ssize_t r;
	int c, remotelen, s;

	/* crea socket */
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* configura el nombre del socket */
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);
	if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	/* linkea el socket con el nombre y crea la cola de mensajes de
	 * conexión; es decir, lo pone en modo "aceptar conexión" */
	if (listen(s, 10) == -1) {
		perror("listen");
		exit(1);
	}

	/* espera una nueva conexión */
	remotelen = sizeof(remote);
	if ((c = accept(s, (struct sockaddr *)&remote, (socklen_t *)&remotelen))
	    == -1) {
		perror("accept");
		exit(1);
	}

	/* atiende el cliente */
	for (;;) {
		r = recv(c, buf, sizeof(buf) - 1, 0);
		if (r == 0) {
			fputs("recv vacío\n", stderr);
			break;
		}
		if (r == -1) {
			perror("recv");
			continue;
		}

		/* transforma lo recibido en un string de C */
		buf[r] = '\0';
		
		printf("comando: %s", buf);

		int pid=fork();
		if (pid==0)
		{
			//El STDOUT de la ejecucion va al cliente 
			dup2(c,STDOUT_FILENO);
			//El STDERR de la ejecucion va al cliente 
			dup2(c,STDERR_FILENO);
			execl("/bin/sh", "sh", "-c", &buf, (char *) 0);
			
		}
		else{
			wait(NULL);
			if (send(c, "END\n", sizeof("END\n") - 1, 0) == -1) {
				perror("send");
				exit(1);
			}



		}
	/*	
		int stdoutPosta = dup(STDOUT_FILENO); 
		int stdErrPosta = dup(STDERR_FILENO);

		//El STDOUT de la ejecucion va al cliente 
		dup2(c,STDOUT_FILENO);
		//El STDERR de la ejecucion va al cliente 
		dup2(c,STDERR_FILENO);
		// ejecuta el comando 
		(void)system(buf);
		printf("END\n");

		dup2(stdoutPosta,STDOUT_FILENO);
		dup2(stdErrPosta,STDERR_FILENO);
		close(stdoutPosta);
		close(stdErrPosta);
		*/
	}


	/* cierra los sockets */
	close(c);
	close(s);

	return 0;
}
