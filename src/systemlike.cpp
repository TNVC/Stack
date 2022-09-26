#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "asserts.h"
#include "systemlike.h"

void *recalloc(void *pointer, size_t elements, size_t elementSize)
{
  size_t oldSize = malloc_usable_size(pointer);

  void *newPointer = realloc(pointer, elements * elementSize);

  if (newPointer && oldSize < elements * elementSize)
   memset((char *)newPointer + oldSize, 0, elements*elementSize - oldSize);

  return newPointer;
}

int isPointerCorrect(const void *pointer)
{
  if (!pointer)
    return 0;

  return write(1, pointer, 0) != -1;
}

size_t getFileSize(const char *filename)
{
  assert(filename != nullptr);

  struct stat temp = {};

  stat(filename, &temp);

  return (size_t)temp.st_size;
}
