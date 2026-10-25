/*
Name : 21_client.c
Author : Dabhi Rushirajsinh
Description : Write a program (client side) to communicate between two machines
              using sockets (TCP). Connects to the server on 127.0.0.1:8080.
              Type 'exit' to end the session.
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
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) { perror("socket"); exit(1); }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server!\n");

    while (1) {
        memset(buffer, 0, MAX);
        printf("Client: ");
        fgets(buffer, MAX, stdin);
        send(sock, buffer, strlen(buffer), 0);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Client exited the chat.\n");
            break;
        }

        memset(buffer, 0, MAX);
        read(sock, buffer, MAX);
        printf("Server: %s", buffer);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server exited the chat.\n");
            break;
        }
    }

    close(sock);

    return 0;
}

/*
Sample Output :

# Terminal 2 (run after server is started):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/21$ ./33_client
Connected to server!
Client: Hello, I am the client!
Server: Hello, I am the server!
Client: exit
Client exited the chat.

*/
