#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_C2S "client_to_server"
#define FIFO_S2C "server_to_client"

int main() {
    int fd_write, fd_read;
    char buffer[100];

    printf("Client started.\n");

    fd_write = open(FIFO_C2S, O_WRONLY);
    fd_read = open(FIFO_S2C, O_RDONLY);

    while (1) {
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(fd_write, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "exit") == 0)
            break;

        memset(buffer, 0, sizeof(buffer));

        read(fd_read, buffer, sizeof(buffer));

        printf("Server: %s\n", buffer);
    }

    close(fd_write);
    close(fd_read);

    return 0;
}
