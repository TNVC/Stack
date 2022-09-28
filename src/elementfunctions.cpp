#include <stdio.h>
#include "systemlike.h"
#include "asserts.h"
#include "elementfunctions.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int printElement(const int *element, FILE *filePtr)
{
  if (!isPointerCorrect(element) || !isPointerCorrect(filePtr))
    return -1;

  return fprintf(filePtr, "%d", *element);
}

int elementLength(const int *element)
{
  if (!isPointerCorrect(element))
    return -1;

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
  if (!isPointerCorrect(element))
    return 0;

  return (int)0xDED00DED == *element;
}
