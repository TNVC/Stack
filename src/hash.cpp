#include <stdio.h>
#include "hash.h"

unsigned getHash(const void *data, size_t size)
{
  if (!data)
    return 0;
  
  int hash = 0;

  for (size_t i = 0; i < size; ++i)
    hash += (*((const char *)data + i) << 5) + 1;

  return (unsigned)hash;
}
