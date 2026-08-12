#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	int pid, num;
	printf("enter num\n");
	scanf("%d", &num);
	pid = fork();
	if(pid<0) {
		printf("fail to create new process\n");
		exit(0);
	     }
	else if(pid == 0){
		unsigned int fact = 1;
		int temp = num;
		while (temp>0) {
			fact = fact * temp;
			temp --;
			}
			printf("factorial of %d is %d", num,fact);
		    }
		else {
			int sqr;
			sqr = num * num;
			printf("dquare of %d is %d\n",num,sqr);
		     }
		return 0;
	}
