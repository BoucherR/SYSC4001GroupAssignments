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

    // This looks long but it's not; if you understand what's going on in the first IF block, the rest of the code is just this block
    // repeated three times. Once for each fork, basically.
    // Each of the IF blocks is one child process, aka the process in which each child has one row of the first array, and multiplies
    // this row against all of the columns in the second array

    if (pid1 == 0) { // it's a child
        printf("This is child 1 with pid %d and ppid %d \n", getpid(), getppid());
        // The row that this fork will use
        row = rowAtValue(0, M); 
        printf("Child Process 1: Working with row: ");
        for (int i = 0; i<4; i++) {
            printf("%d ", row[i]); // iterating over row array
        }
        printf("\n");

        int *dotProduct = malloc(sizeof(int)*4); // array that will hold final row that needs to be put into Q

        for (int i = 0; i<4; i++) {
            col = colAtValue(i, N); // column in second matrix that will be multipled against the row this fork is responsible for
            dotProduct[i] = (row[0] * col[0]) + (row[1] * col[1]) + (row[2] * col[2]) + (row[3] * col[3]); // dot product
        }

        // At this point dotProduct now contains the completed row multiplication against all of the columns in the second array,
        // and is done. 

        printf("First row in solved Q matrix is: \n"); 
        for (int i= 0; i<4; i++) {
            printf("%d ", dotProduct[i]);
        }
        printf("\n");
    }
    else {
        pid2 = fork();

        if (pid2 == 0) { // it's a child
            printf("This is child 2 with pid %d and ppid %d \n", getpid(), getppid());
            row = rowAtValue(1, M);
            printf("Child Process 2: Working with row: ");
            for (int i = 0; i<4; i++) {
                printf("%d ", row[i]); // iterating over row array
            }
            printf("\n");

            int *dotProduct = malloc(sizeof(int)*4);

            for (int i = 0; i<4; i++) {
                col = colAtValue(i, N);
                dotProduct[i] = (row[0] * col[0]) + (row[1] * col[1]) + (row[2] * col[2]) + (row[3] * col[3]);
            }

            printf("Second row in solved Q matrix is: \n");
            for (int i= 0; i<4; i++) {
                printf("%d ", dotProduct[i]);
            }
            printf("\n");
        }
        else {
            pid3 = fork();

            if (pid3 == 0) { // it's a child
                printf("This is child 3 with pid %d and ppid %d \n", getpid(), getppid());
                row = rowAtValue(2, M);
                printf("Child Process 3: Working with row: ");
                for (int i = 0; i<4; i++) {
                    printf("%d ", row[i]); // iterating over row array
                }
                printf("\n");

                int *dotProduct = malloc(sizeof(int)*4);

                for (int i = 0; i<4; i++) {
                    col = colAtValue(i, N);
                    dotProduct[i] = (row[0] * col[0]) + (row[1] * col[1]) + (row[2] * col[2]) + (row[3] * col[3]);
                }

                printf("Third row in solved Q matrix is: \n");
                for (int i= 0; i<4; i++) {
                    printf("%d ", dotProduct[i]);
                }
                printf("\n");
            }
            else {
                pid4 = fork();

                if (pid4 == 0) { // it's a child
                    printf("This is child 4 with pid %d and ppid %d \n", getpid(), getppid());
                    row = rowAtValue(3, M);
                    printf("Child Process 4: Working with row: ");
                    for (int i = 0; i<4; i++) {
                        printf("%d ", row[i]); // iterating over row array
                    }
                    printf("\n");

                    int *dotProduct = malloc(sizeof(int)*4);

                    for (int i = 0; i<4; i++) {
                        col = colAtValue(i, N);
                        dotProduct[i] = (row[0] * col[0]) + (row[1] * col[1]) + (row[2] * col[2]) + (row[3] * col[3]);
                    }

                    printf("Fourth row in solved Q matrix is: \n");
                    for (int i= 0; i<4; i++) {
                        printf("%d ", dotProduct[i]);
                    }
                    printf("\n");
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

