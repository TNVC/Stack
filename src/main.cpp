#include <stdio.h>
#include "stack.h"

#include <limits.h>

void copyInt(int *target, const int *source);

void copyInt(int *target, const int *source)
{
  *target = *source;
}

int main()
{

    printf("%d\n", INT_MAX);

    Stack stack = {};

    stack_dump(&stack, stdout);

    stack_init(&stack, 10, copyInt);

    stack_dump(&stack, stdout);


    for (int i = 0; i < 11; ++i)
    {
      int temp = i;
      if (i == 19)
        {
          temp = 1000000;
        }

        stack_push(&stack, &temp);

        stack_dump(&stack, stdout);
    }

    for (int i = 0; i < 11; ++i)
    {
      int temp = 0;

      stack_pop(&stack, &temp);

      stack_dump(&stack, stdout);
    }

    stack_destroy(&stack);

    stack_dump(&stack, stdout);

    return 0;
}
