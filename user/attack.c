#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  // char* current = sbrk(PGSIZE * 32);
  // for (int i = 0; i < PGSIZE * 32; i += PGSIZE) {
  //   *(current + i + 32 + 8) = 0;
  //   printf("block %d content: %s\n", i / PGSIZE, current + i + 32);
  // }
  //
  //
  // 32 - 9 - 2 - 2 - 2 = 17
  // ELF - 2, struct proc, trapframe, barrier, stack

  int blkpos = 17;
  char *end = sbrk(PGSIZE*blkpos);
  end = end + (blkpos - 1) * PGSIZE;
  write(2, end + 32, 8);
  
  exit(1);
}
