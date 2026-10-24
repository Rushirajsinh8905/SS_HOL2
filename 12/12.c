/*
Name : 12.c
Author : Dabhi Rushirajsinh
Description : Write a program to print information about a message queue using
              msqid_ds and ipc_perm structures:
              (a) Access permission  (b) UID, GID
              (c) Time of last message sent and received
              (d) Time of last change in the message queue
              (e) Size of the queue  (f) Number of messages in the queue
              (g) Maximum number of bytes allowed
              (h) PID of msgsnd and msgrcv
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main()
{
    key_t key;
    int msgid;
    struct msqid_ds buf;
    struct msg_buffer message;

    key   = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    /* Send one message so timestamps are populated */
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello MsgQueue");
    msgsnd(msgid, &message, sizeof(message.msg_text), 0);
    msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);

    /* Retrieve queue info */
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }

    /* (a) Access permissions */
    printf("--- Message Queue Information ---\n");
    printf("(a) Access permission (mode) : %o\n", buf.msg_perm.mode);

    /* (b) UID and GID */
    printf("(b) UID                      : %d\n", buf.msg_perm.uid);
    printf("    GID                      : %d\n", buf.msg_perm.gid);

    /* (c) Times of last send and receive */
    printf("(c) Last msgsnd time         : %s",   ctime(&buf.msg_stime));
    printf("    Last msgrcv time         : %s",   ctime(&buf.msg_rtime));

    /* (d) Time of last change */
    printf("(d) Last change time         : %s",   ctime(&buf.msg_ctime));

    /* (e) Size of queue (max bytes) */
    printf("(e) Max bytes in queue       : %lu\n", (unsigned long)buf.msg_qbytes);

    /* (f) Number of messages currently in queue */
    printf("(f) Messages currently in queue: %lu\n", (unsigned long)buf.msg_qnum);

    /* (g) Max bytes (same as msg_qbytes) */
    printf("(g) Max number of bytes allowed: %lu\n", (unsigned long)buf.msg_qbytes);

    /* (h) PID of last msgsnd and msgrcv */
    printf("(h) PID of last msgsnd       : %d\n", buf.msg_lspid);
    printf("    PID of last msgrcv       : %d\n", buf.msg_lrpid);

    /* Remove queue */
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

/*
Sample Output :

# Compile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/12$ gcc 12.c -o 12

# Create progfile
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/12$ touch progfile

# Run
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/12$ ./12
--- Message Queue Information ---
(a) Access permission (mode) : 666
(b) UID                      : 1000
    GID                      : 1000
(c) Last msgsnd time         : Fri Oct 23 12:00:00 2026
    Last msgrcv time         : Fri Oct 23 12:00:00 2026
(d) Last change time         : Fri Oct 23 12:00:00 2026
(e) Max bytes in queue       : 16384
(f) Messages currently in queue: 0
(g) Max number of bytes allowed: 16384
(h) PID of last msgsnd       : <sample_pid>
    PID of last msgrcv       : <sample_pid>

(Actual values depend on kernel config and runtime.)

*/
