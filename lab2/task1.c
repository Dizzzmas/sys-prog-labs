#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COUNTER 150

void printIds(void);
void parentProcess(void);
void childProcess(void);

int main(void) {
  pid_t pid, gid, sid, forkId;
  pid = getpid();
  gid = getgid();
  sid = getsid(pid);
  if (sid == -1) {
    printf("Couldn't get sid of process.");
  }
  printf("Initial: PID: %d, GID: %d, SID: %d\n", pid, gid, sid);
  forkId = fork();
  if (forkId < 0) {
    printf("Couldn't fork()");
  }
  else if (forkId == 0) {
    childProcess();
    printf("\nChild process ended\n");
  }
  else {
    parentProcess();
    printf("\nParent process ended\n");
  }
  return 0;
}

void printIds(void) {
  int i;
  for (i = 0; i < COUNTER; i++) {
    pid_t pid, gid, sid;
    pid = getpid();
    gid = getgid();
    sid = getsid(pid);
    printf("ITERATION: %d; PID: %d, GID: %d, SID: %d\n", i, pid, gid, sid);
  }
}

void parentProcess(void) {
  int waitRes;
  printIds();
  wait(&waitRes);
}

void childProcess(void) {
  printIds();
}

