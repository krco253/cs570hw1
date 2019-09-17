/*------------------------
 * File: change.c 
 * Purpose: allow users to add, update, and remove
 * entries from the database
 * -----------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pwd.h>
#include "header.h"

/*------------------------*/
/*|Function: removeNewline|*/
/*----------------------- */
/*purpose: removes trailing
 * newline characters from fgets*/

char * removeNewline(char * string){
  int length = strlen(string);
  char last_char = string[length -1];
  if(last_char == '\n'){
  char *pos = string + strlen(string) - 1;
  *pos = '\0';
  }
return string; 
}

/*------------------------*/
/*| Function: main       |*/
/*------------------------*/
main()
{
  int i,recordsid, readid;
  struct StudentInfo *record;
  int * read_count;
  int sema_set;
/* get the id of the shared memory segment with key "KEY" */
/* note that this is the segment where the data is stored */
  recordsid = shmget(KEY,SEGSIZE * MAX_STUDENTS, 0);
  if (recordsid <0){
    perror("change: shmget failed 1");
    exit(1);
  }
  readid = shmget(READ_KEY,sizeof(int), 0);
  if (readid <0){
    perror("change: shmget failed 1");
    exit(1);
  }

/* attach the already created shared memory segment to infoptr so the
   shared memory segment can be accessed through 'inforptr'
   */
  record=(struct StudentInfo *)shmat(recordsid,0,0);
  if (record <= (struct StudentInfo *) (0)) {
    perror("change: shmat failed");
    exit(2);
  }
  read_count =(int *)shmat(readid,0,0);
  if (read_count <= (int *) (0)) {
    perror("change: shmat failed");
    exit(2);
  }

/* get the id of the semaphore set associated with SEMA_KEY */ 
  sema_set=semget(SEMA_KEY, 0,0);

 /*GET PASSWORD*/

 printf("Password: ");
 char password_entered[15];
 fgets(password_entered, 15, stdin);

 if(strcmp(password_entered,"000\n")!=0){
	printf("Incorrect password. Access denied.");
	exit(0);
 } 
 int loop = 0;
 while (loop == 0){
 /*GET STUDENT INFO*/
 printf("(a)dd student, (u)pdate student, or (d)elete student?\n");
 char option[10];
 fgets(option, 10, stdin);
 /*WAIT*/
 Wait(sema_set, 0);

 /*UPDATE, DELETE, ADD*/
 switch(option[0]){//determine the appropriate action to perform and perform it
	case 'a': //ADD
		printf("Please enter the student's ID to add: "); //get student ID
		char new_student_id[10];
		fgets(new_student_id, 10, stdin);
  		struct StudentInfo *infopr = (struct StudentInfo *) malloc(sizeof(struct StudentInfo));
 		for(int k = 0; k < MAX_STUDENTS; k++){ //look for the beginning of shared memory that has not been allocated
		if(strcmp(record[k].studentID, "") == 0){ //if the studentID is empty, this is an empty record we can store in
		       	//get new info
			printf("Name? ");
			char new_student_name[40];
			fgets(new_student_name, 40, stdin);
			strcpy(new_student_name,removeNewline(new_student_name));
			strcpy(infopr->name,new_student_name);
		        printf("Phone number? ");
			char new_tel[15];
			fgets(new_tel, 15, stdin);
			strcpy(new_tel,removeNewline(new_tel));			
			strcpy(infopr->telNumber, new_tel);
			printf("Address? "); 
			char new_address[64];
			fgets(new_address, 64, stdin);
			strcpy(new_address,removeNewline(new_address));//remove trailing newline
			strcpy(infopr->address, new_address);
			strcpy(new_student_id, removeNewline(new_student_id));
			strcpy(infopr->studentID, new_student_id);
		 	strcpy(infopr->whoModified, (getpwuid(getuid()))->pw_name);//update modified by
			record[k] = *infopr; //save the info into shared memory
			free(infopr);

			printf("\nNew Student Added\nName: %s \nPhone Number: %s\nStudent ID: %s\nAddress: %s\n", record[k].name,record[k].telNumber, record[k].studentID, record[k].address);
 			printf("Last modified by: %s\n \n ", record[k].whoModified);//print new record
			
			break;
		}
 
		}
		loop = -1;//break out of outer input validation loop		
		break;
	case 'u': //UPDATE
		/*GET THE STUDENT ID*/
		printf("Please enter the student's ID: ");
		char s_id[10];
		fgets(s_id, 10, stdin);
		strcpy(s_id,removeNewline(s_id));
		int found = -1;
  		struct StudentInfo *infoptr = (struct StudentInfo *) malloc(sizeof(struct StudentInfo));
 		for(int k = 0; k < MAX_STUDENTS; k++){ //find the student
		if(strcmp(record[k].studentID, s_id) == 0){  //found student
			//print current info
			printf("\nName: %s \nPhone Number: %s\nStudent ID: %s\nAddress: %s\n", record[k].name,record[k].telNumber, record[k].studentID, record[k].address);
 			printf("Last modified by: %s\n \n ", record[k].whoModified);
			//get new info
			printf("Name? ");
			char new_name[40];
			fgetc(stdin);
			fgets(new_name, 40, stdin);
			strcpy(new_name,removeNewline(new_name));
			strcpy(infoptr->name,new_name);
		        printf("Phone number? ");
			char new_phone[15];
			fgets(new_phone, 15, stdin);
			strcpy(new_phone,removeNewline(new_phone));			
			strcpy(infoptr->telNumber, new_phone);
			printf("Address? "); 
			char new_addr[64];
			fgets(new_addr, 64, stdin);
			strcpy(new_addr,removeNewline(new_addr));
			strcpy(infoptr->address, new_addr);
			strcpy(infoptr->studentID, s_id);
		 	strcpy(infoptr->whoModified, (getpwuid(getuid()))->pw_name);
			record[k] = *infoptr; //save the info into shared memory
			free(infoptr);
 			found = 0;
			break;
		}
 
		}
		if (found == -1){
		printf("Record not found.\n");
		}

		loop = -1;//break out of larger input validation loop
		break;
	case 'd': //DELETE
		printf("Please enter the student's ID to delete: ");
		char student_id[10];
		fgets(student_id, 10, stdin);
		strcpy(student_id,removeNewline(student_id));
		int found_delete = -1;//flag to track if record was found
  		struct StudentInfo *infptr = (struct StudentInfo *) malloc(sizeof(struct StudentInfo));
 		for(int k = 0; k < MAX_STUDENTS; k++){
		if(strcmp(record[k].studentID, student_id) == 0){ //record found
			printf("\nName: %s \nPhone Number: %s\nStudent ID: %s\nAddress: %s\n", record[k].name,record[k].telNumber, record[k].studentID, record[k].address);
 			printf("Last modified by: %s\n \n ", record[k].whoModified);
			while(strcmp(record[k].name, "") != 0){//move all records 'back' a spot in memory
			strcpy(infptr->name,record[k+1].name);
			strcpy(infptr->telNumber, record[k+1].telNumber);
			strcpy(infptr->address, record[k+1].address);
			strcpy(infptr->studentID, record[k+1].studentID);
		 	strcpy(infptr->whoModified, record[k+1].whoModified);
					
			record[k] = *infptr;
			k++;
			}
			free(infptr);
 			found_delete = 0;
			printf("Student deleted.\n");
			break;
		}
 
		}

		if (found_delete == -1){
		printf("Record not found.\n");
		}

		loop = -1;
		break;
	default:
		printf("Please choose a valid option.\n");
 }
 }
/*SLEEP*/
  sleep(10);
  /*SIGNAL*/
  Signal(sema_set,0); 
  exit(0);

}

