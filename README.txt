----------------------------------
Assignment 3 README 
----------------------------------
    Authors:
        Name: Ryan Boucher      Student #: 101049347
        Name: Simon Krol        Student #: 101047304

----------------------------------
Instructions on Running this Code
----------------------------------
    This code can be run by compiling and executing RecordKeeper.c, and Administrator.c. Messages passed into Administrator.c will be parsed and executed by RecordKeeper.c

----------------------------------
Algorithm to Send Message
----------------------------------
    The algorithm to send messages are as follows:
        1. The Record-Keeper runs constantly, and functions to receive messages from the Administrator
        2. The Administrator runs constantly, and functions to send messages to the record keeper
        3. The Administrator sends messages to the Record-Keeper as a string
        4. The Record-Keeper parses the message, determining what the requested function is, and what parameters have been passed to it.
        5. The Record-Keeper performs the function with given parameters


----------------------------------
Testing Discussion
----------------------------------


----------------------------------
Additional Files Used in this Project
----------------------------------
None
