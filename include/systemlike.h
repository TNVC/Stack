#ifndef SYSTEMLIKE_H_
#define SYSTEMLIKE_H_

#include <stddef.h>

/// Combine realloc and calloc
/// @param [in] pointer Pointer to dimanic memory which was get from malloc/calloc/realloc/recalloc or else
/// @param [in] elements Count of elements which need in dimanic memory
/// @param [in] elementSize Size of one element
/// @return Pointer to allocate memory or NULL if was error in function
void *recalloc(void *pointer, size_t elements, size_t elementSize);

/// Check that address is corrrect
/// @param [in] pointer Pointer for chaeck
/// @return Is pointer correct
int isPointerCorrect(const void *pointer);

/// Get file size
/// @param [in] filename Name of file
/// @return Size of file with name filename
size_t getFileSize(const char *filename);

/// Chech then file exits
/// @param [in] fileName Name of file for check
/// @return If file exits 1 else 0
int isFileExists(const char *fileName);

#endif
