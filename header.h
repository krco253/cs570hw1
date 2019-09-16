/* header.h */
#define KEY  ((key_t)(19716)) /*change it to last five digits of your SID*/
#define READ_KEY ((key_t)(10319)) 
#define SEGSIZE  sizeof(struct StudentInfo)

#define MAX_STUDENTS 50
#define NUM_SEMAPHS 5
#define SEMA_KEY   ((key_t)(9716)) /* change this to last four digits of SID */

struct StudentInfo{
  char name[40];
  char telNumber[15];
  char address[64];
  char studentID[10];
  char whoModified[10];
};

int read_count;

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);

