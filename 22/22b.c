/*
Name : 22b.c
Author : Dabhi Rushirajsinh
Description : Write a program to create a concurrent server using pthread_create().
              For each client connection, a new POSIX thread handles it,
              allowing multiple clients simultaneously without forking.
              Test with: nc localhost 8080
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

#define PORT        8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg)
{
    int client_sock = *((int *)arg);
    free(arg);

    printf("Thread %lu: handling client socket %d\n",
           (unsigned long)pthread_self(), client_sock);

    char buffer[BUFFER_SIZE];
    int n;

    while ((n = read(client_sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        printf("Thread %lu: received '%s'\n", (unsigned long)pthread_self(), buffer);
        write(client_sock, buffer, n);   /* Echo back */
    }

    printf("Thread %lu: client disconnected.\n", (unsigned long)pthread_self());
    close(client_sock);

    return NULL;
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("Concurrent server (pthread) listening on port %d...\n", PORT);
    printf("Test with: nc localhost %d\n", PORT);

    while (1) {
        client_sock = accept(server_sock, NULL, NULL);
        if (client_sock == -1) continue;

        printf("Main thread: new client connected, socket=%d\n", client_sock);

        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);   /* Thread cleans up itself */
    }

    close(server_sock);

    return 0;
}

/*
Sample Output :

# Compile (link pthread)
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ gcc 34b.c -o 34b -lpthread

# Run server
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ ./34b &
Concurrent server (pthread) listening on port 8080...
Test with: nc localhost 8080

# Connect client
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ nc localhost 8080
Hello
Main thread: new client connected, socket=4
Thread 140123456789504: handling client socket 4
Thread 140123456789504: received 'Hello'
Hello

# Stop server
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ kill %1

*/
