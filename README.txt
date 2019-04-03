----------------------------------
Assignment 3 README
----------------------------------
    Authors:
        Name: Ryan Boucher      Student #: 101049347
        Name: Simon Krol        Student #: 101047304

----------------------------------
Instructions on Running this Code
----------------------------------
    The code can be compiled through the Makefile by running "make". Each of the RecordKeeper and the Administrator should be run on different terminal windows by using ./Record and ./Admin respectively.

    To Test the program, any of the following commands can be entered:
    Insert(name, dept_name, emp_number, salary)
    Check_name(emp_number)
    Check_department(emp_number)
    Check_salary(emp_number)
    Check_employee_number(name)
    Check(dept_name)
    Delete(emp_number)

    Alternatively, you can copy/paste the test cases provided at the bottom of Testing.txt. You can paste them all at once just keep in mind that you'll need to press enter once to submit the last request and that the formatting will look strange due to all the inputs coming in before any of the outputs.

    Some things to keep in mind when testing:
        1.Capital Letters are important in both the function name (check_name won't work, Check_name will) as well as the Department and Employee names. The Check and Check_employee_number functions require that the parameters are properly capitalized.
        2. Leading and Trailing whitespace is ignored in command names as well as in Department and Employee names. Insert ( Patrick Star , Rock , 25, 15) is equivalent to Insert(Patrick Star,Rock,25,15)
        3. Quotation marks should NOT be included around Strings. They will cause issues with the string parsing


----------------------------------
Additional Files Used in this Project
----------------------------------
cliserv.h -- This is a header file that contains the definition for our message queue struct
