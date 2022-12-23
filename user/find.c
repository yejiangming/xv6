#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  memcpy(buf, p, strlen(p));
  buf[strlen(p)] = 0;

  return buf;
}

void find(char* cur_file, char* target_file) {
    int fd = open(cur_file, 0);
    if (fd < 0) {
        fprintf(2, "can't open %s\n", cur_file);
        exit(1);
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        fprintf(2, "can't fstat %s\n", cur_file);
        exit(1);
    }

    switch (st.type)
    {
    case T_FILE:
        if (strcmp(fmtname(cur_file), target_file) == 0) {
            fprintf(1, "%s\n", cur_file); 
        }
        break;
    case T_DIR:
        struct dirent de;
        char buf[50];
        strcpy(buf, cur_file);
        char *p = buf + strlen(buf);
        *(p++) = '/';

        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            
            // fprintf(1, "de.name:%s\n", de.name);
            if (de.inum == 0) {
                continue;;
            }
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, target_file);
        }
        break;
    default:
        break;
    }

    close(fd);
    return;
}

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "usage: find target_dir target_file\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
