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

/*GET THE ID OF THE SEMAPHORE SET*/
int sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS
*/	

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
/*GET THE ID OF THE QUERY STUDENT*/
char queried_id[10];
printf("Enter requested student's ID: ");
scanf("%s", queried_id);

Wait(sema_set,1);
*read_count++; 
if(*read_count == 1){
Wait(sema_set,0);
Signal(sema_set, 1);
}

int found = -1;
for(int k = 0; k < MAX_STUDENTS; k++){
	if(strcmp(record[k].studentID, queried_id) == 0){ 
	printf("\nName: %s \nPhone Number: %s\nStudent ID: %s\nAddress: %s\n", record[k].name,record[k].telNumber, record[k].studentID, record[k].address);
 printf("Last modified by: %s\n \n ", record[k].whoModified);
 found = 0;
}
 
}

if (found == -1){

printf("Record not found.\n");
}

  Wait(sema_set, 1);
  *read_count--;
  if(*read_count == 0){
	Signal(sema_set, 0);
  }
  Signal(sema_set, 1);
  shmdt(record);
  shmdt(read_count);
  shmctl(recordsid, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
  shmctl(readid, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
   semctl(sema_set,0,IPC_RMID); /*Remove the semaphore set */
return 0;
}
