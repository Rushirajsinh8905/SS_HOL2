/*
Name : 27.c
Author : Dabhi Rushirajsinh
Description : Write a program to print system limitations for:
              (a) Maximum length of arguments in exec family.
              (b) Maximum number of simultaneous processes per user ID.
              (c) Number of clock ticks (jiffies) per second.
              (d) Maximum number of open files.
              (e) Size of a page.
              (f) Total number of pages in physical memory.
              (g) Number of currently available pages in physical memory.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    long val;

    /* (a) Max length of arguments to exec family */
    val = sysconf(_SC_ARG_MAX);
    printf("(a) Max exec argument length (bytes) : %ld\n", val);

    /* (b) Max simultaneous processes per user ID */
    val = sysconf(_SC_CHILD_MAX);
    printf("(b) Max processes per user ID        : %ld\n", val);

    /* (c) Clock ticks (jiffies) per second */
    val = sysconf(_SC_CLK_TCK);
    printf("(c) Clock ticks per second (jiffies) : %ld\n", val);

    /* (d) Maximum number of open files */
    val = sysconf(_SC_OPEN_MAX);
    printf("(d) Max open files per process       : %ld\n", val);

    /* (e) Size of a memory page */
    val = sysconf(_SC_PAGESIZE);
    printf("(e) Page size (bytes)                : %ld\n", val);

    /* (f) Total number of pages in physical memory (Linux-specific) */
#ifdef _SC_PHYS_PAGES
    val = sysconf(_SC_PHYS_PAGES);
    if (val == -1)
        printf("(f) Total physical pages             : Not available\n");
    else
        printf("(f) Total physical pages             : %ld\n", val);
#else
    printf("(f) Total physical pages             : Not available on this OS\n");
#endif

    /* (g) Number of currently available pages (Linux-specific) */
#ifdef _SC_AVPHYS_PAGES
    val = sysconf(_SC_AVPHYS_PAGES);
    if (val == -1)
        printf("(g) Available physical pages         : Not available\n");
    else
        printf("(g) Available physical pages         : %ld\n", val);
#else
    printf("(g) Available physical pages         : Not available on this OS\n");
#endif

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/27$ gcc 23.c -o 23

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/27$ ./23
(a) Max exec argument length (bytes) : 2097152
(b) Max processes per user ID        : 63371
(c) Clock ticks per second (jiffies) : 100
(d) Max open files per process       : 1024
(e) Page size (bytes)                : 4096
(f) Total physical pages             : 2097152
(g) Available physical pages         : 1234567

(Note: _SC_PHYS_PAGES and _SC_AVPHYS_PAGES are Linux-specific. Not available on macOS.)

*/
