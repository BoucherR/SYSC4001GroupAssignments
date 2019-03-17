#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <stdbool.h>

// Function that given an array, orders the two values largest to smallest
void orderNumerically(int *array, int size, _Bool debug) {
    _Bool swapPerformed = false;
    for (int i = 0; i < size; i++) {
        int temp = array[i];

        if((i+1) < size) {
            if (array[i] < array[i+1]) {
                temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
                swapPerformed = true;
                if (debug) {
                    printf(" Performed swapping \n");
                }
            }
        }
    }
    if (swapPerformed == false && debug) {
        printf(" No swapping \n");
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

// Function to check if an array is sorted correctly. Used to determine the exit condition
// on the do-while loops that each fork uses
_Bool isSorted(int *array, int size) {
    for (int i = 0; i< size; i++) { // iterate over array
        if ((i+1) < size) { // keep in bounds of array
            if (array[i] < array[i+1]) { // check that every element to left is greater than element to right
                return false; // array is not sorted if above is true
            }
        }
    }
    return true;
}

int main() {
	pid_t pid1, pid2, pid3, pid4; // each of the four fork process ID's

    _Bool isDebug;
    printf( "Enter 1 if you want debug mode, enter 0 if you want regular mode: \n");
    scanf("%d", &isDebug);

    // Creating shared memory
    void *shared_memory = (void *)0;
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

    int* Q = shared_memory; // Assigning Q to shared memory, that each of the forks will modify
    Q[0] = 10;
    Q[1] = 9;
    Q[2] = 11;
    Q[3] = 5;
    Q[4] = 7;

    // Starting Semaphore
    errno = 0;
    sem_t *sem;
    sem = sem_open("/mysem", O_CREAT|O_EXCL, 0, 1);
    sem_unlink("/mysem");
    if (sem == SEM_FAILED) {
        int err1 = errno;
        fprintf(stderr, "sem_open() failed.  errno:%d\n", err1);
    }

	printf("--------------------- Program starting ---------------------\n\n");

    printf("Starting array: ");
    printArray(Q, 5);
	
    pid1 = fork();

    if (pid1 == 0) { // it's a child
        while(isSorted(Q, 5) == false) {
            //wait
            sem_wait(sem);

            // critical section
            if (isDebug) {
                printf("P1: ");
            }
            orderNumerically(Q, 2, isDebug);

            //signal
            sem_post(sem);
        } 
        
    }
    else { // second fork that will deal with the second row
        pid2 = fork();

        if (pid2 == 0) { // it's a child
            while(isSorted(Q, 5) == false) {
                //wait
                sem_wait(sem);

                // critical section
                if (isDebug) {
                    printf("P2: ");
                }
                orderNumerically((Q+1), 2, isDebug);

                //signal
                sem_post(sem);
            } 
            
        }
        else { // third fork that will deal with the third row
            pid3 = fork();

            if (pid3 == 0) { // it's a child
                while(isSorted(Q, 5) == false) {
                    //wait
                    sem_wait(sem);

                    // critical section
                    if (isDebug) {
                        printf("P3: ");
                    }
                    orderNumerically((Q+2), 2, isDebug);

                    //signal
                    sem_post(sem);
                } 
                
            }
            else { // fourth fork that will deal with the fourth row
                pid4 = fork();

                if (pid4 == 0) { // it's a child
                    while(isSorted(Q, 5) == false) {
                        //wait
                        sem_wait(sem);

                        // critical section
                        if (isDebug) {
                            printf("P4: ");
                        }
                        orderNumerically((Q+3), 2, isDebug);

                        //signal
                        sem_post(sem);
                    } 
                }
                else // you're at the parent
                {
                    while(isSorted(Q, 5) == false) {
                            // do nothing
                    }
                    sleep(2);
                    printf("Sorted array: ");
                    printArray(Q, 5);
                }
            }
        }
    
    }

    sleep(1);
    exit(0);
}