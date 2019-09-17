#Kelsey Cole
#CS 570 Assignment 1
#Student ID: 912109716

##To compile
run make
##load
Usage: ./load
Will prompt you for a file path. Relative path is fine.
Loads these records into shared memory.
Records should be in format:
Name
Student ID
Address
Telephone
as they are in the Sample.txt

##query
Usage: ./query
Will prompt you for a student ID to search for in the database. Returns error message if not found.

##print
Usage: ./print
Prints all records in shared memory.

##change
Usage: ./change
Will prompt if a user wants to (a)dd, (u)pdate, or (d)elete a record (and they are expected to enter the corresponding character). Will keep prompting until uer gives valid option (a, u, or d).

##clean
Usage: ./clean
Asks for a password, prints the contents of shared memory into a file called "student_records.txt",then deletes shared memory. 
