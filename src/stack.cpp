#include <stdlib.h>
#include "stack.h"
#include "hash.h"
#include "asserts.h"
#include "elementfunctions.h"
#include "recalloc.h"
#include "logging.h"

#pragma GCC diagnostic ignored "-Wcast-qual"

#ifndef RELEASE_BUILD_

#define CHECK_VALID(STACK_POINTER)                          \
  do                                                        \
    {                                                       \
      unsigned error = stack_valid(STACK_POINTER);          \
                                                            \
      if (error)                                            \
        {                                                   \
          stack_dump(STACK_POINTER, error, getLogFile());   \
                                                            \
          printf("CHECK_VALID ERROR: %u\n", error);         \
                                                            \
          exit(1);                                          \
        }                                                   \
    } while (0)

#else

#define CHECK_VALID(STACK_POINTER) ;

#endif

#define UPDATE_HASH(STACK_POINTER)                                      \
  do                                                                    \
    {                                                                   \
      STACK_POINTER->arrayHash = getHash(STACK_POINTER->array,          \
                                         STACK_POINTER->capacity * sizeof(Element)); \
                                                                        \
      STACK_POINTER->hash = 0;                                          \
      STACK_POINTER->hash = getHash(STACK_POINTER, sizeof(Stack));      \
    } while(0)

#define LEFT_CANARY        0xDEADBEAF
#define RIGHT_CANARY       0xBADC0FEE
#define LEFT_ARRAY_CANARY  0xBEADFACE
#define RIGHT_ARRAY_CANARY 0xABADBABE

const size_t DEFAULT_STACK_GROWTH = 2;
const size_t DEFAULT_STACK_OFFSET = 5;


unsigned stack_valid(const Stack *stack)
{
  Stack *stk = (Stack *)stack;

  if (!stk)
    return NULL_STACK_POINTER;

  unsigned error = 0;

  if (!(stk->status & INIT) && (stk->status & DESTROY))
    error |= DESTROY_WITHOUT_INIT;

  if ((stk->status & EMPTY) && !(stk->lastElementIndex + 1))
    error |= INCORRECT_STATUS;

  if (!stk->array && stk->capacity)
    error |= NULL_ARRAY_POINTER;

  if (stk->capacity < stk->lastElementIndex)
    error |= CAPACITY_LESS_THAN_SIZE;

  if (!stk->copyFunction)
    error |= NOT_COPYFUNCTION;

  if (stk->leftCanary != LEFT_CANARY)
    error |= LEFT_CANARY_DIED;

  if (stk->rightCanary != RIGHT_CANARY)
    error |= RIGHT_CANARY_DIED;

  if (stk->array)
    {
      if (*(CANARY *)((char *)stk->array - sizeof(CANARY)) != LEFT_ARRAY_CANARY)
        error |= LEFT_ARRAY_CANARY_DIED;

      if (*(CANARY *)(stk->array + stk->capacity)  != RIGHT_ARRAY_CANARY)
        error |= RIGHT_ARRAY_CANARY_DIED;

      if (getHash(stk->array, stk->capacity * sizeof(Element)) != stk->arrayHash)
        error |= DIFFERENT_ARRAY_HASH;
    }

  unsigned hash = stk->hash;

  stk->hash = 0;

  if (getHash(stk, sizeof(Stack)) != hash)
    error |= DIFFERENT_HASH;

  stk->hash = hash;

  if (!stk->info.name)
    error |= NOT_NAME;

  if (!stk->info.fileName)
    error |= NOT_FILE_NAME;

  if (!stk->info.functionName)
    error |= NOT_FUNCTION_NAME;

  if (stk->info.line <= 0)
    error |= INCORRECT_LINE;

  return error;
}

int do_stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *),
                  const char *name, const char *fileName, const char *functionName, int line)
{
    assert(stk);
    assert(copyFunction);
    assert(name);
    assert(fileName);
    assert(functionName);
    assert(line > 0);

    assert(!(stk->status & INIT));

    stk->leftCanary  = LEFT_CANARY;
    stk->rightCanary = RIGHT_CANARY;

    stk->capacity         = capacity;
    stk->lastElementIndex = 0;
    stk->status           = INIT | EMPTY;
    stk->copyFunction     = copyFunction;

    stk->info.name             = name;
    stk->info.fileName         = fileName;
    stk->info.functionName     = functionName;
    stk->info.line             = line;

    if (capacity == 0)
        stk->array = nullptr;
    else
      {
        stk->array = (Element *) calloc(1, capacity*sizeof(Element) + 2*sizeof(CANARY));

        if (stk->array == nullptr)
            return 1;

        *(CANARY *)stk->array = LEFT_ARRAY_CANARY;

        stk->array = (Element *)((char *)stk->array + sizeof(CANARY));

        *(CANARY *)(stk->array + capacity) = RIGHT_ARRAY_CANARY;
      }

    UPDATE_HASH(stk);

    CHECK_VALID(stk);

    return 0;
}

void stack_destroy(Stack *stk)
{
  CHECK_VALID(stk);

  assert(stk->status & INIT);

  free((char *)stk->array - sizeof(CANARY));

  stk->array = nullptr;

  stk->capacity         = 0;
  stk->lastElementIndex = 0;

  stk->copyFunction = nullptr;

  stk->status |= DESTROY;

  UPDATE_HASH(stk);
}

void stack_clean(Stack *stk)
{
  CHECK_VALID(stk);

  assert(stk->status & DESTROY);

  stk->status = 0x00;

  CHECK_VALID(stk);
}

int stack_push(Stack *stk, const Element *element)
{
  CHECK_VALID(stk);

  assert(element != nullptr);

  if (stk->lastElementIndex == stk->capacity)
    {
      stack_resize(stk, stk->capacity * DEFAULT_STACK_GROWTH);

      if (stk->array == nullptr)
        {
          stk->capacity         = 0;
          stk->lastElementIndex = 0;

          return 1;
        }
    }

  stk->copyFunction(&stk->array[(stk->lastElementIndex)++], element);

  stk->status &= NOT_EMPTY;

  UPDATE_HASH(stk);

  CHECK_VALID(stk);

  return 0;
}

int stack_pop(Stack *stk, Element *element)
{
  CHECK_VALID(stk);

  assert(element != nullptr);

  if (stk->status & EMPTY)
    return 1;

  stk->copyFunction(element, &stk->array[--(stk->lastElementIndex)]);

  if (stk->lastElementIndex == 0)
    stk->status |= EMPTY;

  UPDATE_HASH(stk);

  if (stk->lastElementIndex < stk->capacity / DEFAULT_STACK_GROWTH - DEFAULT_STACK_OFFSET)
    {
      stack_resize(stk, stk->capacity / DEFAULT_STACK_GROWTH);

      if (stk->array == nullptr)
        {
          stk->capacity         = 0;
          stk->lastElementIndex = 0;

          return 1;
        }
    }

  UPDATE_HASH(stk);

  CHECK_VALID(stk);

  return 0;
}

void stack_resize(Stack *stk, size_t newSize)
{
  CHECK_VALID(stk);

  if (newSize)
    {
      stk->array = (Element *)((char *)stk->array - sizeof(CANARY));

      stk->array = (Element *) recalloc(stk->array, 1,  newSize*sizeof(Element) + 2*sizeof(CANARY));

      if (stk->array != nullptr)
        {
          stk->array = (Element *)((char *)stk->array + sizeof(CANARY));

          *(CANARY *)(stk->array + newSize) = RIGHT_ARRAY_CANARY;

          if (stk->capacity < newSize)
            {
              Element poison = getPoison(&stk->array[0]);

              for (size_t i = newSize - stk->capacity; i < newSize; ++ i)
                stk->copyFunction(&stk->array[i], &poison);
            }
        }
    }

  stk->capacity = newSize;

  UPDATE_HASH(stk);

  CHECK_VALID(stk);
}

size_t stack_size(const Stack *stk)
{
  CHECK_VALID(stk);

  return stk->lastElementIndex + 1;
}

size_t stack_capacity(const Stack *stk)
{
  CHECK_VALID(stk);

  return stk->capacity;
}

int stack_isEmpty(const Stack *stk)
{
  CHECK_VALID(stk);

  return stk->status & EMPTY;
}
