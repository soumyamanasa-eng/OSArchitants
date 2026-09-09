#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t producer, consumer;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    producer = fork();

    if (producer == -1) {
        perror("fork");
        exit(1);
    }

    if (producer == 0) {
        // Producer
        close(fd[0]);

        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("ls", "ls", "-l", NULL);

        perror("execlp ls");
        exit(1);
    }

    consumer = fork();

    if (consumer == -1) {
        perror("fork");
        exit(1);
    }

    if (consumer == 0) {
        // Consumer
        close(fd[1]);

        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execlp("grep", "grep", ".c", NULL);

        perror("execlp grep");
        exit(1);
    }

    // Parent
    close(fd[0]);
    close(fd[1]);

    waitpid(producer, NULL, 0);
    waitpid(consumer, NULL, 0);

    return 0;
}
