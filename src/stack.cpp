#include <stdlib.h>
#include <string.h>
#include "asserts.h"
#include "stack.h"

#define CHECK_VALID(STACK_POINTER)                                      \
  do                                                                    \
    {                                                                   \
      assert  (stk->status & INIT);                                     \
      assert(!(stk->status & DESTROY));                                 \
      assert  (stk->status & VALID);                                    \
    } while (0)

const size_t DEFAULT_STACK_CAPACITY = 10;
const size_t DEFAULT_STACK_GROWTH   =  2;
const size_t DEFAULT_STACK_OFFSET   =  5;

int stack_init(Stack *stk, size_t capacity, void (*copyFunction)(Element *, const Element *))
{
    assert(stk          != nullptr);
    assert(copyFunction != nullptr);

    assert(!(stk->status & INIT));

    stk->capacity         = capacity;
    stk->lastElementIndex = -1;
    stk->status           = INIT | EMPTY | VALID;
    stk->copyFunction     = copyFunction;

    if (capacity == 0)
        stk->array = nullptr;
    else
      {
        stk->array = (Element *) calloc(capacity, sizeof(Element));

        if (stk->status & FAIL_RESIZE)
          {
            stk->status |= FAIL_INIT;

            return 0;
          }
      }

    return 1;
}

void stack_destroy(Stack *stk)
{
    assert(stk != nullptr);

    assert  (stk->status & INIT);
    assert(!(stk->status & DESTROY));

    stk->capacity         =  0;
    stk->lastElementIndex = -1;
    stk->status          |=  DESTROY;

    if (stk->status & VALID)
      stk->status ^= VALID;

    free(stk->array);
}

void stack_clean(Stack *stk)
{
  assert(stk != nullptr);

  assert(stk->status & DESTROY);

  stk->status = 0x00;
}

int stack_push(Stack *stk, const Element *element)
{
    assert(stk     != nullptr);
    assert(element != nullptr);

    CHECK_VALID(stk);

    if (stack_size(stk) == stk->capacity)
    {
        stk->capacity *= DEFAULT_STACK_GROWTH;

        stack_resize(stk, stk->capacity);

        if (stk->status & FAIL_RESIZE)
        {
            stk->capacity         =  0;
            stk->lastElementIndex = -1;

            return 0;
        }
    }

    stk->copyFunction(&stk->array[++(stk->lastElementIndex)], element);

    if (stk->status & EMPTY)
      stk->status ^= EMPTY;

    return 1;
}

int stack_pop(Stack *stk, Element *element)
{
    assert(stk     != nullptr);
    assert(element != nullptr);

    CHECK_VALID(stk);
    assert(!(stk->status & EMPTY));

    stk->copyFunction(element, &stk->array[(stk->lastElementIndex)--]);

    if (stk->lastElementIndex == -1)
      stk->status |= EMPTY;

    if (stack_size(stk) <= stk->capacity / DEFAULT_STACK_GROWTH - DEFAULT_STACK_OFFSET)
    {
        stk->capacity /= DEFAULT_STACK_GROWTH;

        stack_resize(stk, stk->capacity);

        if (stk->status & FAIL_RESIZE)
        {
            stk->capacity         =  0;
            stk->lastElementIndex = -1;

            return 0;
        }
    }

    return 1;
}

void stack_resize(Stack *stk, size_t newSize)
{
  assert(stk != nullptr);

  CHECK_VALID(stk);

  stk->array = (Element *) realloc(stk->array, newSize * sizeof(Element));

  if (newSize != 0 && stk->array == nullptr)
    {
      stk->status |= FAIL_RESIZE;
      stk->status ^= VALID;
    }

  if (newSize < stack_size(stk))
    stk->lastElementIndex =(ssize_t)newSize - 1;

}
size_t stack_size(const Stack *stk)
{
  assert(stk != nullptr);

  CHECK_VALID(stk);

  return (size_t)(stk->lastElementIndex + 1);
}

int stack_isEmpty(const Stack *stk)
{
  assert(stk != nullptr);

  CHECK_VALID(stk);

  return stk->status & EMPTY;
}

