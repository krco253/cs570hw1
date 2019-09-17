/*file clean.c*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

int main(){

struct StudentInfo * record;	
/*GET THE IDS OF THE TWO SHARED MEMORY SEGMENTS*/
int readid = shmget(READ_KEY, sizeof(int), 0);
int recordsid = shmget(KEY, MAX_STUDENTS * SEGSIZE, 0666);

/*ATTACH THE SHARED MEMORY SEGMENTS*/
/*ATTACH MEMORY SEGMENT FOR READ_COUNT*/
int * read_count= (int *)shmat(readid,NULL,0666);/*attach the shared memory segment
				       to the process's address space */
  if (read_count <= (int) (0)) {
    perror("create: shmat failed");
    exit(2);
}

/*ATTACH MEMORY SEGMENT FOR RECORDS*/
record = (struct StudentInfo *)shmat(recordsid, NULL,0);/*attach the shared memory segment
				       to the process's address space */

 if (record <= (int) (0)) {
    perror("create: shmat failed");
    exit(2);
    }


/*GET THE ID OF THE SEMAPHORE SET*/
int sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); 

/*PROMPT THE USER FOR PASSWORD*/
 printf("Password: ");
 char password_entered[15];
 fgets(password_entered, 15, stdin);

/*VALIDATE PASSWORD*/
 if(strcmp(password_entered,"000\n")!=0){
	printf("Incorrect password. Access denied.");
	exit(0);
 } 
/*WAIT*/
Wait(sema_set,0);
/*WRITE OUTPUT FILE*/
FILE *out_file = fopen("student_records.txt","w");
if (out_file == NULL) {   
printf("Error! Could not write file\n"); 
   return -1;  
}

int k = 0;
//for(int i = 0; i < MAX_STUDENTS; i++){
while(strcmp(record[k].name,"")){
   fprintf(out_file,"%s\n%s\n%s\n%s\n",
	     record[k].name,record[k].studentID, record[k].address, record[k].telNumber);
 k++;		
}	
//}
fclose(out_file);
/*DELETE SHARED MEMORY SEGMENTS*/
  shmdt(record);
  shmdt(read_count);
  shmctl(recordsid, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
  shmctl(readid, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/

/*SLEEP*/
sleep(10);

/*SIGNAL*/
Signal(sema_set, 0);

/*DELETE THE SEMAPHORES*/
  semctl(sema_set,0,IPC_RMID); /*Remove the semaphore set */
  return 0;
}
