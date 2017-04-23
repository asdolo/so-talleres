#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int cantidadLlamados = 5;
int termine =0;
void handlerSIGURG(){
	write(1,"ya va!\n",7);
	cantidadLlamados--;
}

void handlerSIGINT(){
	termine=1;
}

int main(int argc, char* argv[]) {

	int pid = fork();
	if (pid==0)
	{
		
		while(cantidadLlamados>0){
			signal(SIGURG,handlerSIGURG);//PORQUE HAY QUE METERLO ACA PARA QUE FUNCIONE?

		}
		kill(getppid(),SIGINT);
	}else{
		signal(SIGINT,handlerSIGINT);
		while(!termine)
		{
			sleep(1);
			write(1,"sup!\n",5);
			kill(pid,SIGURG);
		}

		
	}
















	return 0;
}
