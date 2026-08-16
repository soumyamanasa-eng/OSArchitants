#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
	pid_t pid;
	printf("Parent Process Started\n");
	printf("Parent PID : %d\n", getpid());
	printf("Parent PPID : %d\n", getppid());
	
	pid = fork();
	if(pid<0){
		printf("Fork failed!\n");
		return 1;
		}
	if(pid==0) {
		printf("----------Child Process----------\n");
		printf("CHild PID : %d\n", getpid());
		printf("Child PPID : %d\n", getppid());
	
		printf("\nChild is Running....\n");
		sleep(5);
			
		printf("Child resumes execution.\n");
		sleep(5);
		printf("Child Process Terminating..\n");
		exit(0);
	     }
	else{
		printf("------Parent Process------\n");
		printf("Parent PID : %d\n", getpid());
		printf("Child PID : %d\n", pid);
		printf("\nParent waiting for child..\n");
		wait(NULL);
		
		printf("\nCHild has terminated.\n");
		printf("Parent Process Exiting..\n");
	}
	return 0;
     }

