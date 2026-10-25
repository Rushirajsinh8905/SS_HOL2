/*
Name : 26.c
Author : Dabhi Rushirajsinh
Description : Write a program to measure the time taken to execute 100
              getppid() system calls using a time stamp counter (TSC).
              Uses ARM64 counter register (cntvct_el0) on AArch64 Linux/macOS.
              For x86_64, replace with RDTSC inline assembly.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <unistd.h>

/*
 * Read the hardware cycle counter.
 * On ARM64 (QEMU / Raspberry Pi / Apple Silicon):  cntvct_el0
 * On x86_64: use __asm__ volatile("rdtsc" ...)
 */
static inline unsigned long long read_tsc(void)
{
#if defined(__aarch64__) || defined(__arm64__)
    unsigned long long val;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#elif defined(__x86_64__) || defined(__i386__)
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)hi << 32) | lo;
#else
    /* Fallback: not supported */
    return 0ULL;
#endif
}

int main()
{
    unsigned long long start, end, elapsed;
    int i;

    start = read_tsc();

    for (i = 0; i < 100; i++) {
        getppid();
    }

    end = read_tsc();
    elapsed = end - start;

    printf("100 getppid() calls completed.\n");
    printf("Start counter : %llu\n", start);
    printf("End counter   : %llu\n", end);
    printf("Elapsed cycles: %llu\n", elapsed);
    printf("Average cycles per call: %llu\n", elapsed / 100);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/26$ gcc 22.c -o 22

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/26$ ./22
100 getppid() calls completed.
Start counter : 3457892341256
End counter   : 3457892378900
Elapsed cycles: 37644
Average cycles per call: 376

(Counter values and cycle counts are runtime-dependent and vary per machine.)

*/
