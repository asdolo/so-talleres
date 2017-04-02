#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>



int count=0;
int termine=0;

void handlerSigUrg(){
	printf("ya va!\n");
	count++;
}
void handlerSigInt(){
	termine=1;
}


int main(int argc, char* argv[]) {
	
	pid_t pid=fork();
	if(pid==0){
		//soy hijo
		signal(SIGURG,handlerSigUrg);
		while(count<5){

			}
		kill(getppid(),SIGINT);
	}else{
		//Porque siempre empieza el padre...
		signal(SIGINT,handlerSigInt);
		//porque signal siempre termina y dsp empieza el hijo..
		while(!termine){
			sleep(1);
			printf("sup!\n");
			kill(pid,SIGURG);
		}
		//wait();
	}
	

	
	return 0;
}



