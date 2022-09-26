#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "consolewaiting.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static char CHARS[] = {'\\', '|', '/', '-'};

static pthread_t Waiter = {};

static void *wait(void *args);

void startConsoleWaiting()
{
  pthread_create(&Waiter, nullptr, wait, nullptr);
}

void stopConsoleWaiting()
{
  pthread_cancel(Waiter);

  printf("\b\b\b\b\b\b\e[?25h");
}

void *wait(void *args)
{
  clock_t start = clock();

  int i = 0;

  printf("\e[?25lwait %c", CHARS[i++ % 4]);
  fflush(stdout);

  while (1)
      if (clock() - start >= CLOCKS_PER_SEC)
        {
          start = clock();

          printf("\b%c", CHARS[i++ % 4]);
          fflush(stdout);
        }

  return nullptr;
}
