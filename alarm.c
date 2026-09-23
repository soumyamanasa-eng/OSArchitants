#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<time.h>

void alarm_handler(int sig)
{
	printf("=====================================\n");
	printf("           ALARAM! 🔔🔔\n");
	printf("=====================================\n");
	printf("        Your scheduled alarm is ON!\n");
	printf("        Please check the time.\n");
	printf("=====================================\n");
	for (int i = 0; i < 5; i++)
	{
    	system("paplay alarm.wav");
    	sleep(1);
	}

	   printf("\nAlarm finished.\n");
}

int main()
{
	printf("==========================\n");
	printf("      LINUX ALARM CLOCK    ");
	printf("\n==========================\n");

	int hour, minute;

	printf("\nEnter alarm time (HH:MM): ");
	scanf("%d:%d", &hour, &minute);
	if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        printf("Invalid time!\n");
        return 1;
    }

    time_t now = time(NULL);
    struct tm target = *localtime(&now);

    target.tm_hour = hour;
    target.tm_min = minute;
    target.tm_sec = 0;

    time_t target_time = mktime(&target);

    if (target_time <= now)
    {
        target.tm_mday++;
        target_time = mktime(&target);
    }

    int seconds = (int)difftime(target_time, now);

    printf("\nAlarm set for %02d:%02d\n", hour, minute);
    printf("Waiting for %d seconds...\n", seconds);

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Failed to create process.\n");
        return 1;
    }

    if (pid == 0)
    {
        printf("\n[Child Process]\n");
        printf("Alarm process started.\n");
        printf("Child PID: %d\n", getpid());

        signal(SIGALRM, alarm_handler);

        alarm(seconds);

        pause();

        return 0;
    }
    else
    {
        printf("\n[Parent Process]\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        printf("Parent is waiting for the alarm process...\n");

        waitpid(pid, NULL, 0);

        printf("\nParent: Alarm process finished.\n");
    }

    return 0;
}
