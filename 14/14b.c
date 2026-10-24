/*
Name : 14b.c
Author : Dabhi Rushirajsinh
Description : Write a program to receive messages from the message queue
              with IPC_NOWAIT as a flag (non-blocking receive).
              If no message is available, the call returns immediately with ENOMSG.
Date : 23rd Oct, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
    struct msg_buffer message;

    key   = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) { perror("msgget"); exit(1); }

    printf("Attempting non-blocking receive (IPC_NOWAIT)...\n");

    /* IPC_NOWAIT: return immediately if no message is available */
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            printf("No message available (IPC_NOWAIT returned ENOMSG).\n");
        } else {
            perror("msgrcv");
        }
    } else {
        printf("Message received (IPC_NOWAIT): %s\n", message.msg_text);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

/*
Sample Output :

# Case 1: No message in queue (IPC_NOWAIT behavior)
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/14$ ./14b
Attempting non-blocking receive (IPC_NOWAIT)...
No message available (IPC_NOWAIT returned ENOMSG).

# Case 2: Message present (run sender first)
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/14$ ./14_sender
Message sent: Hello from sender!
dabhi-rushirajsinh@dabhi-rushirajsinh-QEMU-Virtual-Machine:~/Hand_On_List2/14$ ./14b
Attempting non-blocking receive (IPC_NOWAIT)...
Message received (IPC_NOWAIT): Hello from sender!

*/
