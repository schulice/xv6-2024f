#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ping[2];
  int pong[2];
  pipe(ping);
  pipe(pong);
  int pid = fork();
  if (pid > 0) {
    close(pong[1]);
    close(ping[0]);
    write(ping[1], "p", 1);
    close(ping[1]);
    for (;;) {
      char buf[1]; 
      if (read(pong[0], buf, 1) < 0) {
        continue;
      }
      if (strcmp(buf, "c") != 0) {
        close(pong[0]);
        fprintf(2, "parent read error message.\n");
        exit(0);
      } else {
        break;
      }
    }
    close(pong[0]);
    pid = getpid(); 
    fprintf(1, "%d: received pong\n", pid);
    wait(0);
  } else if (pid == 0){
    close(ping[1]);
    close(pong[0]);
    char buf[1];
    for (;;) {
      if (read(ping[0], buf, 1) < 0) {
        continue;
      }
      if (strcmp(buf, "p") != 0) {
        close(ping[0]);
        close(pong[1]);
        fprintf(2, "child read error message.\n");
        exit(0);
      } else {
        break;
      }
    }
    close(ping[0]);
    pid = getpid(); 
    fprintf(1, "%d: received ping\n", pid);
    write(pong[1], "c", 1);
    close(pong[1]);
  } else {
    close(ping[0]);
    close(ping[1]);
    close(pong[0]);
    close(pong[1]);
    fprintf(2, "fork recieved error\n");
  }
  exit(0);
}
