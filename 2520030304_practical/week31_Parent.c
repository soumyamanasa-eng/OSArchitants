/* probelm statement 
   C Program using fork() to create a child process.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
	int num;
	pid_t pid;
	char str[20];
	printf("enter a number: ");
	scanf("%d", &num);
	
	pid = fork();
	if (pid<0){
	   printf("fork failed\n");
           exit(1);
         }
	if (pid == 0) {
	   sprintf(str, "%d" ,num);
	   execl("./factoral", "factorial", str, NULL);
	
	   perror("exec failed");
         }
	else {
	     wait(NULL);
	     
	     printf("\n--- Parent Process ---\n");
	     printf("Square of %d = %d\n", num, num * num);
	}
	return 0;
	}
