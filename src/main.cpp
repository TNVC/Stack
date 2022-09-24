#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "logging.h"

/// For test
void copyInt(int *target, const int *source);

int main()
{
  FILE *file = getLogFile();

  Stack stack = {};

  stack_dump(&stack, stack_valid(&stack), file);

  stack_init(&stack, 2, copyInt);

  stack_dump(&stack, stack_valid(&stack), file);

  for (int i = 0; i < 100; ++i)
    {
      int temp = (int) (((double)rand())/RAND_MAX * 200000);

      stack_push(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);
    }

  for (int i = 0; i < 100; ++i)
    {
      int temp = 0;

      stack_pop(&stack, &temp);

      stack_dump(&stack, stack_valid(&stack), file);
    }

  stack_destroy(&stack);

  stack_dump(&stack, stack_valid(&stack), file);

  return 0;
}

void copyInt(int *target, const int *source)
{
  *target = *source;
}
