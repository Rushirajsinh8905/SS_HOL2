/*
Name : 19b.c
Author : Dabhi Rushirajsinh
Description : Write a program to protect shared memory from concurrent write
              access using a binary semaphore (SysV IPC).
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#ifndef __APPLE__
union semun { int val; struct semid_ds *buf; unsigned short *array; };
#endif

void sem_p(int semid) { struct sembuf op = {0, -1, 0}; semop(semid, &op, 1); }
void sem_v(int semid) { struct sembuf op = {0, +1, 0}; semop(semid, &op, 1); }

int main()
{
    /* Create shared memory */
    int shmid = shmget(IPC_PRIVATE, 128, 0666 | IPC_CREAT);

    /* Create binary semaphore */
    int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    pid_t pid = fork();

    if (pid == 0) {
        /* Child */
        sem_p(semid);
        char *shm = (char *)shmat(shmid, NULL, 0);
        printf("Child: Writing 'Hello from Child' to shared memory.\n");
        strcpy(shm, "Hello from Child");
        sleep(1);
        printf("Child: Shared memory contains: %s\n", shm);
        shmdt(shm);
        sem_v(semid);
        exit(0);
    } else {
        /* Parent */
        sem_p(semid);
        char *shm = (char *)shmat(shmid, NULL, 0);
        printf("Parent: Writing 'Hello from Parent' to shared memory.\n");
        strcpy(shm, "Hello from Parent");
        sleep(1);
        printf("Parent: Shared memory contains: %s\n", shm);
        shmdt(shm);
        sem_v(semid);
        wait(NULL);
    }

    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/19$ gcc 31b.c -o 31b

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/19$ ./31b
Parent: Writing 'Hello from Parent' to shared memory.
Parent: Shared memory contains: Hello from Parent
Child: Writing 'Hello from Child' to shared memory.
Child: Shared memory contains: Hello from Child

(Semaphore ensures only one process writes at a time.)

*/
