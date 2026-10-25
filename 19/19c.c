/*
Name : 19c.c
Author : Dabhi Rushirajsinh
Description : Write a program to protect multiple pseudo resources (two) using
              a counting semaphore. Counting semaphore value = 2 allows two
              processes to use resources simultaneously; a third must wait.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#ifndef __APPLE__
union semun { int val; struct semid_ds *buf; unsigned short *array; };
#endif

void sem_p(int semid) { struct sembuf op = {0, -1, 0}; semop(semid, &op, 1); }
void sem_v(int semid) { struct sembuf op = {0, +1, 0}; semop(semid, &op, 1); }

int main()
{
    int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    union semun arg;
    arg.val = 2;   /* 2 pseudo-resources available */
    semctl(semid, 0, SETVAL, arg);

    printf("Counting semaphore initialized. Value = 2 (two resources available).\n");

    int i;
    for (i = 1; i <= 4; i++) {
        if (fork() == 0) {
            sem_p(semid);
            printf("Process %d (PID=%d): Acquired resource. Using it...\n", i, getpid());
            sleep(2);
            printf("Process %d (PID=%d): Released resource.\n", i, getpid());
            sem_v(semid);
            exit(0);
        }
    }

    for (i = 0; i < 4; i++) wait(NULL);

    semctl(semid, 0, IPC_RMID);
    printf("All done. Semaphore removed.\n");

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/19$ gcc 31c.c -o 31c

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/19$ ./31c
Counting semaphore initialized. Value = 2 (two resources available).
Process 1 (PID=1241): Acquired resource. Using it...
Process 2 (PID=1242): Acquired resource. Using it...
(Processes 3 and 4 block here — semaphore = 0, both resources busy)
Process 1 (PID=1241): Released resource.
Process 3 (PID=1243): Acquired resource. Using it...
Process 2 (PID=1242): Released resource.
Process 4 (PID=1244): Acquired resource. Using it...
Process 3 (PID=1243): Released resource.
Process 4 (PID=1244): Released resource.
All done. Semaphore removed.

*/
