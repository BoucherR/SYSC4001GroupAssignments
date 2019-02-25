#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/time.h>
#include "shm_arr.h"

#define MICRO_SEC_IN_SEC 1000000

/**
 * Function that will return a row in a given array. Note, array MUST be 4x4
**/

void calcRow(int rowNumber, struct shared_arrays *arr)
{
    int sum;
    for (int i = 0; i < 4; i++)
    {
        sum = 0;
        for (int j = 0; j < 4; j++)
        {
            sum += arr->N[rowNumber][j] * arr->M[j][i]; // taking cross product
        }
        arr->Q[rowNumber][i] = sum; // placing into final solved matrix, Q

    }
    arr->numComplete = arr->numComplete+1;
}

int main() {
    struct timeval start, end;
    int numForks = 4; //Default number of forks


    pid_t pid;
    int rowNumber = 0;

    void *shared_memory = (void *)0;
    struct shared_arrays *arrays;
    int shmid;
    shmid = shmget((key_t)1502, sizeof(struct shared_arrays), 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    // putting M, N, and Q array structure into shared memory
    arrays = (struct shared_arrays *)shared_memory;
    // hard-coding values in M and N array in shared memory
    int x [4][4] = {1,2,3,4,5,6,7,8,4,3,2,1,8,7,6,5};
    int y [4][4] = {1,3,5,7,2,4,6,8,7,3,5,7,8,6,4,2};
    for(int i=0; i < 4; i++)
    {
        for(int j = 0; j< 4; j++)
        {
            arrays->N[i][j] = x[i][j];
            arrays->M[i][j] = y[i][j];
        }
    }
    arrays->numComplete = 0;

	printf("--------------------- Fork program starting ---------------------\n\n");
    do
    {
        if(numForks!=4)
        {
            printf("Invalid number of forks, please try again\n");
        }
        printf( "Enter number of forks(1,2 or 4):");
        scanf("%d", &numForks);
    }
    while(numForks!=1 && numForks !=2 && numForks!=4);
    gettimeofday(&start, NULL);
    do
    {
        pid = fork();
        if(pid != 0){
            rowNumber+=4/numForks; // rownumber this fork will be responsible for
        }
    }
    while(pid != 0 && rowNumber < 4); // stop at pre-defined value, numForks
    if(pid == 0)
    {
        int sum;
        for (int i=0; i<4/numForks;i++)
        {
            printf("Child Process %d: Working with row  %d\n", rowNumber, rowNumber+i);
            calcRow(rowNumber+i, arrays);
        }
        if(arrays->numComplete == 4) { // you're done
            for (int i = 0; i<4; i++) {
                for (int j = 0; j<4; j++) {
                    printf("%d ", arrays->Q[i][j]); // print final solved array, Q
                }
                printf("\n");
            }
            gettimeofday(&end, NULL);
            printf("Elapsed Time: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
        }
    }

    sleep(1);
    exit(0);
}

