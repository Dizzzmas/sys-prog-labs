#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#define BUFFER 1025

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Need one argument: string id\n");
    exit(1);
  }
  char *label = argv[1];
  struct pollfd fds[1];
  int val, len;
  char buff[BUFFER] = {0};

  while (1)
  {
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    val = poll(fds, 1, 5000);
    if (val == -1)
    {
      printf("%s, error at poll, %s", label, strerror(errno));
      exit(EXIT_FAILURE);
    }
    else if (val)
    {
      ssize_t readBytes = read(STDIN_FILENO, buff, BUFFER);
      len = readBytes - 1;
      if (buff[len] == '\n')
        buff[len] = '\0';
      printf("\"%s\"\nGot: \"%s\"\n", label, buff);
      break;
    }
    else
    {
      printf("%s: 5 seconds passed but got no input, retrying...\n", label);
    }
  }
  return 0;
}
