#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
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

size_t getFileSize(const char *fileName)
{
  if (!isPointerCorrect(fileName))
    return 0;

  struct stat temp = {};

  stat(fileName, &temp);

  return (size_t)temp.st_size;
}

int isFileExists(const char *fileName)
{
  if (!isPointerCorrect(fileName))
    return 0;

  struct stat temp = {};

  if (stat(fileName, &temp) == -1)
    return 1;

  return 0;
}
