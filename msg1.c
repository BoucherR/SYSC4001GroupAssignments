#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "cliserv.h"
int main() {
    int running = 1;
    int msgid;
    my_msg_st some_data;
    long int msg_to_receive = 0;


    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    printf("%d", msgid);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }

    while(running)
    {
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno); exit(EXIT_FAILURE);
        }
        printf("You wrote: %s", some_data.some_text.name);
        if (strncmp(some_data.some_text.name, "end", 3) == 0)
        {
            running = 0;
        }
    }
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

