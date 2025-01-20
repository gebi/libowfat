#include <stdio.h>

int main() {
  int p=0;
  for (int c=getchar(); c!=EOF; c=getchar()) {
    if (c=='\\') p=1; else
    if (c=='\n' && p==1); else
    if (c==' ' && p==1) p=0; else
    putchar(c);
  }
}
