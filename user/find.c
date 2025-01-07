#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
basename(char *path) {
  char* p; 
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char *path, char* target) {
  int fd;
  struct dirent de; 
  struct stat st;
  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: can not open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: can not get fstat %s\n", path);
    close(fd);
    return;
  }
  if (st.type == T_FILE) {
    if (strcmp(basename(path), target) == 0){
      printf("%s\n", path);
    } 
  } else if (st.type == T_DIR){
    char buf[512], *p;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      fprintf(2, "find: path too long\n");
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0)
        continue;
      if (strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, target);
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "ERROR: Do not have enough params.\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
