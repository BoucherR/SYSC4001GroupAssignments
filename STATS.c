#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

// Function that given an array, orders the two values largest to smallest
void orderNumerically(int *array, int size) {
    for (int i = 0; i < size; i++) {
        int temp = array[i];

        if((i+1) < size) {
            if (array[i] < array[i+1]) {
                temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
            }
        }
    }
}

// Function to print an array
void printArray(int *array, int size) {
    printf("Array Values: ");

    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
	pid_t pid1, pid2, pid3, pid4; // each of the four fork process ID's

    // Creating shared memory
    void *shared_memory = (void *)0;
    struct shared_arrays *arrays;
    int shmid;
    shmid = shmget((key_t)1501, sizeof(int)*5, 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    int *Q = malloc(sizeof(int)*5); // matrix that will be ordered
    Q[0] = 5;
    Q[1] = 6;
    Q[2] = 8;
    Q[3] = 2;
    Q[4] = 7;

    // Assigning Q to shared memory, that each of the forks will modify
    Q = shared_memory; 

	printf("--------------------- Program starting ---------------------\n\n");
	
    pid1 = fork();

    if (pid1 == 0) { // it's a child
        printf("This is child 1 with pid %d and ppid %d \n", getpid(), getppid());

        int *pairOne = malloc(sizeof(int)*2);

        
        pairOne[0] = Q[0];
        pairOne[1] = Q[1];
        orderNumerically(pairOne, 2);
        
    }
    else { // second fork that will deal with the second row
        pid2 = fork();

        if (pid2 == 0) { // it's a child
            printf("This is child 2 with pid %d and ppid %d \n", getpid(), getppid());

            int *pairTwo = malloc(sizeof(int)*2);
            pairTwo[0] = Q[1];
            pairTwo[1] = Q[2];
            orderNumerically(pairTwo, 2);
            
        }
        else { // third fork that will deal with the third row
            pid3 = fork();

            if (pid3 == 0) { // it's a child
                printf("This is child 3 with pid %d and ppid %d \n", getpid(), getppid());

                int *pairThree = malloc(sizeof(int)*2);
                pairThree[0] = Q[2];
                pairThree[1] = Q[3];
                orderNumerically(pairThree, 2);
                
            }
            else { // fourth fork that will deal with the fourth row
                pid4 = fork();

                if (pid4 == 0) { // it's a child
                    printf("This is child 4 with pid %d and ppid %d \n", getpid(), getppid());

                    int *pairFour = malloc(sizeof(int)*2);
                    pairFour[0] = Q[3];
                    pairFour[1] = Q[4];
                    orderNumerically(pairFour, 2);
                }
                else // you're at the parent
                {
                
                }
            }
        }
    
    }

    sleep(1);
    exit(0);
}