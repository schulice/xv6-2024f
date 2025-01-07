#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
primes(int* left)
{
  close(left[1]);
  int current;
  if (read(left[0], &current, 4) == 0) {
      close(left[0]);
      return;
  }
  fprintf(1, "prime %d\n", current);
  int right[2];
  pipe(right);
  int pid = fork();
  if (pid == 0) {
    close(left[0]);
    primes(right);
    return;
  }
  // parrent
  close(right[0]);
  int num;
  for (;;) {
    if (read(left[0], &num, 4) == 0) {
      close(left[0]); 
      break;
    }
    if (num % current) {
      write(right[1], &num, 4);
    }
  }
  close(right[1]);
  wait(0);
  return;
}

int
main(int argc, char *argv[])
{
  int src[2];
  pipe(src);
  int pid = fork();
  if (pid == 0) {
    primes(src);
  } else {
    close(src[0]);
    for (int i = 2; i <= 280; i++) {
      write(src[1], &i, 4); 
    }
    close(src[1]);
    wait(0);
  }
  exit(0);
}

