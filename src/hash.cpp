#include <stdio.h>
#include "systemlike.h"
#include "hash.h"

unsigned getHash(const void *data, size_t size)
{
  if (!isPointerCorrect(data))
    return 0;

  int hash = 0;

  int temp = 0;

  for (size_t i = 0; i < size; ++i)
    {
      temp = *((const char *)data + i);

      hash += 17 + (temp << 5) + temp;
    }

  return (unsigned)hash;
}
