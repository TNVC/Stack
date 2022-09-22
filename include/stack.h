#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef int Element;

typedef struct {
  Element *array;
  size_t capacity;
  ssize_t lastElementIndex;
  void (*copyFunction)(Element *, const Element *);
  unsigned status;
} Stack;

const unsigned INIT        = 0x01 << 0;
const unsigned FAIL_INIT   = 0x01 << 1;
const unsigned DESTROY     = 0x01 << 2;
const unsigned EMPTY       = 0x01 << 3;
const unsigned FAILPUSH    = 0x01 << 4;
const unsigned FAIL_POP    = 0x01 << 5;
const unsigned FAIL_RESIZE = 0x01 << 6;
const unsigned VALID       = 0x01 << 7;

const unsigned STATUS_COUNT = 8;

/// Init Stack
/// @param [in/out] stk Pointer to stack for init
/// @param [in] capacity Start capacity for Stack
/// @param [in] copyFunction Function for copy Elements
/// @return Was stack init
/// @note Call before all using
int stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *));

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
/// @return 1 if element was push or 0 if element wasn`t push
int stack_push(Stack *stk, const Element *element);

/// Pop one element from stack
/// @param [in/out] stk Pointer to stack
/// @param [out] element Container for pop-element
/// @return 1 if was pop element or 0 if was not
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

/// Check that stack is empty
/// @param [in] stk Pointer to stack
/// @return 1 if Stack is empty or 0 if is not
int stack_isEmpty(const Stack *stk);

/// Dump stack into file
/// @param [in] stk Pointer to Stack for dump
/// @param [in] filePtr File for logging
void stack_dump(const Stack *stk, FILE *filePtr);

#endif
