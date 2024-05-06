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

const int MIN_RECORD = 0;   // 选择要记录的最小内存单元

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

  if (allow_record == true && size >= MIN_RECORD) {
    allow_record = false;
    // void *callstack[128];
    // int frames = backtrace(callstack, 128);
    // char **symbols = backtrace_symbols(callstack, frames);
    printf("ALLOC ptr %p when %-7zu size %-7zu", ptr,
           get_cur_ms() - time_base, size);
    // for (int i = 1; i < frames; i++) {
    //   printf("%s ", symbols[i]);
    // }
    printf("\n");
    allow_record = true;
  }

  return ptr;
}


void *realloc(void *ptr, size_t size) {
  static int flag = 0;
  static void *(*realloc_cp)(void *, size_t);

  if (flag == 0) {
    time_base = get_cur_ms();
    realloc_cp = (void *(*)(void *, size_t))dlsym(RTLD_NEXT, "realloc");
    char *err;
    if ((err = dlerror()) != NULL) {
      _exit(1);
    }
    flag = 1;
  }

  void *new_ptr = realloc_cp(ptr, size);

  if (allow_record == true && size >= MIN_RECORD) {
    allow_record = false;
    // void *callstack[128];
    // int frames = backtrace(callstack, 128);
    // char **symbols = backtrace_symbols(callstack, frames);
    printf("REALLOC ptr %p when %-7zu size %-7zu", new_ptr,
           get_cur_ms() - time_base, size);
    // for (int i = 1; i < frames; i++) {
    //   printf("%s ", symbols[i]);
    // }
    printf("\n");
    allow_record = true;
  }

  return new_ptr;
}

void *calloc(size_t nmemb, size_t size) {
  static int flag = 0;
  static void *(*calloc_cp)(size_t, size_t);

  if (flag == 0) {
    time_base = get_cur_ms();
    calloc_cp = (void *(*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
    char *err;
    if ((err = dlerror()) != NULL) {
      _exit(1);
    }
    flag = 1;
  }

  void *ptr = calloc_cp(nmemb, size);

  if (allow_record == true && size >= MIN_RECORD) {
    allow_record = false;
    // void *callstack[128];
    // int frames = backtrace(callstack, 128);
    // char **symbols = backtrace_symbols(callstack, frames);
    printf("CALLOC ptr %p when %-7zu size %-7zu", ptr,
           get_cur_ms() - time_base, nmemb * size);
    // for (int i = 1; i < frames; i++) {
    //   printf("%s ", symbols[i]);
    // }
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
    // void *callstack[128];
    // int frames = backtrace(callstack, 128);
    // char **symbols = backtrace_symbols(callstack, frames);
    // printf("FREE  ptr %p when %-7zu size %-7d where ", ptr,
    //        get_cur_ms() - time_base, 0);
    // for (int i = 1; i < frames; i++) {
    //   printf("%s ", symbols[i]);
    // }
    // printf("\n");

    printf("FREE  ptr %p when %-7zu \n", ptr, get_cur_ms() - time_base);

    allow_record = true;
  }

  free_cp(ptr);
}
