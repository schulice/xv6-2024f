#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPRIO 6
// xv6 ticket is 100ms
#define MAX_TIME 40

int prios[NPRIO] = {5, 6, 7, 8, 9, 10};

void
spin_delay()
{
  for(int i = 0; i < 10; i++)
    __asm__ __volatile__ ("nop" :::);
}

int
count_during(int prio)
{
  int start_time = uptime();
  int acc = 0;
  setprio(prio);
  for(;;){
    spin_delay();
    acc += 1;
    if(acc % 400 == 0){
      int current = uptime() - start_time;
      if(current > MAX_TIME){
        return acc;
      }
    }
  }
}

void
stridetest(void)
{
  int pids[NPRIO];
  int i;
  setprio(4);
  for(i = 0; i < NPRIO; i++){
    if((pids[i] = fork()) == 0){
      int cnt = count_during(prios[i]);
      printf("child%d: spinning times is %d\n", i, cnt);
      exit(0);
    }
  }
  for(i = 0; i < NPRIO; i++){
    int cpid;
    int status;
    if((cpid = wait(&status)) < 0){
      printf("ERROR: wait error at child %d\n", i);
      exit(1);
    }
    if(pids[i] != cpid){
      printf("ERROR: order broken at child%d\n", i);
      exit(1);
    }
  }
  printf("OK: stride scheduler test PASS\n");
}

void
setpriotest(void)
{
  if(setprio(1) == 0){
    printf("ERROR: set 1 prio shoulb be reject\n");
    exit(1);
  }
  if(setprio(400) < 0){
    printf("ERROR: set normal prio error\n");
    exit(1);
  }
  // prio work
  spin_delay();
  if(setprio(U64_MAX) < 0){
    printf("ERROR: set MAX prio error\n");
    exit(1);
  }
}

int
main(void)
{
  setpriotest();
  stridetest();
  exit(0);
}

