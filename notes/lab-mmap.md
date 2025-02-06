# Syscall mmap

## design

A vma array on each proc struct.

A vmastack for free vma allocation, when vma being deallocated, travel all vma in array to get new vmastack.

sys_mmap just add a new vma with duplicated file pointer.

mmap_handler in usertrap when `scause == 0xc | 0xd | 0xf` which is different type of page fault. Need to check the permission of vma and `scause`. At the time, the pte is actually updated.

readfilepage and writefilepage do what it look like. Think about the effect of f->ip->size.

uvmumap has a replica specified for mmap, which simply skip all invalid pte and write back to file when freeing pa.

sys_munmap has 4 conditions: whole, left, right, middle. When whole, simply free whole vma. left or right just trim left or right. When middle, it will be a hole on vma, so we simply add a new vma for that case.


## Attention

When read page from file, you need to fill 0 to the tail when meeting the tail of file. At this time, pay attention to not to overleap the PGSIZE, because it may cause a strange kalloc error and hard to find the reason, which is the next page pointer in the front of page was set to 0.