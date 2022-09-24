#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdio.h>
#include "conf.h"

#define LINE_INFO __FILE__, __func__, __LINE__
#define INIT_INFO(VALUE) #VALUE + 1, LINE_INFO

typedef struct {
  const char *name;
  const char *fileName;
  const char *functionName;
  int line;
} DebugInfo;

typedef unsigned CANARY;

typedef struct {
  CANARY leftCanary;

  Element *array;
  size_t capacity;
  size_t lastElementIndex;

  void (*copyFunction)(Element *, const Element *);

  unsigned status;

  DebugInfo info;

  unsigned hash;
  unsigned arrayHash;

  CANARY rightCanary;
} Stack;

/// Codes of stack status
unsigned enum STACK_STATUS {
  INIT        = 0x01 << 0,
  DESTROY     = 0x01 << 1,
  EMPTY       = 0x01 << 2,
};

/// Codes of errors for stack_valid
unsigned enum ERROR {
  NULL_STACK_POINTER              = 0x01 <<  0,
  DESTROY_WITHOUT_INIT            = 0x01 <<  1,
  INCORRECT_STATUS                = 0x01 <<  2,
  NULL_ARRAY_POINTER              = 0x01 <<  3,
  CAPACITY_LESS_THAN_SIZE         = 0x01 <<  4,
  NOT_COPYFUNCTION                = 0x01 <<  5,
  LEFT_CANARY_DIED                = 0x01 <<  6,
  RIGHT_CANARY_DIED               = 0x01 <<  7,
  LEFT_ARRAY_CANARY_DIED          = 0x01 <<  8,
  RIGHT_ARRAY_CANARY_DIED         = 0x01 <<  9,
  NOT_NAME                        = 0x01 << 10,
  NOT_FILE_NAME                   = 0x01 << 11,
  NOT_FUNCTION_NAME               = 0X01 << 12,
  INCORRECT_LINE                  = 0x01 << 13,
  DIFFERENT_HASH                  = 0x01 << 14,
  DIFFERENT_ARRAY_HASH            = 0x01 << 15
};

const unsigned NOT_EMPTY = -1u ^ (0x01 << 2);

const unsigned STATUS_COUNT = 3;
const unsigned ERRORS_COUNT = 16;

enum DUMP_LEVEL {
  DUMP_ALL,
  DUMP_NOT_POISON,
  DUMP_NOT_EMPTY
};

extern DUMP_LEVEL DUMP_LVL;

/// Chech valid of stack
/// @param [in] stk Pointer to stack
/// @return Code of error
unsigned stack_valid(const Stack *stk);

#define stack_init(stk, capacity, copyFunction) \
  do_stack_init(stk, capacity, copyFunction, INIT_INFO(stk))

/// Init Stack
/// @param [in/out] stk Pointer to stack for init
/// @param [in] capacity Start capacity for Stack
/// @param [in] copyFunction Function for copy Elements
/// @param [in] name Origin name of variable
/// @param [in] fileName File name where was create variable
/// @param [in] functionName Function name where was create variable
/// @param [in] line Line where was create variable
/// @return Was stack init
/// @note Call before all using
int do_stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *),
                  const char *name, const char *fileName, const char *functionName, int line);

/// Destroy Stack
/// @param [in] stk Pointer to stack for destroy
/// @note Call after all using
void stack_destroy(Stack *stk);

/// Clean Stack for repeat use
/// @param [in/out] stk Pointer to Stack
/// @note Call after stack_destroy()
void stack_clean(Stack *stk);

/// Push one element to stack
/// @param [in/out] stk Pointer to stack
/// @param [in] element Pointer to element to push
/// @return 1 if element wasn`t push else 0
int stack_push(Stack *stk, const Element *element);

/// Pop one element from stack
/// @param [in/out] stk Pointer to stack
/// @param [out] element Container for pop-element
/// @return 1 if wasn`t pop else 0
int stack_pop(Stack *stk, Element *element);

/// Resize Stack`s array to new size
/// @param [in/out] stk Pointer to stack for resize
/// @param [in] newSize New size for Stack in Elements
/// @note Using newSize * sizeof(Element)
void stack_resize(Stack *stk, size_t newSize);

/// Size of Stack
/// @param [in] stk Pointer to stack
/// @return Size of stack
size_t stack_size(const Stack *stk);

/// Size of Stack`s array
/// @param [in] stk Pointer to stack
/// @return Stack`s capacity
size_t stack_capacity(const Stack *stk);

/// Check that stack is empty
/// @param [in] stk Pointer to stack
/// @return 1 if Stack is empty or 0 if is not
int stack_isEmpty(const Stack *stk);

#define stack_dump(stk, errorCode, filePtr)     \
  do_stack_dump(stk, errorCode, filePtr, LINE_INFO)

/// Dump stack into file
/// @param [in] stk Pointer to Stack for dump
/// @param [in] errorCode Code from stack_valid()
/// @param [in] filePtr File for logging
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function where was call function
/// @param [in] line Line where was call function
void do_stack_dump(const Stack *stk, unsigned errorCode, FILE *filePtr,
                   const char *fileName, const char *functionName, int line);

#endif
