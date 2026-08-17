#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("Browser process started.\n");
    printf("Browser PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        // Child process: open PDF using external application
        printf("Child process created.\n");
        printf("Child PID: %d\n", getpid());
        printf("Opening PDF using xdg-open...\n");

        execlp("xdg-open", "xdg-open", "sample.pdf", (char *)NULL);

        perror("exec failed");
        exit(1);
    }
    else
    {
        // Parent process: browser remains running
        printf("Parent/browser process is still running.\n");
        printf("Child PID: %d\n", pid);

        sleep(10);

        printf("Browser process finished.\n");
        wait(NULL);
    }

    return 0;
}
