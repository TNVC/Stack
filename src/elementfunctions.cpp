#include <stdio.h>
#include "asserts.h"
#include "elementfunctions.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int printElement(const int *element, FILE *filePtr)
{
  assert(element);

  return fprintf(filePtr, "%d", *element);
}

int elementLength(const int *element)
{
  int value = *element;

  int charsNum = !(value > 0);

  value = value * !charsNum - value * charsNum;
  while (value > 0)
    {
      ++charsNum;

      value /= 10;
    }

  return charsNum;
}

int maxElementLength(const int *element)
{
  return 12;
}

int getPoison(const int *element)
{
  return (int)0xDED00DED;
}

int isPoison(const int *element)
{
  return (int)0xDED00DED == *element;
}
