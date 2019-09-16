
 /* File create.c */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

char * removeNewline(char * string){
  int length = strlen(string);
  char last_char = string[length -1];
  if(last_char == '\n'){
  char *pos = string + strlen(string) - 1;
  *pos = '\0';
  }
return string; 
}

main()
{
  int i,recordsid, readid;
  int sema_set;
  int * read_count;
  FILE *fp1;
  char content;
  char *  line = NULL;
  size_t len = 0;
  struct StudentInfo *  records;
  
/*CREATE SHARED MEMORY TO STORE STUDENT RECORDS*/
  recordsid = shmget(KEY, SEGSIZE * MAX_STUDENTS,IPC_CREAT|0666);/* get shared memory to store data*/
  if (recordsid <0){
    perror("create: shmget failed");
    exit(1);
}
/*CREATE SHARED MEMORY TO STORE SHARED VARIABLE READ_COUNT*/
 readid = shmget(READ_KEY, sizeof(int), IPC_CREAT|0666);
 if (readid <0){
    perror("create: shmget failed");
    exit(1);
}

/*ATTACH MEMORY SEGMENT FOR READ_COUNT*/
read_count= (int *)shmat(readid,NULL,0);/*attach the shared memory segment
				       to the process's address space */
  if (read_count <= (int) (0)) {
    perror("create: shmat failed");
    exit(2);
}
records = (struct StudentInfo *)shmat(recordsid,NULL,0);/*attach the shared memory segment
				       to the process's address space */
  if (records <= (struct StudentInfo *) (0)) {
    perror("create: shmat failed");
    exit(2);
}

/*CREATE A SEMAPHORE SET*/
sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS
						   semaphores*/
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }

/*WAIT*/
Wait(sema_set, 0);

/*INITIALIZE READ_COUNT*/
*read_count = 0;

fp1 = fopen("./Sample.txt", "r");

for(int k = 0; k < MAX_STUDENTS; k++){
  struct StudentInfo *infoptr = (struct StudentInfo *) malloc(sizeof(struct StudentInfo));
     if (getline(&line, &len, fp1) != -1) {
/* LOAD SHARED MEMORY SEGMENT WITH DATA FROM THE FILE */
  line = removeNewline(line);  
  strcpy(infoptr->fName, line);
  getline(&line, &len, fp1);
//  int length = strlen(line);
//  char last_char = line[length -1];
//  if(last_char == '\n'){
//  char *pos = line + strlen(line) - 1;
//  *pos = '\0';
//  }
  line = removeNewline(line);  
  strcpy(infoptr->studentID, line);
  getline(&line, &len, fp1);
  line = removeNewline(line);
  strcpy(infoptr->address, line);
  getline(&line, &len, fp1);
  line = removeNewline(line);
  strcpy(infoptr->telNumber, line);
  strcpy(infoptr->whoModified, " ");
  records[k] = *infoptr;
  free(infoptr);
 } else{
	break;
 }
}

shmdt(records); /* detach the shared memory segment */
shmdt(read_count);
fclose(fp1);
printf("Load complete.\n"); 
  Signal(sema_set, 0); 
  exit(0);
}
