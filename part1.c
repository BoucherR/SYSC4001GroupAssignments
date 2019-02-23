#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Function that will return a row in a given array. Note, array MUST be 4x4
**/
int * rowAtValue(int rowNum, int *array) {
    int *row = malloc(sizeof(int) * 4);
    int count = rowNum*4;

    for (int i = 0; i<4; i++) {
        row[i] = array[count];
        count++; 
    }

    return row;
}

/**
 * Function that will return a column in a given array. Note, array MUST be 4x4
**/
int * colAtValue(int colNum, int *array) {
    int *col = malloc(sizeof(int) * 4);
    int count = colNum;

    for (int i =0; i<4; i++) {
        col[i] = array[count];
        count += 4;
    }

    return col;
}

int main() {
	pid_t pid1, pid2, pid3, pid4; 

    int *row; // row that this fork will be responsible for
    int *col; // col that this fork will be responsible for

    int *M = malloc(sizeof(int)*16); // first matrix to multiply
    M[0] = 1;
    M[1] = 2;
    M[2] = 3;
    M[3] = 4;
    M[4] = 5;
    M[5] = 6;
    M[6] = 7;
    M[7] = 8;
    M[8] = 4;
    M[9] = 3;
    M[10] = 2;
    M[11] = 1;
    M[12] = 8;
    M[13] = 7;
    M[14] = 6;
    M[15] = 5;

    int *N = malloc(sizeof(int)*16); // second matrix to multiply
    N[0] = 1;
    N[1] = 3;
    N[2] = 5;
    N[3] = 7;
    N[4] = 2;
    N[5] = 4;
    N[6] = 6;
    N[7] = 8;
    N[8] = 7;
    N[9] = 3;
    N[10] = 5;
    N[11] = 7;
    N[12] = 8;
    N[13] = 6;
    N[14] = 4;
    N[15] = 2;

    int *Q = malloc(sizeof(int)*16); // matrix to store completed multiplication in
    Q[0] = 0;
    Q[1] = 0;
    Q[2] = 0;
    Q[3] = 0;
    Q[4] = 0;
    Q[5] = 0;
    Q[6] = 0;
    Q[7] = 0;
    Q[8] = 0;
    Q[9] = 0;
    Q[10] = 0;
    Q[11] = 0;
    Q[12] = 0;
    Q[13] = 0;
    Q[14] = 0;
    Q[15] = 0;

	printf("--------------------- Fork program starting ---------------------\n\n");
	
    pid1 = fork();

    if (pid1 == 0) { // it's a child
        printf("This is child 1 with pid %d and ppid %d \n", getpid(), getppid());
        row = rowAtValue(0, M);
        col = colAtValue(0, N);

        printf("Row Values for Child 1:\n");
        for (int i = 0; i<4; i++) {
            printf("%d", row[i]);
        }
        printf("\n");
        printf("Column Values for Child 1:\n");
        for (int i = 0; i<4; i++) {
            printf("%d\n", col[i]);
        }

    }
    else {
        pid2 = fork();

        if (pid2 == 0) { // it's a child
            printf("This is child 2 with pid %d and ppid %d \n", getpid(), getppid());
            row = rowAtValue(1, M);
            col = colAtValue(1, N);

            printf("Row Values for Child 2:\n");
            for (int i = 0; i<4; i++) {
                printf("%d", row[i]);
            }
            printf("\n");
            printf("Column Values for Child 2:\n");
            for (int i = 0; i<4; i++) {
                printf("%d\n", col[i]);
            }
        }
        else {
            pid3 = fork();

            if (pid3 == 0) { // it's a child
                printf("This is child 3 with pid %d and ppid %d \n", getpid(), getppid());
                row = rowAtValue(2, M);
                col = colAtValue(2, N);

                printf("Row Values for Child 3:\n");
                for (int i = 0; i<4; i++) {
                    printf("%d", row[i]);
                }
                printf("\n");
                printf("Column Values for Child 3:\n");
                for (int i = 0; i<4; i++) {
                    printf("%d\n", col[i]);
                }
            }
            else {
                pid4 = fork();

                if (pid4 == 0) { // it's a child
                    printf("This is child 4 with pid %d and ppid %d \n", getpid(), getppid());
                    row = rowAtValue(3, M);
                    col = colAtValue(3, N);

                    printf("Row Values for Child 4:\n");
                    for (int i = 0; i<4; i++) {
                        printf("%d", row[i]);
                    }
                    printf("\n");
                    printf("Column Values for Child 4:\n");
                    for (int i = 0; i<4; i++) {
                        printf("%d\n", col[i]);
                    }
                }
                else // you're at the parent
                {
                    sleep(2);
                    printf("This is the parent with pid %d \n", getpid());
                    printf("(If the matrix contains all zeroes this is still a work in progress)\n");
                    printf("Matrix Q contains: \n");
                    for (int i = 0; i<16; i++) {
                        if (i != 0 && i%4 == 0 ) {
                            printf("\n"); // formatting output to look like a 2-D array
                        }
                        printf("%d", Q[i]);
                    }
                    printf("\n");
                }
            }
        }
    
    }

    sleep(1);
    exit(0);
}

