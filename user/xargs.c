#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
nextline(char *p)
{
  while (read(0, p, 1) == 1 && *p++ != '\n')
    ;
  *p-- = 0;
  return p;
}

int
split(char** out, char* buf)
{
  int cnt = 0;
  char *prev;
  prev = buf;
  while (*buf) {
    while (*buf != ' ' && *buf != '\n')
      buf++;
    out[cnt++] = prev;
    *buf++ = 0;
    prev = buf;
  }
  out[cnt] = 0; 
  return cnt;
}

int
main(int argc, char *argv[])
{
  char buf[512];
  char* nargv[MAXARG];
  for (int i = 0; i < argc - 1; i++)
    nargv[i] = argv[i + 1];
  while (nextline(buf) - buf > 0) {
    split(nargv + argc - 1, buf);
    if (fork() == 0) {
      exec(nargv[0], nargv);
      break;
    }
    wait(0);
  }
  exit(0);
}
