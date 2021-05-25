#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define BUFFER 512

void read_tolower_write(int rfd, int wfd);
int read_file(const char *filename, unsigned char is_ex);
int write_file(const char *filename, unsigned char is_ex);

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Wrong argument count.");
    exit(1);
  }

  printf("Reading file %s and writing to %s\n", argv[1], argv[2]);
  int rfd = read_file(argv[1], 1);
  printf("read file descriptor is %d", rfd);
  int wfd = write_file(argv[2], 1);
  printf("write file descriptor is %d\n", wfd);
  read_tolower_write(rfd, wfd);
  close(rfd);
  close(wfd);
  return EXIT_SUCCESS;
}

void read_tolower_write(int rfd, int wfd)
{
  char rbuf[BUFFER] = {0}, totalbuf[BUFFER] = {0};
  ssize_t curbytes, curwrotebytes;
  ssize_t totalreadbytes = 0, totalwrotebytes = 0;
  do
  {
    curbytes = read(rfd, &rbuf, (size_t)BUFFER);
    if (curbytes < 0)
    {
      fprintf(stderr, "Can't read file descriptor %d, Errno: %d, Str: %s\n", rfd, errno, strerror(errno));
      return;
    }
    if (curbytes == 0)
    {
      break;
    }
    for (ssize_t i = 0; i < curbytes; i++)
    {
      rbuf[i] = tolower(rbuf[i]);
    }
    curwrotebytes = write(wfd, &rbuf, (size_t)curbytes);
    if (curwrotebytes < 0)
    {
      fprintf(stderr, "Can't write data to descriptor %d, Errno: %d, Str: %s\n", wfd, errno, strerror(errno));
      return;
    }
    if (curwrotebytes != curbytes)
    {
      fprintf(stderr, "Bytes read and written are different");
      return;
    }
    totalreadbytes += curbytes;
    totalwrotebytes += curwrotebytes;
  } while (curbytes > 0);

  sprintf(totalbuf, "Bytes wrote: %ld, bytes read: %ld", totalwrotebytes, totalreadbytes);
  curwrotebytes = write(wfd, &totalbuf, strlen(totalbuf));
  if (curwrotebytes < 0)
  {
    fprintf(stderr, "Can't display statistics, Errno: %d, Str:[%s]\n", errno, strerror(errno));
  }
  printf("Total bytes wrote and read: %s", totalbuf);
}

int read_file(const char *filename, unsigned char is_ex)
{
  const int flags = O_RDONLY;
  int file;
  file = open(filename, flags);
  if (file == -1)
  {
    fprintf(stderr, "Can't open file %s to read, Errno: %d, Str: %s\n", filename, errno, strerror(errno));
    if (is_ex)
    {
      exit(EXIT_FAILURE);
    }
  }
  return file;
}

int write_file(const char *filename, unsigned char is_ex)
{
  const int flags = O_WRONLY | O_CREAT | O_EXCL;
  const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  int file;
  file = open(filename, flags, mode);
  if (file == -1)
  {
    fprintf(stderr, "Can't create or write to file %s, Errno: %d, Str: %s\n", filename, errno, strerror(errno));
    if (is_ex)
    {
      exit(EXIT_FAILURE);
    }
  }
  return file;
}
