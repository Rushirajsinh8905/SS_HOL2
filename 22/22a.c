/*
Name : 22a.c
Author : Dabhi Rushirajsinh
Description : Write a program to create a concurrent server using fork().
              For each client connection, the server forks a child process to
              handle it, allowing multiple clients simultaneously.
              Test with: nc localhost 8080
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080

/* Reap zombie children */
void sigchld_handler(int s)
{
    (void)s;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_client(int client_sock)
{
    char buffer[1024];
    int n;
    while ((n = read(client_sock, buffer, sizeof(buffer))) > 0) {
        write(client_sock, buffer, n);   /* Echo back to client */
    }
    close(client_sock);
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr;
    struct sigaction sa;

    /* Avoid zombie processes */
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    printf("Concurrent server (fork) listening on port %d...\n", PORT);
    printf("Test with: nc localhost %d\n", PORT);

    while (1) {
        client_sock = accept(server_sock, NULL, NULL);
        if (client_sock == -1) continue;

        if (fork() == 0) {
            /* Child: handle this client */
            close(server_sock);
            handle_client(client_sock);
            exit(0);
        }

        /* Parent: close client socket and accept next connection */
        close(client_sock);
    }

    close(server_sock);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ gcc 34a.c -o 34a

# Run server in background
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ ./34a &
Concurrent server (fork) listening on port 8080...
Test with: nc localhost 8080

# Connect client 1 (in another terminal)
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ nc localhost 8080
Hello
Hello
How are you?
How are you?

# Check forked processes
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ ps -ef | grep 34a
  501  1241  1233   0 ./34a
  501  1242  1241   0 ./34a  (child for client 1)

# Stop server
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/22$ kill 1241

*/
