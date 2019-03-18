#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <stdbool.h>

void debugPrint(char* toPrint,_Bool isDebug, int forkVal)
{
    if(isDebug)
    {
        printf("P%i%s", forkVal+1, toPrint);
    }
}

// Function to prompt user to input 5 numbers that will make array to be sorted
void createArray(int *array) {
    printf("Enter the 5 numbers for the array to be sorted, hitting enter after each number: \n");
    for (int i = 0; i< 5; i++) {
        scanf("%d", &array[i]);
    }
}



void orderNumerically(int *array, int size, int forkVal, _Bool debug)
{
    if(array[forkVal] < array[forkVal+1])
    {
        int temp = array[forkVal];
        array[forkVal] = array[forkVal+1];
        array[forkVal+1] = temp;
        debugPrint(" Performed swapping \n", debug, forkVal);
    }
    else
    {
        debugPrint(" No Swapping \n", debug, forkVal);
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
    for (int i = 1; i < size; i++) { // iterate over array
        if (array[i-1] < array[i]) { // check that every element to left is greater than element to right
            return false; // array is not sorted if above is true
        }
    }
    return true;
}




int main()
{
    pid_t pid;
    int numForks = 4;
    int forkVal = 0;

    // prompting the user if they want to be in debug mode
    _Bool isDebug;
    printf( "Enter 1 if you want debug mode, enter 0 if you want regular mode: \n"); // how the user enters debug mode
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

    createArray(Q); // prompt the user to create the values that will be put into the array
    printf("Starting array: ");
    printArray(Q, 5); // print the starting array

    do
    {
        pid = fork();
        if(pid != 0)
        {
            forkVal++;
        }
    }
    while(pid !=0 && forkVal < numForks);

    if (pid == 0) { // it's a child
        while(!isSorted(Q, 5)) { // exit when the array is sorted
            //wait
            sem_wait(sem);

            // critical section
            orderNumerically(Q, 2, forkVal, isDebug);

            //signal
            sem_post(sem);
        }
    }
    if(forkVal == numForks-1) // The last fork will deal with printing
    {
        printf("Sorted array: ");
        printArray(Q, 5);
        printf("Minimum Value: %d \n", Q[4]);
        printf("Median Value: %d \n", Q[2]);
        printf("Maximum Value: %d \n", Q[0]);
    }

}
