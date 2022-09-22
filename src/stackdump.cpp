#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

#define SHORT_HORIZONTAL_BORDER "#------"
#define LONG_HORIZONTAL_BORDER  "#-----------"
#define SHORT_FILLED_LINE       "|      "
#define LONG_FILLED_LINE        "|           "
#define SHORT_EMPTY_LINE        "|======"
#define LONG_EMPTY_LINE         "|==========="

const char * STATUS_NAME[] = {
  "INIT",
  "FAIL_INIT",
  "DESTROY",
  "EMPTY",
  "FAIL_PUSH",
  "FAIL_POP",
  "FAIL_RESIZE",
  "VALID"
};

/// Count a size of int value
/// @param [in] value Integer value which need to count chars
/// @return Count ofchars in value
static unsigned intLength(int value);

/// Print Stack status into file
/// @param [in] stk Pointer to stack
/// @param [in] filePtr File for writing
static void printStatus(const Stack *stk, FILE *filePtr);

/// Print border for stack array into file
/// @param [in] stk Pointer to stack
/// @param [in] filePtr File for writing
static void printBorder(const Stack *stk, FILE *filePtr);

/// Print line for stack array into file
/// @param [in] stk Pointer to stack
/// @param [in] filePtr File for writing
static void printLine(const Stack *stk, FILE *filePtr);

/// Print values in stack array
/// @param [in] stk Pointer to stack
/// @param filePtr File for writing
static void printValue(const Stack *stk, FILE *filePtr);

/// Print arror for stack array into file
/// @param [in] stk Pointer to stack
/// @param [in] filePtr File for writing
static void printArror(const Stack *stk, FILE *filePtr);

void stack_dump(const Stack *stk, FILE *filePtr)
{
    assert(stk     != nullptr);
    assert(filePtr != nullptr);

    fputc('\n', filePtr);

    printStatus(stk, filePtr);

    printBorder(stk, filePtr);

    printLine  (stk, filePtr);

    printValue (stk, filePtr);

    printLine  (stk, filePtr);

    printBorder(stk, filePtr);

    printArror (stk, filePtr);

    fputc('\n', filePtr);
}

static void printStatus(const Stack *stk, FILE *filePtr)
{
  fprintf(filePtr, "#---------------------------#------#\n");

  fprintf(filePtr, "|%27s|%6lu|\n|%27s|%6zd|\n",
          "Array size", stk->capacity,
          "Stack size", (stk->status & INIT) ? stk->lastElementIndex + 1 : 0);

  fprintf(filePtr, "#---------------------------#------#\n");

  for (unsigned i = 0; i < STATUS_COUNT; ++i)
      fprintf(filePtr, "|%27s|%6s|\n", STATUS_NAME[i], ((stk->status >> i) & 0x01) ? "True" : "False");

  fprintf(filePtr, "#---------------------------#------#\n");
}

static void printBorder(const Stack *stk, FILE *filePtr)
{
  if (!(stk->status & INIT) || (stk->status & DESTROY))
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    fprintf(filePtr, intLength(stk->array[i]) <= 6 ?
            SHORT_HORIZONTAL_BORDER : LONG_HORIZONTAL_BORDER);

  fprintf(filePtr, "#\n");
}

static void printLine(const Stack *stk, FILE *filePtr)
{
  if (!(stk->status & INIT) || (stk->status & DESTROY))
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    {
      const char *string = (intLength(stk->array[i]) < 6 ?
                            ((ssize_t)i <= stk->lastElementIndex ? SHORT_FILLED_LINE : SHORT_EMPTY_LINE)
                            :
                            ((ssize_t)i <= stk->lastElementIndex ? LONG_FILLED_LINE : LONG_EMPTY_LINE));

      fprintf(filePtr, "%s", string);
    }

  fprintf(filePtr, "|\n");

}

static void printValue(const Stack *stk, FILE *filePtr)
{
  if (!(stk->status & INIT) || (stk->status & DESTROY))
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    fprintf(filePtr, "|%*d", intLength(stk->array[i]) < 6 ? 6 : 11, stk->array[i]);

  fprintf(filePtr, "|\n");
}

static void printArror(const Stack *stk, FILE *filePtr)
{
  if (!(stk->status & INIT) || (stk->status & DESTROY))
    return;

  fprintf(filePtr, "%s", intLength(stk->array[stk->lastElementIndex]) <= 6 ?
          "~~~~~~" : "~~~~~~~~~~~");

  for (ssize_t i = 0; i < stk->lastElementIndex; ++i)
      fprintf(filePtr, "%s", intLength(stk->array[i]) <= 6 ? "~~~~~~~" : "~~~~~~~~~~~~");

  fputc('^', filePtr);
}

static unsigned intLength(int value)
{
  int charsNum = !(value > 0);

  value = value * !charsNum - value * charsNum;
  while (value > 0)
    {
      ++charsNum;

      value /= 10;
    }

  return (unsigned)charsNum;
}
