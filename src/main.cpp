#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "logging.h"

#include <time.h>
#include "consolewaiting.h"

/// For test
void copyInt(int *target, const int *source);

int main()
{
  //startConsoleWaiting();

  FILE *file = getLogFile();

  Stack stack = {};

  DUMP_LVL = DUMP_ALL;

  stack_dump(&stack, stack_valid(&stack), file);

  stack_init(&stack, 4, copyInt);

  stack_dump(&stack, stack_valid(&stack), file);

  for (int i = 0; i < 10; ++i)
    {
      int temp = (int) (((double)rand())/RAND_MAX * 200000);

      stack_push(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);

      stack_pop(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);
    }

  for (int i = 0; i < 21; ++i)
    {
      int temp = (int) (((double)rand())/RAND_MAX * 200000);

      if (i % 3)
        temp = (int) 0xDED00DED;

      if (i > 10)
        DUMP_LVL = DUMP_ALL;

      stack_push(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);
    }

  for (int i = 0; i < 21; ++i)
    {
      int temp = 0;

      if (i > 10)
        DUMP_LVL = DUMP_NOT_POISON;

      stack_pop(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);
    }

  stack_destroy(&stack);

  stack_dump(&stack, stack_valid(&stack), file);

  clock_t now = clock();

  clock_t wait = 0 * CLOCKS_PER_SEC;

  while (clock() - now < wait)
    continue;

  //stopConsoleWaiting();

  return 0;
}

void copyInt(int *target, const int *source)
{
  *target = *source;
}
