#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER 1025
#define SEC 5

int main(int argc, char *argv[])
{
  char buffer[BUFFER + 1];
  char *str_id;
  int sel;
  struct timeval tv;
  fd_set fds;
  if (argc < 2)
  {
    printf("Need one argument: string id\n");
    return -1;
  }
  str_id = argv[1];
  while (1)
  {
    tv.tv_sec = SEC;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    sel = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (sel == -1)
    {
      printf("Couldn't call select");
      return -1;
    }
    else if (sel == 0)
    {
      printf("%s\n%d seconds passed but got no input, retrying...\n", str_id, SEC);
      continue;
    }
    else
    {
      read(STDIN_FILENO, &buffer, BUFFER);
      buffer[BUFFER] = '\0';
      printf("\"%s\"\nGot: \"%s\"\n", str_id, buffer);
      break;
    }
  }
  return EXIT_SUCCESS;
}
