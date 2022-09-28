#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "coloroutput.h"
#include "consolewaiting.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static const char CHARS[] = "\\|/-";

static pthread_t Waiter = {};

static int IsPThreadAlive = 0;

static void *wait(void *args);

void startConsoleWaiting()
{
  IsPThreadAlive = 1;

  pthread_create(&Waiter, nullptr, wait, nullptr);
}

void stopConsoleWaiting()
{
  IsPThreadAlive = 0;

  resetConsole();

  printf("\r\e[?25h");
}

void *wait(void *args)
{
  clock_t start = clock();

  int i = 0;

  setForegroundColor(i % COLORS_COUNT);

  printf("\e[?25lwait %c", CHARS[i++ % 4]);
  fflush(stdout);

  while (IsPThreadAlive)
      if (clock() - start >= CLOCKS_PER_SEC)
        {
          start = clock();

          setForegroundColor(i % COLORS_COUNT);

          printf("\rwait %c", CHARS[i++ % 4]);
          fflush(stdout);
        }

  return nullptr;
}
