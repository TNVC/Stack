#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "recalloc.h"

void *recalloc(void *pointer, size_t elements, size_t elementSize)
{
  size_t oldSize = malloc_usable_size(pointer);

  void *newPointer = realloc(pointer, elements * elementSize);

  if (newPointer && oldSize < elements * elementSize)
   memset((char *)newPointer + oldSize, 0, elements*elementSize - oldSize);

  return newPointer;
}
