#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "shm_arr.h"

/**
 * Function that will return a row in a given array. Note, array MUST be 4x4
**/
#define numForks 4

int main() {
    pid_t pid;
    int rowNumber = 0;

    void *shared_memory = (void *)0;
    struct shared_arrays *arrays;
    int shmid;
    shmid = shmget((key_t)1501, sizeof(struct shared_arrays), 0666 | IPC_CREAT);

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
        pid = fork();
        if(pid != 0){
            rowNumber++; // rownumber this fork will be responsible for
        }
    }
    while(pid != 0 && rowNumber < numForks); // stop at pre-defined value, numForks
    if(pid == 0)
    {
        int sum;
        printf("Child Process: Working with row  %d\n", rowNumber);
        for (int i = 0; i < 4; i++)
        {
            sum = 0;
            for (int j = 0; j < 4; j++)
            {
                sum += arrays->N[rowNumber][j] * arrays->M[j][i]; // taking cross product
            }
            arrays->Q[rowNumber][i] = sum; // placing into final solved matrix, Q
        }
        arrays->numComplete = arrays->numComplete+1;
        if(arrays->numComplete == 4) { // you're done
            for (int i = 0; i<4; i++) {
                for (int j = 0; j<4; j++) {
                    printf("%d ", arrays->Q[i][j]); // print final solved array, Q
                }
                printf("\n");
            }
        }
    }

    sleep(1);
    exit(0);
}

