---
date: 2025-01-09T02:35:28+08:00
aliases: []
tags: []
---
# Trace



# Attack XV6

fork -> just copy what parent vm usage, but 2 page for new proc: proc struct and trapframe.

exec -> read ELF memory usage then alloc, 2 page for isolate and userstack.

after exit, parent mapping page was released.

ELF memory usage: attacktest -> 0x1070 as 2 page, attack -> 0x1020 as 2 page.

So totally use 2 + 2 + 2 = 6.

secret alloc 32 and write to 10, freelist is a link stack. so we just read the `32 - 10 + 1 - 6` block is the secret block.

# Problem

1. Looking at the backtrace output, which function called syscall?

`usertrap()` in `user/trap.c`

2. What is the value of `p->trapframe->a7` and what does that value represent? (Hint: look `user/initcode.S`, the first user program xv6 starts.)

0x7, the number of syscall id, when 0x7, it is exec syscall.

3. What was the previous mode that the CPU was in?

See the `spp` bit on `sstatus`, which announce the trap from user or supervisor

4. Write down the assembly instruction the kernel is panicing at. Which register corresponds to the variable num?

```assembly
lw a3, 0(zero)
a3 
```

5.`user/secret.c` copies the secret bytes to memory whose address is 32 bytes after the start of a page. Change the 32 to 0 and you should see that your attack doesn't work anymore; why not?

`struct kmem` will use 32 bytes for hold the spinlock and next ptr, though it is 28 bytes actually, but pending may need to take 32 bytes.
