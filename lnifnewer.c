#define _GNU_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc,char* argv[]) {
  if (argc<1) {
    puts("usage: lnifnewer compiler.h");
    return 1;
  }
  mkdir("libowfat",0755);	// don't care if it fails
  int i;
  char buf[128];
  for (i=1; i<argc; ++i) {
    if (strlen(argv[i])>100) {
      puts("name too long");
    }
    stpcpy(stpcpy(buf,"libowfat/"),argv[i]);
    struct stat a,b;
    if (stat(argv[i],&a)==0 && stat(buf,&b)==0 && a.st_mtime >= b.st_mtime)
      continue;
    if (link(argv[i],buf)==-1 && errno!=EEXIST) {
      perror(argv[i]);
      return 111;
    }
  }
}
