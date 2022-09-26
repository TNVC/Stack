#include <stdlib.h>
#include "stack.h"
#include "hash.h"
#include "asserts.h"
#include "elementfunctions.h"
#include "systemlike.h"
#include "logging.h"

#pragma GCC diagnostic ignored "-Wcast-qual"

#ifndef RELEASE_BUILD_

#define CHECK_VALID(STACK_POINTER, ERROR, ...)                          \
  do                                                                    \
    {                                                                   \
      unsigned ERROR_CODE_TEMP = stack_valid(STACK_POINTER);            \
                                                                        \
      if (ERROR_CODE_TEMP)                                              \
        {                                                               \
          stack_dump(STACK_POINTER, ERROR_CODE_TEMP, getLogFile());     \
                                                                        \
          if (ERROR)                                                    \
            *ERROR = ERROR_CODE_TEMP;                                   \
                                                                        \
          return __VA_ARGS__;                                           \
        }                                                               \
    } while (0)

#define UPDATE_HASH(STACK_POINTER)                                      \
  do                                                                    \
    {                                                                   \
      STACK_POINTER->arrayHash = getHash(STACK_POINTER->array,          \
                                         STACK_POINTER->capacity * sizeof(Element)); \
                                                                        \
      STACK_POINTER->hash = 0;                                          \
      STACK_POINTER->hash = getHash(STACK_POINTER, sizeof(Stack));      \
    } while(0)

#else

#define CHECK_VALID(STACK_POINTER, ERROR, ...) ;

#define UPDATE_HASH(STACK_POINTER) ;

#endif

#define LEFT_CANARY        0xDEADBEAF
#define RIGHT_CANARY       0xBADC0FEE
#define LEFT_ARRAY_CANARY  0xBEADFACE
#define RIGHT_ARRAY_CANARY 0xABADBABE

const size_t DEFAULT_STACK_GROWTH = 2;
const size_t DEFAULT_STACK_OFFSET = 5;


unsigned stack_valid(const Stack *stk)
{
#ifdef RELEASE_BUILD_

  return 0;

#endif

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

#ifndef RELEASE_BUILD_

  if (stk->leftCanary != LEFT_CANARY)
    error |= LEFT_CANARY_DIED;

  if (stk->rightCanary != RIGHT_CANARY)
    error |= RIGHT_CANARY_DIED;

  if (stk->array && isPointerCorrect(stk->array))
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

#endif

  return error;
}

void do_stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *),
                  const char *name, const char *fileName, const char *functionName, int line,
                  unsigned *error)
{
    assert(stk);
    assert(copyFunction);
    assert(name);
    assert(fileName);
    assert(functionName);
    assert(line > 0);

    assert(!(stk->status & INIT));

#ifndef RELEASE_BUILD_

    stk->leftCanary  = LEFT_CANARY;
    stk->rightCanary = RIGHT_CANARY;

#endif

    stk->capacity         = capacity;
    stk->lastElementIndex = 0;
    stk->status           = INIT | EMPTY;
    stk->copyFunction     = copyFunction;

#ifndef RELEASE_BUILD_

    stk->info.name             = name;
    stk->info.fileName         = fileName;
    stk->info.functionName     = functionName;
    stk->info.line             = line;

#endif

    if (capacity == 0)
        stk->array = nullptr;
    else
      {
#ifndef RELEASE_BUILD_

        stk->array = (Element *) calloc(1, capacity*sizeof(Element) + 2*sizeof(CANARY));
#else

        stk->array = (Element *) calloc(1, capacity*sizeof(Element));

#endif
        if (stk->array == nullptr)
          {
            if (isPointerCorrect(error))
              *error = 1;

            return;
          }

#ifndef RELEASE_BUILD_

        *(CANARY *)stk->array = LEFT_ARRAY_CANARY;

        stk->array = (Element *)((char *)stk->array + sizeof(CANARY));

        *(CANARY *)(stk->array + capacity) = RIGHT_ARRAY_CANARY;

#endif
      }

    UPDATE_HASH(stk);

    CHECK_VALID(stk, error);
}

void stack_destroy(Stack *stk, unsigned *error)
{
  CHECK_VALID(stk, error);

  assert(stk->status & INIT);

#ifndef RELEASE_BUILD_

  free((char *)stk->array - sizeof(CANARY));

#else

  free(stk->array);

#endif
  stk->array = nullptr;

  stk->capacity         = 0;
  stk->lastElementIndex = 0;

  stk->copyFunction = nullptr;

  stk->status |= DESTROY;

  UPDATE_HASH(stk);
}

void stack_push(Stack *stk, const Element *element, unsigned *error)
{
  CHECK_VALID(stk, error);

  assert(element != nullptr);

  if (stk->lastElementIndex == stk->capacity)
    {
      stack_resize(stk, stk->capacity * DEFAULT_STACK_GROWTH);

      if (stk->array == nullptr)
        {
          stk->capacity         = 0;
          stk->lastElementIndex = 0;

          if (isPointerCorrect(error))
            *error = 1;

          return;
        }
    }

  stk->copyFunction(&stk->array[(stk->lastElementIndex)++], element);

  stk->status &= NOT_EMPTY;

  UPDATE_HASH(stk);

  CHECK_VALID(stk, error);
}

void stack_pop(Stack *stk, Element *element, unsigned *error)
{
  CHECK_VALID(stk, error);

  assert(element != nullptr);

  if (stk->status & EMPTY)
    {
      if (isPointerCorrect(error))
        *error = 1;

      return;
    }

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

          if (isPointerCorrect(error))
            *error = 1;

          return;
        }
    }

  UPDATE_HASH(stk);

  CHECK_VALID(stk, error);
}

void stack_resize(Stack *stk, size_t newSize, unsigned *error)
{
  CHECK_VALID(stk, error);

  if (newSize)
    {
#ifndef RELEASE_BUILD_

      stk->array = (Element *)((char *)stk->array - sizeof(CANARY));

      stk->array = (Element *) recalloc(stk->array, 1,  newSize*sizeof(Element) + 2*sizeof(CANARY));

#else

      stk->array = (Element *) recalloc(stk->array, 1, newSize*sizeof(Element));

#endif

      if (stk->array != nullptr)
        {
#ifndef RELEASE_BUILD_

          stk->array = (Element *)((char *)stk->array + sizeof(CANARY));

          *(CANARY *)(stk->array + newSize) = RIGHT_ARRAY_CANARY;

          if (stk->capacity < newSize)
            {
              Element poison = getPoison(&stk->array[0]);

              for (size_t i = newSize - stk->capacity; i < newSize; ++ i)
                stk->copyFunction(&stk->array[i], &poison);
            }

#endif
        }
      else
        {
          if (isPointerCorrect(error))
            *error = 1;

          return;
        }
    }
  else
    {
#ifndef RELEASE_BUILD_

      stk->array = (Element *)((char *)stk->array - sizeof(CANARY));

      free(stk->array);

#else

      free(stk->array);

#endif

      stk->array = nullptr;
    }

  stk->capacity = newSize;

  UPDATE_HASH(stk);

  CHECK_VALID(stk, error);
}

size_t stack_size(const Stack *stk, unsigned *error)
{
  CHECK_VALID(stk, error, -1u);

  return stk->lastElementIndex + 1;
}

size_t stack_capacity(const Stack *stk, unsigned *error)
{
  CHECK_VALID(stk, error, -1u);

  return stk->capacity;
}

int stack_isEmpty(const Stack *stk, unsigned *error)
{
  CHECK_VALID(stk, error, 0);

  return stk->status & EMPTY;
}
