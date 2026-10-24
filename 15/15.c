/*
Name : 15.c
Author : Dabhi Rushirajsinh
Description : Write a program to change the existing message queue permissions
              using the msqid_ds structure and msgctl with IPC_SET.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main()
{
    key_t key;
    int msgid;
    struct msqid_ds buf;

    key   = ftok("progfile", 65);
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) { perror("msgget"); exit(1); }

    /* Get current queue info */
    if (msgctl(msgid, IPC_STAT, &buf) == -1) { perror("msgctl IPC_STAT"); exit(1); }
    printf("Current permissions : %o\n", buf.msg_perm.mode);

    /* Change permissions to 0644 */
    buf.msg_perm.mode = 0644;
    if (msgctl(msgid, IPC_SET, &buf) == -1) { perror("msgctl IPC_SET"); exit(1); }

    /* Verify */
    msgctl(msgid, IPC_STAT, &buf);
    printf("New permissions     : %o\n", buf.msg_perm.mode);

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/15$ gcc 15.c -o 15
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/15$ touch progfile

# Run 15_sender first (optional, to pre-create queue):
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/15$ ./15_sender
Message sent: Hello from sender!

# Then run 15:
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/15$ ./15
Current permissions : 666
New permissions     : 644

*/
