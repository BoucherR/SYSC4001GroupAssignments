#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "cliserv.h"
#define MAX_TEXT 512
int main()
{
    int running = 1;
    int msgid_send;
    int msgid_rec;
    my_msg_st command_msg;
    char buffer[BUFSIZ];
    int selected;

    //Initialize our message queues
    msgid_send = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid_send == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }
    msgid_rec = msgget((key_t)1235, 0666 | IPC_CREAT);
    if (msgid_rec == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }

    while(running)
    {
        printf("Enter command: ");
        //Collect entered commands
        fgets(buffer, BUFSIZ, stdin);
        command_msg.msg_type = 1;
        strcpy(command_msg.command, buffer);

        //Add commands to msg and send to Record Keeper
        if (msgsnd(msgid_send, (void *)&command_msg, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }

        //Wait for response from Record Keeper and print to console
        if (msgrcv(msgid_rec, (void *)&command_msg, BUFSIZ, 0, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno); exit(EXIT_FAILURE);
        }
        printf("%s\n", command_msg.command);
    }
    if(msgctl(msgid_send, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    if(msgctl(msgid_rec, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}
