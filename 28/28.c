/*
Name : 28.c
Author : Dabhi Rushirajsinh
Description : Write a simple program to create three threads and print the
              IDs of the created threads.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *arg)
{
    printf("Thread ID: %lu  (pthread_self)\n", (unsigned long)pthread_self());
    return NULL;
}

int main()
{
    pthread_t th1, th2, th3;

    if (pthread_create(&th1, NULL, thread_func, NULL) != 0) { perror("pthread_create th1"); exit(1); }
    if (pthread_create(&th2, NULL, thread_func, NULL) != 0) { perror("pthread_create th2"); exit(1); }
    if (pthread_create(&th3, NULL, thread_func, NULL) != 0) { perror("pthread_create th3"); exit(1); }

    /* Wait for all threads to finish */
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    printf("Main thread: all three threads completed.\n");

    return 0;
}

/*
Sample Output :

# Compile (link pthread library)
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/28$ gcc 24.c -o 24 -lpthread

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/28$ ./24
Thread ID: 140123456789504  (pthread_self)
Thread ID: 140123448396800  (pthread_self)
Thread ID: 140123440004096  (pthread_self)
Main thread: all three threads completed.

(Thread IDs are runtime-dependent. Print order may vary due to scheduling.)

*/
