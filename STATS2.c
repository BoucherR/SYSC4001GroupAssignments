
#include <errno.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>

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

static int set_semvalue(sem_id) {
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return (0);
  return (1);
}

static int semaphore_p(sem_id) {
  struct sembuf sem_b;

  sem_b.sem_num = 0;
  sem_b.sem_op = -1; /* P() */
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id, & sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_p failed\n");
    return 0;
  }
  return 1;

}

static int semaphore_v(sem_id) {
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1; /* V() */
  sem_b.sem_flg = SEM_UNDO;
  if (semop(sem_id, & sem_b, 1) == -1) {
    fprintf(stderr, "semaphore_v failed\n");
    return (0);
  }
  return (1);
}


int main()
{
    pid_t pid;
    int numValues = 5;
    int numForks = 4;
    int forkVal = 0;

    // prompting the user if they want to be in debug mode
    _Bool isDebug;
    printf( "Enter 1 if you want debug mode, enter 0 if you want regular mode: \n"); // how the user enters debug mode
    scanf("%i", &isDebug);

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
    int sem[numValues];
    for (int sems = 0; sems < numValues; sems++)
    {
        sem[sems] = semget((key_t) 1234+sems, 1, 0666 | IPC_CREAT);
        if (!set_semvalue(sem[sems])) {
            fprintf(stderr, "Failed to initialize semaphore\n");
            exit(EXIT_FAILURE);
        }
    }



    printf("--------------------- Program starting ---------------------\n\n");

    createArray(Q); // prompt the user to create the values that will be put into the array
    printf("Starting array: ");
    printArray(Q, 5); // print the starting array
    int offset;
    do
    {
        pid = fork();
        if(pid != 0)
        {
            forkVal++;
        }
        offset = (forkVal % 2 != 0);        //Get the starting value as forkVal + offset, secondary as forkVal + (1-offset)

    }
    while(pid !=0 && forkVal < numForks);

    if (pid == 0) { // it's a child
        while(!isSorted(Q, 5)) { // exit when the array is sorted

            if (!semaphore_p(sem[forkVal])) exit(EXIT_FAILURE);
            if (!semaphore_p(sem[forkVal+1])) exit(EXIT_FAILURE);
            orderNumerically(Q, 2, forkVal, isDebug);
            if (!semaphore_v(sem[forkVal+1])) exit(EXIT_FAILURE);
            if (!semaphore_v(sem[forkVal])) exit(EXIT_FAILURE);
            for (int i =0; i< 10000000; i++);


        }

    }
    if(pid != 0) // The main fork will deal with printing
    {
        wait(NULL);
        printf("Sorted array: ");
        printArray(Q, 5);
        printf("Minimum Value: %d \n", Q[4]);
        printf("Median Value: %d \n", Q[2]);
        printf("Maximum Value: %d \n", Q[0]);
    }

}
