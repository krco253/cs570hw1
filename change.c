/*File change.c */

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

main(int argc, char* argv[])
{
  int i,recordsid, readid;
  struct StudentInfo *record;
  int * read_count;
  int sema_set;
//  if (argc == 0){
//    fprintf(stderr, "usage: ./change <u");
//    exit(3);
 // } 

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
 scanf("%s",password_entered);

 if(strcmp(password_entered,"000")!=0){
	printf("Incorrect password. Access denied.");
	exit(0);
 } 
 int loop = 0;
 while (loop == 0){
 /*GET STUDENT INFO*/
 printf("(a)dd student, (u)pdate student, or (d)elete student?\n");
 char * option;
 scanf("%s", option);
 Wait(sema_set, 0);
 switch(option[0]){
	case 'a': //code
		break;
	case 'u': //UPDATE
		printf("Please enter the student's ID: ");
		char s_id[10];
		scanf("%s", s_id);
		int found = -1;
  		struct StudentInfo *infoptr = (struct StudentInfo *) malloc(sizeof(struct StudentInfo));
 		for(int k = 0; k < MAX_STUDENTS; k++){
		if(strcmp(record[k].studentID, s_id) == 0){ 
			printf("\nName: %s \nPhone Number: %s\nStudent ID: %s\nAddress: %s\n", record[k].name,record[k].telNumber, record[k].studentID, record[k].address);
 			printf("Last modified by: %s\n \n ", record[k].whoModified);
			printf("Please enter updated information.\n");
			printf("Name? ");
			char new_name[40];
			fgets(new_name, 40, stdin);
			strcpy(infoptr->name,new_name);
		        printf("\nPhone number? ");
			char new_phone[15];
			fgets(new_phone, 15, stdin);
			strcpy(infoptr->telNumber, new_phone);
			printf("\nAddress? "); 
			char new_addr[64];
			fgets(new_addr, 64, stdin);
			strcpy(infoptr->address, new_addr);
			strcpy(infoptr->studentID, s_id);
		 	strcpy(infoptr->whoModified, (getpwuid(getuid()))->pw_name);
			record[k] = *infoptr;
			free(infoptr);
 			found = 0;
			break;
		}
 
		}

		if (found == -1){
		printf("Record not found.\n");
		}

		loop = -1;
		break;
	case 'd': //code
		break;
	default:
		printf("Please choose a valid option.\n");
 }
 }

/*  Wait(sema_set,1);
  printf("the value of sema_set=%d\n", sema_set); 
  strcpy(infoptr->name,argv[1]);
  strcpy(infoptr->lName, argv[2]);
  sleep(10);
  strcpy(infoptr->telNumber, argv[3]);
  strcpy(infoptr->whoModified, (getpwuid(getuid()))->pw_name);
  sleep(10);*/
  Signal(sema_set,0); 
  exit(0);

}

