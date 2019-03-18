----------------------------------
Assignment 2 README 
----------------------------------
    Authors: 
        Name: Ryan Boucher      Student #: 101049347
        Name: Simon Krol        Student #: 101047304

----------------------------------
Instructions on Running this Code
----------------------------------
    This code has one file, STATS.c, which runs under two modes. Upon compiling and executing this code, the program will prompt the user to enter a 1 if they want to enter the debug mode, or a 0 if they want to enter the normal mode. Everything else will be handled by the program without depending on the user. 

    The normal mode will output the starting array, as well as the sorted array, once sorted. 

    The debug mode start by outputting its starting array. Next, it will have each process output whether or not it has performed a switch every time it checks its two values that it reads from the shared array. Finally, it will output the sorted array.


----------------------------------
Algorithm to Sort Array
----------------------------------
    The algorithm to sort this array works as follows:
        1. A semaphore is set up to determine whether or not the array to be sorted is being accessed and modified by a process.
        2. The code forks four times, with each process being responsible for two elements in the array to be sorted. While the function isSorted() returns false for the array to be sorted, the fork will continually execute.
        3: Each of the forks attempts to read from the array to be sorted, and if the semaphore allows the array to be accessed, the fork will run orderNumerically() at the appropriate two elements in the array.
        4. Depending on whether the debug mode is true or false, the fork and orderNumerically() will print what it is currently doing.
        5. The forks will continue to perform these operations until isSorted() returns true, for the shared memory array that has been sorted. 
        6. The code will output the sorted array.


----------------------------------
Fork Pseudo-Code:
----------------------------------

    pid = fork() // create the fork

    if (pid == 0) { this is a child
        while(arrayNotSorted( arrayBeingSorted )) {
            // wait section
            sem_wait() // wait on the semaphore until it is available, aka can be modified by this fork

            // critical section
            swapIfNeeded(arrayBeingSorted[values) // swap two values, if needed, that this fork is responsible for

            // signal section
            sem_post() // signal that the critical section is done, therefore the semaphore is now available to be taken by another fork
        }
    }


    Four of the above forks exist in the code, each responsible for a specific two values in the array. When the array is finally sorted, the original parent will proceed to print the largest value, smallest value, and median value of the sorted array, as well as the sorted array itself. 


----------------------------------
Testing Discussion
----------------------------------
    This code has been tested with both sets of arrays that have been mandated: 
        i)      Q = [5 6 8 2 7]
        ii)     Q = [10 9 11 5 7]
    With successful sorting being achieved in both test cases. 

    All other cases that have been tested have been successful.  

    Output of First Test Case:

        --------------------- Program starting ---------------------

        Enter the 5 numbers for the array to be sorted, hitting enter after each number: 
        5
        6
        8
        2
        7
        Starting array: Array Values: 5 6 8 2 7 
        Sorted array: Array Values: 8 7 6 5 2 
        Minimum Value: 2 
        Median Value: 6 
        Maximum Value: 8

    Output of Second Test Case:

        --------------------- Program starting ---------------------

        Enter the 5 numbers for the array to be sorted, hitting enter after each number: 
        10
        9
        11
        5
        7
        Starting array: Array Values: 10 9 11 5 7 
        Sorted array: Array Values: 11 10 9 7 5 
        Minimum Value: 5 
        Median Value: 9 
        Maximum Value: 11 

----------------------------------
Additional Files Used in this Project
----------------------------------
None    