#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "cliserv.h"
#define MAX_EMPLOYEE_SIZE 20
int main() {

    printf("This");
    int running = 1;
    int msgid;
    int test;
    int employee_index=0;
    employee_data employees[MAX_EMPLOYEE_SIZE];
    msg_transfer some_data;
    long int msg_to_receive = 0;

    printf("Here?");
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    printf("%d", msgid);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }
    printf("Made it here");

    while(running)
    {
        printf("Waiting to recieive");
        printf("%d", msgid);
        scanf("%d", &test);
        //sleep(1);
        if (msgrcv(msgid, &some_data, sizeof(some_data), msg_to_receive, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno); exit(EXIT_FAILURE);
        }
        printf("RECEIVED INFO");
        // switch(some_data.request_type)
        // {
        //     case 1:
        //         employees[employee_index] = some_data.data;
        //         employee_index++;
        //         break;
        // }
        // employees[employee_index] = some_data.data;
        // employee_index++;
        // //break;
        // int i;
        // for (i=0; i<employee_index;i++)
        // {
        //     printf("%s", employees[i].name);
        // }


    }
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


