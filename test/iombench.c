#include <libowfat/io.h>
#include <stdio.h>
#if !defined(PTHREAD) && (defined(__dietlibc__) || defined(__linux__))
#define THRD
#include <threads.h>
#else
#include <pthread.h>
#endif
#include <unistd.h>
#include <string.h>

iomux_t c;

int worker(void* arg) {
  uintptr_t i=(uintptr_t)arg;
  char buf[100];
  int64 s;
  unsigned int events;
  int n=0;

  for (;;) {
    switch (iom_wait(&c,&s,&events,1000)) {
    case -1:
      perror("iom_wait");
      return -1;
    case 0:
      write(1,buf,sprintf(buf,"timeout in thread %ld\n",i));
      return 0;
    case 1:
      break;
    }
    switch (read(s,buf,1)) {
    case -1:
      perror("read");
      return -1;
    case 0:
      return n;
    }
//    write(1,".",1);
    ++n;
    if (iom_requeue(&c,s,IOM_READ)==-1) {
      perror("iom_requeue");
      return 1;
    }
  }

  return 0;
}

#ifndef THRD
void* workerwrapper(void* arg) {
  return (void*)(uintptr_t)worker(arg);
}
#endif

enum { nfd=10, nthrd=4 };

int main() {
  int i;
  int fd[2*nfd];
  for (i=0; i<nfd; ++i) {
    if (pipe(fd+i*2)==-1) {
      perror("pipe");
      return 111;
    }
  }
  iom_init(&c);
  for (i=0; i<nfd; ++i) {
    if (iom_add(&c,fd[i*2],IOM_READ)==-1) {
      perror("iom_add");
      return 1;
    }
  }
#ifdef THRD
  thrd_t x[nthrd];
#else
  pthread_t x[nthrd];
#endif
  puts("launching threads");
  for (i=0; i<nthrd; ++i)
#ifdef THRD
    if (thrd_create(&x[i],worker,(void*)(uintptr_t)i)==-1)
#else
    if (pthread_create(&x[i],0,workerwrapper,(void*)(uintptr_t)i)==-1)
#endif
    {
      perror("thrd_create");
      return 111;
    }

  // now write data to pipe
  char spaces[512];
  memset(spaces,' ',sizeof spaces);
  for (i=0; i<1000; ++i) {
    int j;
    for (j=0; j<nfd; ++j) {
      write(fd[j*2+1],spaces,sizeof spaces);
    }
  }
  for (i=0; i<nfd; ++i)
    close(fd[i*2+1]);
//  sleep(1);
//  iom_abort(&c);

  puts("joining threads");
  int r;
  for (i=0; i<nthrd; ++i) {
#ifdef THRD
    if (thrd_join(x[i],&r)==-1)
#else
    void* tmp;
    
    if (pthread_join(x[i],&tmp)!=-1)
      r=(int)(uintptr_t)tmp;
    else
#endif
    {
      perror("thrd_join");
      return 111;
    }
    printf("thread %d returned %d\n",i,r);
  }

  fflush(stdout);

  return 0;
}
