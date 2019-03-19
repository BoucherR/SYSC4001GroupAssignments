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
        1. Two of the semaphores are attempted to have their locks taken by a process
        2. Once both locks are taken by a process, it performs the switch
        3. The process releases both of the locks
        4. A slight wait occurs to help the process go smoothers

----------------------------------
Fork Pseudo-Code:
----------------------------------

    if (pid == 0) { // it's a child
        while(!isSorted(Q, 5)) { // exit when the array is sorted

            if (!semaphore_p(sem[forkVal])) exit(EXIT_FAILURE); // attempting to get first lock
            if (!semaphore_p(sem[forkVal+1])) exit(EXIT_FAILURE); // attempting to get second lock
            orderNumerically(Q, 2, forkVal, isDebug); // swapping if needed
            if (!semaphore_v(sem[forkVal+1])) exit(EXIT_FAILURE); // releasing lock
            if (!semaphore_v(sem[forkVal])) exit(EXIT_FAILURE); // releasing lock 
            for (int i =0; i< 10000000; i++); // wait 
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