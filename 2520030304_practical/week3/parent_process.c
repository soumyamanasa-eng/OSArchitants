#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int num;
	pid_t pid;
	char str[20];
	
	printf("Enter a number: ");
	scanf("%d", num);

	pid = fork();

	if (pid < 0){
	    printf("Fork failed\n");
	    exit(1);
	}
	
	if (pid == 0) {
	    sprintf(str, "%d", num);
	    
	    excel("./factorial", "factorial" , str NULL);

	    perror("exec failed");
	
	    scanf("%d", &num);
	    pid = fork();
