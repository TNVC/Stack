#ifndef STACK_H_
#define STACK_H_

#include <stddef.h>
#include <stdio.h>
#include "conf.h"

#define STACK_LINE_INFO __FILE__, __func__, __LINE__
#define STACK_INIT_INFO(VALUE) #VALUE + 1, STACK_LINE_INFO

#ifndef RELEASE_BUILD_

typedef struct {
  const char *name;
  const char *fileName;
  const char *functionName;
  int line;
} DebugInfo;

typedef unsigned CANARY;

#endif

typedef struct {
#ifndef RELEASE_BUILD_

  CANARY leftCanary;

#endif

  Element *array;
  size_t capacity;
  size_t lastElementIndex;

  void (*copyFunction)(Element *, const Element *);

  unsigned status;

#ifndef RELEASE_BUILD_

  DebugInfo info;

  mutable unsigned hash;
  mutable unsigned arrayHash;

  CANARY rightCanary;

#endif
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

#define stack_init(stk, capacity, copyFunction)                     \
  do_stack_init(stk, capacity, copyFunction, STACK_INIT_INFO(stk))

/// Init Stack
/// @param [in/out] stk Pointer to stack for init
/// @param [in] capacity Start capacity for Stack
/// @param [in] copyFunction Function for copy Elements
/// @param [in] name Origin name of variable
/// @param [in] fileName File name where was create variable
/// @param [in] functionName Function name where was create variable
/// @param [in] line Line where was create variable
/// @param [out] error Return error code
/// @note Call before all using
void do_stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *),
                  const char *name, const char *fileName, const char *functionName, int line,
                  unsigned *error = nullptr);

/// Destroy Stack
/// @param [in] stk Pointer to stack for destroy
/// @param [out] error Return error code
/// @note Call after all using
void stack_destroy(Stack *stk, unsigned *error = nullptr);

/// Push one element to stack
/// @param [in/out] stk Pointer to stack
/// @param [in] element Pointer to element to push
/// @param [out] error Return error code
void stack_push(Stack *stk, const Element *element, unsigned *error = nullptr);

/// Pop one element from stack
/// @param [in/out] stk Pointer to stack
/// @param [out] element Container for pop-element
/// @param [out] error Return error code
void stack_pop(Stack *stk, Element *element, unsigned *error = nullptr);

/// Resize Stack`s array to new size
/// @param [in/out] stk Pointer to stack for resize
/// @param [in] newSize New size for Stack in Elements
/// @param [out] error Return error code
/// @note Functioun itself multiplay to sizeof(Element)
void stack_resize(Stack *stk, size_t newSize, unsigned *error = nullptr);

/// Size of Stack
/// @param [in] stk Pointer to stack
/// @param [out] error Return error code
/// @return Size of stack
size_t stack_size(const Stack *stk, unsigned *error = nullptr);

/// Size of Stack`s array
/// @param [in] stk Pointer to stack
/// @param [out] error Return error code
/// @return Stack`s capacity
size_t stack_capacity(const Stack *stk, unsigned *error = nullptr);

/// Check that stack is empty
/// @param [in] stk Pointer to stack
/// @param [out] error Return error code
/// @return 1 if Stack is empty or 0 if is not
int stack_isEmpty(const Stack *stk, unsigned *error = nullptr);

#ifndef RELEASE_BUILD_

#define stack_dump(stk, errorCode, filePtr)         \
  do_stack_dump(stk, errorCode, filePtr, STACK_LINE_INFO)

#else

#define stack_dump(stk, errorCode, filePtr) ;

#endif

/// Dump stack into file
/// @param [in] stk Pointer to Stack for dump
/// @param [in] errorCode Code from stack_valid()
/// @param [in] filePtr File for logging
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function where was call function
/// @param [in] line Line where was call function
/// @param [out] error Return error code
void do_stack_dump(const Stack *stk, unsigned errorCode, FILE *filePtr,
                   const char *fileName, const char *functionName, int line);

#endif
