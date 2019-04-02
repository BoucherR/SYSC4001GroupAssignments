typedef struct {
    char name[BUFSIZ];
    char dept_name[BUFSIZ];
    long int emp_number;
    long int salary;
} employee_data;


typedef struct {
    long int request_type;
    employee_data data;
} msg_transfer;
