#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "cliserv.h"
#define MAX_TEXT 512
employee_data collect(int, int, int, int);
int main()
{
    int running = 1;
    int msgid;
    msg_transfer some_data;
    employee_data employee;
    int selected;


    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    printf("%d", msgid);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }

    while(running)
    {
        do{
        printf("1-Insert Employee\n");
        printf("2-Check Name\n");
        printf("3-Check Department\n");
        printf("4-Check Salary\n");
        printf("5-Check Employee Number\n");
        printf("6-Check Department Name\n");
        printf("7-Delete Employee\n");

        printf("Select an option:");
        scanf("%d", &selected);
        if(selected < 0 || selected > 7)
        {
            printf("Invalid option, please try again.");
        }
        }while(selected < 0 || selected > 7);

        switch(selected)
        {
            case 1:
                employee = collect(1,1,1,1);
                break;
            case 2:
            case 3:
            case 4:
            case 7:
                employee = collect(0,0,1,0);
                break;
            case 5:
                employee = collect(1,0,0,0);
                break;
            case 6:
                employee = collect(0,1,0,0);
                break;

        }
        //some_data.request_type = selected;
        some_data.request_type = 1;
        some_data.data = employee;
        printf("Sending message");
        if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}

employee_data collect(int get_name, int get_dept_name, int get_emp_num, int get_salary)
{
    employee_data employee;
    employee.emp_number = 0;
    employee.salary = 0;

    if(get_name)
    {
        printf("Enter Employee Name: ");
        scanf("%s", employee.name);
    }
    if(get_dept_name)
    {
        printf("Enter Department Name: ");
        scanf("%s", employee.dept_name);
    }
    if(get_emp_num)
    {
        printf("Enter Employee Number: ");
        scanf("%ld", &employee.emp_number);
    }
    if(get_salary)
    {
        printf("Enter Salary: ");
        scanf("%ld", &employee.salary);
    }
    return employee;
    //printf("%s:%s:%ld:%ld", employee.name, employee.dept_name, employee.emp_number, employee.salary);
}
