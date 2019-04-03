#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include "cliserv.h"
#define MAX_TEXT 512



struct employee_data {
    char dept_name[BUFSIZ];
    char name[BUFSIZ];
    int emp_number;
    int salary;
    struct employee_data *next;
};

typedef struct employee_data *node; //Define node as pointer of data type struct LinkedList
node head;
void parse(char command[]);
void remove_spaces(char* source);
void insert(char* ptr);
void send_msg(char *msg);
void check_name(char *ptr);
void check_dept(char *ptr);
void check_salary(char *ptr);
void check_employee_number(char *ptr);
void check(char *ptr);
void delete(char *ptr);
node create_node();
int msgid_send;
struct employee_data first_emp;



int main() {

    int running = 1;
    int msgid_rec;
    int test;

    my_msg_st command_msg;
    long int msg_to_receive = 0;

    msgid_rec= msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid_rec == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }
    msgid_send= msgget((key_t)1235, 0666 | IPC_CREAT);
    if (msgid_send == -1)
    {
        fprintf(stderr, "msgget failed with error: %d\n?", errno); exit(EXIT_FAILURE);
    }

    while(running)
    {

        if (msgrcv(msgid_rec, (void *)&command_msg, BUFSIZ, msg_to_receive, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno); exit(EXIT_FAILURE);
        }
        parse(command_msg.command);


    }
    if(msgctl(msgid_rec, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    if(msgctl(msgid_send, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void parse(char command[])
{
    printf("%s", command);
    char delim[] = "()";
    char *ptr = strtok(command, delim);
    remove_spaces(ptr);
    if(!strcmp(ptr, "Insert"))
    {
        insert(ptr);
    }
    else if(!strcmp(ptr, "Check_name"))
    {
        check_name(ptr);
    }
    else if(!strcmp(ptr, "Check_department"))
    {
        check_dept(ptr);
    }
    else if(!strcmp(ptr, "Check_salary"))
    {
        check_salary(ptr);
    }
    else if(!strcmp(ptr, "Check_employee_number"))
    {
        check_employee_number(ptr);
    }
    else if(!strcmp(ptr, "Check"))
    {
        check(ptr);
    }
    else if(!strcmp(ptr, "Delete"))
    {
        delete(ptr);
    }
    else
    {
        send_msg("Invalid Command, please try again");
    }

    // while(ptr != NULL)
    // {
    //     printf("'%s'\n", ptr);
    //     ptr = strtok(NULL, delim);
    // }


}

void insert(char *ptr){
    node temp,p;// declare two nodes temp and p
    temp = create_node();//createNode will return a new node with data = value and next pointing to NULL.

    char delim[] = ",";

    ptr = strtok(NULL, delim);
    strcpy(temp->name, ptr);
    ptr = strtok(NULL, delim);
    strcpy(temp->dept_name, ptr);
    ptr = strtok(NULL, delim);
    temp->emp_number = atoi(ptr);
    ptr = strtok(NULL, delim);
    temp->salary = atoi(ptr);

    if(head == NULL){
        head = temp;     //when linked list is empty
    }
    else{
        p  = head;//assign head to p
        while(p->next != NULL){
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;
    }
    send_msg("Employee inserted successfully");
    return;
}

void delete(char *ptr){
    node temp, lag;// declare two nodes temp and p
    temp = create_node();//createNode will return a new node with data = value and next pointing to NULL.

    char delim[] = ",";


    ptr = strtok(NULL, delim);
    if(head == NULL){
        send_msg("Delete was unsuccessful(0)");
        return;
    }
    temp = head;
    lag = NULL;
    do
    {
        if(temp->emp_number ==  atoi(ptr))
        {
            if(lag == NULL)
            {
                head = temp->next;
            }
            else
            {
                lag->next = temp->next;
            }
            free(temp);
            send_msg("Delete was successful(1)");
            return;
        }
        lag = temp;
        temp = temp->next;
    }while(temp !=NULL);
    send_msg("Delete was unsuccessful(0)");
    return;
}

void check_name(char *ptr)
{
    char delim[] = ",";
    if(head == NULL){
       send_msg("No employees exist");
       return;
    }
    node temp;
    temp = head;
    ptr = strtok(NULL, delim);
    do
    {
        if(temp->emp_number ==  atoi(ptr))
        {
            char t[MAX_TEXT];
            snprintf(t, sizeof(t), "Employee Name: %s", temp->name);
            send_msg(t);
            return;
        }
        temp = temp->next;
    }while(temp !=NULL);
    send_msg("Employee not found");
}

void check_dept(char *ptr)
{
    char delim[] = ",";
    if(head == NULL){
       send_msg("No employees exist");
       return;
    }
    node temp;
    temp = head;
    ptr = strtok(NULL, delim);
    do
    {
        if(temp->emp_number ==  atoi(ptr))
        {
            char t[MAX_TEXT];
            snprintf(t, sizeof(t), "Employee Department: %s", temp->dept_name);
            send_msg(t);
            return;
        }
        temp = temp->next;
    }while(temp !=NULL);
    send_msg("Employee not found");
}

void check_salary(char *ptr)
{
    char delim[] = ",";
    if(head == NULL){
       send_msg("No employees exist");
       return;
    }
    node temp;
    temp = head;
    ptr = strtok(NULL, delim);
    do
    {
        if(temp->emp_number ==  atoi(ptr))
        {
            char t[MAX_TEXT];
            snprintf(t, sizeof(t), "Employee Salary: %d", temp->salary);
            send_msg(t);
            return;
        }
        temp = temp->next;
    }while(temp !=NULL);
    send_msg("Employee not found");
}

void check_employee_number(char *ptr)
{
    char delim[] = ",)";
    if(head == NULL){
       send_msg("No employees exist");
       return;
    }
    node temp;
    temp = head;
    ptr = strtok(NULL, delim);
    do
    {
        printf("%s:%s", temp->name, ptr);
        if(strcmp(temp->name, ptr) == 0)
        {
            char t[MAX_TEXT];
            snprintf(t, sizeof(t), "Employee Number: %d", temp->emp_number);
            send_msg(t);
            return;
        }
        temp = temp->next;
    }while(temp !=NULL);
    send_msg("Employee not found");
}

void check(char *ptr)
{
    char delim[] = ",)";
    if(head == NULL){
       send_msg("No employees exist");
       return;
    }
    node temp;
    temp = head;
    ptr = strtok(NULL, delim);
    int num = 0;
    char t[MAX_TEXT];
    snprintf(t, sizeof(t), "Employee Numbers:");
    do
    {
        printf("%s:%s", temp->name, ptr);
        if(strcmp(temp->dept_name, ptr) == 0)
        {

            snprintf(t, sizeof(t), "%s\n%d", t, temp->emp_number);
            num++;
        }
        temp = temp->next;
    }while(temp !=NULL);
    if(num!=0)
    {
        send_msg(t);
        return;
    }
    send_msg("Department has no employees");
}





node create_node(){
    node temp; // declare a node
    temp = (node)malloc(sizeof(struct employee_data)); // allocate memory using malloc()
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}
void remove_spaces(char* source)
{
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
        {
            i++;
        }
    }
    *i = 0;
}

void send_msg(char *msg)
{
    my_msg_st to_send;
    to_send.msg_type = 1;
    strcpy(to_send.command, msg);
    if (msgsnd(msgid_send, (void *)&to_send, MAX_TEXT, 0) == -1)
    {
        fprintf(stderr, "msgsnd failed\n");
        exit(EXIT_FAILURE);
    }
}


