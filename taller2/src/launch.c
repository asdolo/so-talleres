#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		if(ptrace(PTRACE_TRACEME,0,NULL,NULL)){
			perror("ERROR child ptrace(PTRACE_TRACEME,...)");
			exit(1);
		}
		/* S'olo se ejecuta en el Hijo */
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* S'olo se ejecuta en el Padre */

			//ptrace(PTRACE_ATTACH,child);

		while(1) {
					
						ptrace(PTRACE_CONT,child,NULL,NULL);

			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */	
			printf("HUBO UNA SENAL\n");
			
			printf("Se ha hecho justicia\n");
			if (ptrace(PTRACE_KILL,child,NULL,NULL) == -1) { perror("ERROR ptrace KILL");}
			break;
		}
	}

	return 0;
}
