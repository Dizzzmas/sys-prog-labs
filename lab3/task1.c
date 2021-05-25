#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>


void signal_handler(int signo, siginfo_t * si, void * ucontext);


int FP;


int main(void) {
  char buff[100];
  int counter = 0;
  int bytes_written = 0;
  // "static" modifier to init each member of struct with 0 
  static struct sigaction oa;
  static struct sigaction na;

  // writing info about process to test.log
  bytes_written = sprintf(buff, "PID: %d\nStarted working...\n", (int)getpid());
  FP = open("test.log", O_CREAT | O_RDWR | O_TRUNC, 0644);
  write(FP, buff, bytes_written);
  close(FP);
  
  // save the old handler (use it further if necessary)
  sigaction(SIGHUP, NULL, &oa);
  // assign the new handler
  na.sa_sigaction = signal_handler;
  // set all signals to be blocked when processing incoming signals
  // sigfillset(&(na.sa_mask));
  // if sa_flags is set to SA_SIGINFO,
  // then the "sa_sigaction" field is used instead of "sa_handler"
  // and to "siginfo_t * si" the additional info is passed
  na.sa_flags = SA_SIGINFO;
  // set new action for SIGHUP signal
  sigaction(SIGHUP, &na, NULL);
  while(1) {
    printf("Inside infinite loop (%d)\n", counter);
    FP = open("test.log", O_CREAT | O_RDWR | O_APPEND, 0644);
    write(FP, "Sleeping...\n", sizeof("Sleeping...\n") - 1);
    close(FP);
    sleep(3);
    counter++;
  }

  return 0;
}

void signal_handler(int signo, siginfo_t *si, void * ucontext) {
  char buff[] =
  "si_signo:\n"
  "\tSignal number being delivered. This field is always set.\n"
  "si_code:\n"
  "\tSignal code. This field is always set. Refer to Signal Codes for information on valid settings, and for which of the remaining fields are valid for each code.\n"
  "si_value:\n"
  "\tSignal value.\n"
  "si_errno:\n"
  "\tIf non-zero, an errno value associated with this signal.\n"
  "si_pid:\n"
  "\tProcess ID of sending process.\n"
  "si_uid:\n"
  "\tReal user ID of sending process.\n"
  "si_addr:\n"
  "\tAddress at which fault occurred.\n"
  "si_status:\n"
  "\tExit value or signal for process termination.\n"
  "si_band:\n"
  "\tBand event for SIGPOLL/SIGIO.\n"
  "st_mtime:\n"
  "\tTime of last data modification.\n\n";
  FP = open("test.log", O_CREAT | O_RDWR | O_APPEND, 0644);
  
  
  write(FP, buff, sizeof(buff) - 1);
  close(FP);
}
