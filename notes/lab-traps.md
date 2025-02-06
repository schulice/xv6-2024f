---
date: 2025-01-16T00:48:41+08:00
aliases: []
tags: []
---

# EZ Riscv

Just answer problem. Functions were all inline optimized.

# Backtrace

Simply read the fp from (fp - 16) and print return address in (fp - 8).

# Alarm

new struct alarmcom to save

```txt
ticks
passed
handler
calling
..registers
```

remember to create new page on `allocproc()` and `freeproc()`.

change `p->trapframe->epc` to change userspace pc.

save all userspace register because time interrupt may be happened anytime, also when function calling.

`sys_sigreturn()` need to return a0 because `syscall()` will reassign `trapframe->a0` from return value.

# Problem

1. Which registers contain arguments to functions? For example, which register holds 13 in main's call to printf?

	normally on a1->a7, also named x11 -> x17, 13 on a2.

1. Where is the call to function f in the assembly code for main? Where is the call to g? (Hint: the compiler may inline functions.)

	60:	45b1                	li	a1,12

	do not call the f but inline, also happened on g inside f.

1. At what address is the function printf located?

	00000000000006e2


1. What value is in the register ra just after the jalr to printf in main?

	0x6da

1. Run the following code.

```c
	unsigned int i = 0x00646c72;
	printf("H%x Wo%s", 57616, (char *) &i);
```

What is the output? [Here's an ASCII table](https://www.asciitable.com/) that maps bytes to characters.

The output depends on that fact that the RISC-V is little-endian. If the RISC-V were instead big-endian what would you set `i` to in order to yield the same output? Would you need to change `57616` to a different value?

[Here's a description of littleand big-endian](http://www.webopedia.com/TERM/b/big_endian.html) and [a more whimsical description](https://www.rfc-editor.org/ien/ien137.txt).

He110 World. Do not need change because this is a number param to function.

6. In the following code, what is going to be printed after `'y='`? (note: the answer is not a specific value.) Why does this happen?

```c
	printf("x=%d y=%d", 3);
```

The possible result maybe the numbers in a2 because value because function get the value from the a2 register. Or compiler error.
