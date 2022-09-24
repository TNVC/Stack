#ifndef RECALLOC_H_
#define RECALLOC_H_

#include <stdlib.h>

/// Combine realloc and calloc
/// @param [in] pointer Pointer to dimanic memory which was get from malloc/calloc/realloc/recalloc or else
/// @param [in] elements Count of elements which need in dimanic memory
/// @param [in] elementSize Size of one element
/// @return Pointer to allocate memory or NULL if was error in function
void *recalloc(void *pointer, size_t elements, size_t elementSize);

#endif
