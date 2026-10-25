/*
Name : 18a.c
Author : Dabhi Rushirajsinh
Description : Write a program to create a binary semaphore (initial value = 1)
              and initialize its value using semctl with SETVAL.
              A binary semaphore acts like a mutex/lock.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* On Linux, union semun must be defined by the user */
#ifndef __APPLE__
union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};
#endif

int main()
{
    key_t key;
    int semid;
    union semun arg;

    key   = ftok("semfile", 65);
    if (key == -1) { perror("ftok"); exit(1); }

    /* Create a set of 1 semaphore */
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) { perror("semget"); exit(1); }

    /* Initialize to 1 (binary semaphore = mutex) */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) { perror("semctl SETVAL"); exit(1); }

    printf("Binary semaphore created.\n");
    printf("  semid         = %d\n", semid);
    printf("  initial value = %d  (1 = unlocked)\n", semctl(semid, 0, GETVAL));

    printf("Use 'ipcs -s' to inspect. Remove with 'ipcrm -s %d'\n", semid);

    /* Clean up */
    semctl(semid, 0, IPC_RMID);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ gcc 18a.c -o 18a
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ touch semfile

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/18$ ./18a
Binary semaphore created.
  semid         = 65536
  initial value = 1  (1 = unlocked)
Use 'ipcs -s' to inspect. Remove with 'ipcrm -s 65536'

*/
