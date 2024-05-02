#include <dlfcn.h>
#include <execinfo.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int cur_usage = 0;
size_t time_base = 0;

char buf[255];

bool allow_record = true;

size_t get_cur_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  size_t time_stamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  return time_stamp;
}

void *malloc(size_t size) {
  static int flag = 0;
  static void *(*malloc_cp)(unsigned long);

  if (flag == 0) {
    time_base = get_cur_ms();
    malloc_cp = (void *(*)(unsigned long))dlsym(RTLD_NEXT, "malloc");
    char *err;
    if ((err = dlerror()) != NULL) {
      _exit(1);
    }
    flag = 1;
  }

  void *ptr = malloc_cp(size);

  if (allow_record == true) {
    allow_record = false;
    void *callstack[128];
    int frames = backtrace(callstack, 128);
    char **symbols = backtrace_symbols(callstack, frames);
    printf("ALLOC ptr %p when %-7zu size %-7zu where ", ptr,
           get_cur_ms() - time_base, size);
    for (int i = 1; i < frames; i++) {
      printf("%s ", symbols[i]);
    }
    printf("\n");
    allow_record = true;
  }

  return ptr;
}

void free(void *ptr) {

  static int flag = 0;
  static void (*free_cp)(void *);

  if (flag == 0) {
    time_base = get_cur_ms();
    free_cp = (void (*)(void *))dlsym(RTLD_NEXT, "free");
    char *err;
    if ((err = dlerror()) != NULL) {
      _exit(1);
    }
    flag = 1;
  }

  if (allow_record == true) {
    allow_record = false;
    void *callstack[128];
    int frames = backtrace(callstack, 128);
    char **symbols = backtrace_symbols(callstack, frames);
    printf("FREE  ptr %p when %-7zu size %-7d where ", ptr,
           get_cur_ms() - time_base, 0);
    for (int i = 1; i < frames; i++) {
      printf("%s ", symbols[i]);
    }
    printf("\n");
    allow_record = true;
  }

  free_cp(ptr);
}
