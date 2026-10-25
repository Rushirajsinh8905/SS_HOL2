/*
Name : 25.c
Author : Dabhi Rushirajsinh
Description : Write a program to set a system resource limit using the
              setrlimit system call. This example sets the core file size
              limit, then verifies the change using getrlimit.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void print_limit(int resource, const char *name)
{
    struct rlimit rl;
    getrlimit(resource, &rl);
    printf("%s: soft = %lu, hard = %lu\n", name,
           (unsigned long)rl.rlim_cur,
           (unsigned long)rl.rlim_max);
}

int main()
{
    struct rlimit rl;

    /* Show current RLIMIT_CORE limit */
    printf("Before setrlimit:\n");
    print_limit(RLIMIT_CORE, "RLIMIT_CORE");

    /* Set soft limit to 0 (no core dumps) */
    rl.rlim_cur = 0;
    rl.rlim_max = RLIM_INFINITY;   /* keep hard limit unlimited */

    if (setrlimit(RLIMIT_CORE, &rl) == -1) {
        perror("setrlimit");
        exit(1);
    }

    /* Verify change */
    printf("After setrlimit (core = 0):\n");
    print_limit(RLIMIT_CORE, "RLIMIT_CORE");

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/25$ gcc 21.c -o 21

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/25$ ./21
Before setrlimit:
RLIMIT_CORE: soft = 0, hard = 4294967295
After setrlimit (core = 0):
RLIMIT_CORE: soft = 0, hard = 4294967295

*/
