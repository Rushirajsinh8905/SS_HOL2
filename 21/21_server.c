/*
Name : 21_server.c
Author : Dabhi Rushirajsinh
Description : Write a program (server side) to communicate between two machines
              using sockets (TCP). The server listens on port 8080 and engages
              in a chat with the connected client. Type 'exit' to end the session.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX  1024

int main()
{
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX];

    /* Create TCP socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        exit(1);
    }

    listen(server_fd, 3);
    printf("Server listening on port %d...\n", PORT);

    client_sock = accept(server_fd, (struct sockaddr *)&address,
                         (socklen_t *)&addrlen);
    if (client_sock == -1) { perror("accept"); exit(1); }

    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, MAX);
        read(client_sock, buffer, MAX);
        printf("Client: %s", buffer);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Client exited the chat.\n");
            break;
        }

        memset(buffer, 0, MAX);
        printf("Server: ");
        fgets(buffer, MAX, stdin);
        send(client_sock, buffer, strlen(buffer), 0);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server exited the chat.\n");
            break;
        }
    }

    close(client_sock);
    close(server_fd);

    return 0;
}

/*
Sample Output :

# Compile both programs
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/21$ gcc 33_server.c -o 33_server
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/21$ gcc 33_client.c -o 33_client

# Terminal 1 (Server):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/21$ ./33_server
Server listening on port 8080...
Client connected!
Client: Hello, I am the client!
Server: Hello, I am the server!
Client: exit
Client exited the chat.

# Terminal 2 (Client):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/21$ ./33_client
Connected to server!
Client: Hello, I am the client!
Server: Hello, I am the server!
Client: exit
Client exited the chat.

*/
