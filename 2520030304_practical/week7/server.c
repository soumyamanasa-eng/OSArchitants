#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

#define FIFO_C2S "client_to_server"
#define FIFO_S2C "server_to_client"

void handle_signal(int sig) {
    printf("\nServer shutting down...\n");

    unlink(FIFO_C2S);
    unlink(FIFO_S2C);

    exit(0);
}

int main() {
    int fd_read, fd_write;
    char buffer[100];

    signal(SIGINT, handle_signal);

    mkfifo(FIFO_C2S, 0666);
    mkfifo(FIFO_S2C, 0666);

    printf("Server started. Waiting for client...\n");

    fd_read = open(FIFO_C2S, O_RDONLY);
    fd_write = open(FIFO_S2C, O_WRONLY);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        read(fd_read, buffer, sizeof(buffer));

        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;

        printf("Server response: ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(fd_write, buffer, strlen(buffer) + 1);
    }

    close(fd_read);
    close(fd_write);

    unlink(FIFO_C2S);
    unlink(FIFO_S2C);

    return 0;
}
