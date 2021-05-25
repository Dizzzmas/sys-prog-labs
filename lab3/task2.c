#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* pid_t, getpid(), frtuncate() */
#include <unistd.h> /* pid_t, getpid(), frtuncate() */
#include <time.h> /* struct tm, time_t, time(), localtime(), asctime() */
#include <sys/mman.h> /* mmap(), munmap(), msync() */
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */

/* Struct must be defined with no reference types (with no pointers)! */
/* Problems as Segmentation Fault and Bus Fault can occur in this case. */
struct datum_s {
	pid_t pid;
	struct tm cur_time;
	char buff[100];
};

typedef struct datum_s datum_t;

int main(void) {
  int fd;
  char buff[100];
  datum_t * datum = NULL;
  /* tmp variable for current time */
  time_t rawtime;

  /* Create shared memory object */
  fd = shm_open("/lab3", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  
  if (fd == -1) {
      printf("Error while trying to shm_open!");
      return -1;
  }
  
  /* Set its size (clear the needed space) */
  if (ftruncate(fd, sizeof(struct datum_s)) == -1) {
      printf("Error while trying to ftruncate fd from shp_open!");
      return -1;
  }
  
  /* Map shared memory object */
  datum = mmap(NULL, sizeof(struct datum_s), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (datum == MAP_FAILED) {
      printf("Error while trying to mmap() fd!");
      return -1;
  }

  /* Now the mapped region could be referred with datum,
  e.g. datum->buff */
  while(1) {
    printf("Please, enter some string:\n");
    /* 1. Read string from input to buff */
    fgets(buff, 100, stdin);
    /* 2. Read datum info */
    msync(datum, sizeof(struct datum_s), MS_SYNC);
    printf("DATUM (PID: %d):\nTIME: %sSTRING:%s\n\n",
           datum->pid, asctime(&(datum->cur_time)), datum->buff);
  
    /* 3. Write to datum */
    datum->pid = getpid();
    time(&rawtime);
    datum->cur_time = (* localtime(&rawtime));
    strcpy(datum->buff, buff);
  }
  
  /* free */
  munmap(datum, sizeof(struct datum_s));
  shm_unlink("/lab3");
  return 0;
}
