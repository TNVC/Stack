#include <stdio.h>
#include "systemlike.h"
#include "hash.h"

const int DEFAULT_HASH_OFFSET = 17;

unsigned getHash(const void *data, size_t size)
{
  if (!isPointerCorrect(data))
    return 0;

  int hash = DEFAULT_HASH_OFFSET;

  for (const char *ptr = (const char *)data; ptr != (const char *)data + size; ++ptr)
    hash += (hash << 5) + hash + *ptr;

  return (unsigned)hash;
}
