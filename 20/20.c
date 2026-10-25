/*
Name : 20.c
Author : Dabhi Rushirajsinh
Description : Develop a program that intentionally induces a deadlock scenario
              using semaphores. Two processes each hold one semaphore and wait
              for the other — classical circular-wait deadlock.
              Process A: locks sem1 then tries to lock sem2.
              Process B: locks sem2 then tries to lock sem1.
              Both block indefinitely -> deadlock.
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

/* Semaphore P (lock/wait) */
void sem_p(int semid, int semnum)
{
    struct sembuf op = {(unsigned short)semnum, -1, 0};
    semop(semid, &op, 1);
}

/* Semaphore V (unlock/signal) */
void sem_v(int semid, int semnum)
{
    struct sembuf op = {(unsigned short)semnum, +1, 0};
    semop(semid, &op, 1);
}

int main()
{
    /* Create a semaphore set with 2 semaphores */
    int semid = semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT);
    if (semid == -1) { perror("semget"); exit(1); }

    union semun arg;

    /* sem[0] = 1 (Resource 1) */
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    /* sem[1] = 1 (Resource 2) */
    arg.val = 1;
    semctl(semid, 1, SETVAL, arg);

    printf("Deadlock demonstration:\n");
    printf("  Process A acquires Resource1 then waits for Resource2.\n");
    printf("  Process B acquires Resource2 then waits for Resource1.\n");
    printf("  Both will block indefinitely => DEADLOCK.\n\n");

    pid_t pid = fork();

    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        /* Process B: lock sem[1] first, then try sem[0] */
        printf("Process B (PID=%d): Acquiring Resource2...\n", getpid());
        sem_p(semid, 1);
        printf("Process B: Got Resource2. Sleeping 1s then requesting Resource1...\n");
        sleep(1);
        printf("Process B: Waiting for Resource1 (held by Process A)... BLOCKED\n");
        sem_p(semid, 0);   /* Will block — deadlock */
        printf("Process B: Got Resource1. (Should never print if deadlock occurs)\n");
        sem_v(semid, 0);
        sem_v(semid, 1);
        exit(0);
    } else {
        /* Process A: lock sem[0] first, then try sem[1] */
        printf("Process A (PID=%d): Acquiring Resource1...\n", getpid());
        sem_p(semid, 0);
        printf("Process A: Got Resource1. Sleeping 1s then requesting Resource2...\n");
        sleep(1);
        printf("Process A: Waiting for Resource2 (held by Process B)... BLOCKED\n");
        sem_p(semid, 1);   /* Will block — deadlock */
        printf("Process A: Got Resource2. (Should never print if deadlock occurs)\n");
        sem_v(semid, 1);
        sem_v(semid, 0);
        wait(NULL);
    }

    semctl(semid, 0, IPC_RMID);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/20$ gcc 32.c -o 32

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/20$ ./32
Deadlock demonstration:
  Process A acquires Resource1 then waits for Resource2.
  Process B acquires Resource2 then waits for Resource1.
  Both will block indefinitely => DEADLOCK.

Process A (PID=1241): Acquiring Resource1...
Process A: Got Resource1. Sleeping 1s then requesting Resource2...
Process B (PID=1242): Acquiring Resource2...
Process B: Got Resource2. Sleeping 1s then requesting Resource1...
Process A: Waiting for Resource2 (held by Process B)... BLOCKED
Process B: Waiting for Resource1 (held by Process A)... BLOCKED

(Program hangs here — both processes are in deadlock. Press Ctrl+C to terminate.)

# Clean up IPC resources if program is killed:
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/20$ ipcs -s
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/20$ ipcrm -s <semid>

*/
