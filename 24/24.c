/*
Name : 24.c
Author : Dabhi Rushirajsinh
Description : Write a program to print system resource limits using the
              getrlimit system call.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

void print_limit(int resource, const char *name)
{
    struct rlimit rl;
    if (getrlimit(resource, &rl) == 0) {
        printf("%-35s soft = ", name);
        if (rl.rlim_cur == RLIM_INFINITY)
            printf("unlimited");
        else
            printf("%lu", (unsigned long)rl.rlim_cur);

        printf(",  hard = ");
        if (rl.rlim_max == RLIM_INFINITY)
            printf("unlimited\n");
        else
            printf("%lu\n", (unsigned long)rl.rlim_max);
    } else {
        perror(name);
    }
}

int main()
{
    printf("%-35s %-20s %s\n", "Resource", "Soft Limit", "Hard Limit");
    printf("%-35s %-20s %s\n", "--------", "----------", "----------");

    print_limit(RLIMIT_CPU,     "RLIMIT_CPU (CPU time, s)");
    print_limit(RLIMIT_FSIZE,   "RLIMIT_FSIZE (file size, bytes)");
    print_limit(RLIMIT_DATA,    "RLIMIT_DATA (data segment, bytes)");
    print_limit(RLIMIT_STACK,   "RLIMIT_STACK (stack size, bytes)");
    print_limit(RLIMIT_CORE,    "RLIMIT_CORE (core file, bytes)");
    print_limit(RLIMIT_NOFILE,  "RLIMIT_NOFILE (open files)");
    print_limit(RLIMIT_AS,      "RLIMIT_AS (virtual memory, bytes)");

#ifdef RLIMIT_NPROC
    print_limit(RLIMIT_NPROC,   "RLIMIT_NPROC (processes/user)");
#endif

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/24$ gcc 20.c -o 20

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/24$ ./20
Resource                            Soft Limit           Hard Limit
--------                            ----------           ----------
RLIMIT_CPU (CPU time, s)            soft = unlimited,  hard = unlimited
RLIMIT_FSIZE (file size, bytes)     soft = unlimited,  hard = unlimited
RLIMIT_DATA (data segment, bytes)   soft = unlimited,  hard = unlimited
RLIMIT_STACK (stack size, bytes)    soft = 8388608,  hard = unlimited
RLIMIT_CORE (core file, bytes)      soft = 0,  hard = unlimited
RLIMIT_NOFILE (open files)          soft = 1024,  hard = 4096
RLIMIT_AS (virtual memory, bytes)   soft = unlimited,  hard = unlimited
RLIMIT_NPROC (processes/user)       soft = 63371,  hard = 63371

(Actual values depend on system configuration and ulimit settings.)

*/
