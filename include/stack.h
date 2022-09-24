#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "elementfunctions.h"
#include "asserts.h"
#include "logging.h"

#define LINE_INFO __FILE__, __func__, __LINE__
#define INIT_INFO(VALUE) #VALUE, LINE_INFO

#ifndef RELEASE_BUILD_

#define CHECK_VALID(STACK_POINTER)                        \
  do                                                      \
    {                                                     \
      unsigned error = stack_valid(STACK_POINTER);        \
                                                          \
      if (error)                                          \
        {                                                 \
          stack_dump(STACK_POINTER, error, getLogFile()); \
                                                          \
          abort();                                        \
        }                                                 \
    } while (0)

#else

#define CHECK_VALID(STACK_POINTER) ;

#endif

#define LEFT_CANARY        0xDEADBEAF
#define RIGHT_CANARY       0xBADC0FEE
#define LEFT_ARRAY_CANARY  0xBEADFACE
#define RIGHT_ARRAY_CANARY 0xABADBABE

typedef unsigned CANARY;

const size_t DEFAULT_STACK_GROWTH = 2;
const size_t DEFAULT_STACK_OFFSET = 5;

typedef struct {
  const char *name;
  const char *fileName;
  const char *functionName;
  int line;
} DebugInfo;

typedef struct {
  unsigned leftCanary;

  Element *array;
  size_t capacity;
  size_t lastElementIndex;

  void (*copyFunction)(Element *, const Element *);

  unsigned status;

  DebugInfo info;

  unsigned rightCanary;
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
};

const unsigned NOT_EMPTY = -1u ^ (0x01 << 2);

const unsigned STATUS_COUNT = 3;
const unsigned ERRORS_COUNT = 16;

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

unsigned stack_valid(const Stack *stk)
{
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
    }

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

  if (stack_size(stk) < stk->capacity / DEFAULT_STACK_GROWTH - DEFAULT_STACK_OFFSET)
    {
      stack_resize(stk, stk->capacity / DEFAULT_STACK_GROWTH);

      if (stk->array == nullptr)
        {
          stk->capacity         = 0;
          stk->lastElementIndex = 0;

          return 1;
        }
    }

  return 0;
}

void stack_resize(Stack *stk, size_t newSize)
{
  CHECK_VALID(stk);

  if (newSize)
    {
      stk->array = (Element *)((char *)stk->array - sizeof(CANARY));

      stk->array = (Element *) realloc(stk->array, newSize*sizeof(Element) + 2*sizeof(CANARY));//////

      if (stk->array != nullptr)
        {
          stk->array = (Element *)((char *)stk->array + sizeof(CANARY));

          *(CANARY *)(stk->array + newSize) = RIGHT_ARRAY_CANARY;
        }
    }

  stk->capacity = newSize;

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

/// stack_dump

#define STATUS_BORDER "#---------------------------#------#"
#define ERRORS_BORDER "#----------------------------------#"

const int POISON_LENGTH = 6;

int MAX_LENGTH    = 0;
int MIDDLE_LENGTH = 0;

const char *ERRORS_MESSAGE[] = {
  "Pointer to stack is NULL",
  "Stack was destroy without init",
  "Status is incorrect",
  "Pointer to stack`s array is NULL",
  "Capacity less than size",
  "Stack hasn`t a copyFunction",
  "Left canary is died",
  "Right canary is died",
  "Left array canary is died",
  "Right array canary is died",
  "Stack hasn`t a name",
  "Stack hasn`t a file name",
  "Stack hasn`t a function name",
  "Stack hasn`t a correct line"
  "Stack hash is corrupted",
  "Stack`s array hash is corrupted"
};

const char *STATUS_NAME[] = {
  "INIT",
  "DESTROY",
  "EMPTY"
};

/// Print errors` message
/// @param [in] errorCode Code of errors
/// @param [in] filePtr File for writing
static void printErrors(unsigned errorCode, FILE *filePtr);

/// Print Stack status into file
/// @param [in] stk Pointer to stack
/// @param [in] filePtr File for writing
static void printStatus(const Stack *stk, FILE *filePtr);

/// Print addressses of stack`s elements
/// @param [in] stk Pointer to stack
/// @param [in] filePtr FIle for writing
static void printAddresses(const Stack *stk, FILE *filePtr);

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
static void printValues(const Stack *stk, FILE *filePtr);

/// Print arror for stack array into file
/// @param [in] stk  Pointer to stack
/// @param [in] filePtr File for writing
static void printArror(const Stack *stk, FILE *filePtr);

void do_stack_dump(const Stack *stk, unsigned errorCode, FILE *filePtr,
                   const char *fileName, const char *functionName, int line)
{
  assert(filePtr);
  assert(fileName);
  assert(functionName);
  assert(line);

  fputc('\n', filePtr);

  fprintf(filePtr, "%s at %s (%d):\n", functionName, fileName, line);
  fprintf(filePtr, "Stack[%p]", stk);

  if (stk)
    fprintf(filePtr, " \"%s\" at %s at %s (%d)",
            stk->info.name         ? stk->info.name         : "nullptr",
            stk->info.functionName ? stk->info.functionName : "nullptr",
            stk->info.fileName     ? stk->info.fileName     : "nullptr",
            stk->info.line);

  if (stk && stk->array)
    {
      MAX_LENGTH    = maxElementLength(&stk->array[0]);

      MIDDLE_LENGTH = (MAX_LENGTH + 1) / 2;
    }

  fputc('\n', filePtr);

  printErrors(errorCode, filePtr);

  printStatus   (stk, filePtr);

  printAddresses(stk, filePtr);

  printBorder   (stk, filePtr);

  printLine     (stk, filePtr);

  printValues   (stk, filePtr);

  printLine     (stk, filePtr);

  printBorder   (stk, filePtr);

  printArror    (stk, filePtr);

  fputc('\n', filePtr);
}

static void printErrors(unsigned errorCode, FILE *filePtr)
{
  if (!errorCode)
    {
      fprintf(filePtr, "Stack is ok\n");

      return;
    }

  fprintf(filePtr, ERRORS_BORDER "\n");
  fprintf(filePtr,"|%-34s|\n", "ERRORS!!");
  fprintf(filePtr, ERRORS_BORDER "\n");

  for (unsigned i = 0; i < ERRORS_COUNT; ++i)
    {
      if (!((errorCode >> i) & 0x01))
        continue;

      fprintf(filePtr, "|%-34s|\n", ERRORS_MESSAGE[i]);
    }

  fprintf(filePtr, ERRORS_BORDER "\n");
}

static void printStatus(const Stack *stk, FILE *filePtr)
{
  fprintf(filePtr, STATUS_BORDER "\n");

  fprintf(filePtr, "|%-27s|%6lu|\n|%-27s|%6lu|\n",
          "Stack capacity", stk->capacity,
          "Stack size", stk->lastElementIndex);

  fprintf(filePtr, STATUS_BORDER "\n");

  for (unsigned i = 0; i < STATUS_COUNT; ++i)
      fprintf(filePtr, "|%-27s|%-6s|\n", STATUS_NAME[i], ((stk->status >> i) & 0x01) ? "True" : "False");

  fprintf(filePtr, STATUS_BORDER "\n");
}

static void printAddresses(const Stack *stk, FILE *filePtr)
{
  if (!stk->array)
    return;

  int firstSize = elementLength(&stk->array[0]) < MIDDLE_LENGTH ?
    MIDDLE_LENGTH : MAX_LENGTH;

  if (isPoison(&stk->array[0]))
    firstSize = POISON_LENGTH;

  if (stk->capacity == 1)
    {
      fprintf(filePtr, "%p\n%*s|\n%*s|\n%*sV\n",
               stk->array, firstSize, "", firstSize, "", firstSize, "");

      return;
    }

  int temp = elementLength(&stk->array[stk->capacity - 1]) < MIDDLE_LENGTH ?
    MIDDLE_LENGTH : MAX_LENGTH;

  if (isPoison(&stk->array[stk->capacity - 1]))
    temp = POISON_LENGTH;

  int lastSize = temp;

  for (size_t i = 1; i < stk->capacity - 1; ++i)
    {
      int size = (elementLength(&stk->array[i]) < MIDDLE_LENGTH ?
                  MIDDLE_LENGTH : MAX_LENGTH);

      if (isPoison(&stk->array[i]))
        size = POISON_LENGTH;

      lastSize += size + 1;
    }

  fprintf(filePtr, "%p\n%*s|%*s%p\n%*s|%*s|\n%*sV%*sV\n",
          stk->array,
          firstSize, "", lastSize - temp, "", stk->array + stk->capacity - 1,
          firstSize, "", lastSize, "",
          firstSize, "", lastSize, "");
}

static void printBorder(const Stack *stk, FILE *filePtr)
{
  if (!stk->array)
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    {
      fputc('#', filePtr);

      int size = elementLength(&stk->array[i]) < MIDDLE_LENGTH ? MIDDLE_LENGTH : MAX_LENGTH;

      if (isPoison(&stk->array[i]))
        size = POISON_LENGTH;

      for (int j = 0; j < size; ++j)
        fputc('-', filePtr);
    }

  fprintf(filePtr, "#\n");
}

static void printLine(const Stack *stk, FILE *filePtr)
{
  if (!stk->array)
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    {
      fputc('|', filePtr);

      const char ch = i < stk->lastElementIndex ? ' ' : '=';

      int size = elementLength(&stk->array[i]) < MIDDLE_LENGTH ? MIDDLE_LENGTH : MAX_LENGTH;

      if (isPoison(&stk->array[i]))
        size = 6;

      for (int j = 0; j < size; ++j)
        fputc(ch, filePtr);
    }

  fprintf(filePtr, "|\n");

}

static void printValues(const Stack *stk, FILE *filePtr)
{
  if (!stk->array)
    return;

  for (size_t i = 0; i < stk->capacity; ++i)
    {
      int elementSize = elementLength(&stk->array[i]);

      int size = elementSize < MIDDLE_LENGTH ? MIDDLE_LENGTH : MAX_LENGTH;

      if (isPoison(&stk->array[i]))
        {
          fprintf(filePtr, "|POISON");

          continue;
        }

      fputc('|', filePtr);

      for (int j = 0; j < size- elementSize; ++j)
        fputc(' ', filePtr);

      printElement(&stk->array[i], filePtr);
    }

  fprintf(filePtr, "|\n");
}

static void printArror(const Stack *stk, FILE *filePtr)
{
  if (!stk->array || !stk->lastElementIndex)
    return;

  fputc('>', filePtr);

  for (size_t i = 0; i < stk->lastElementIndex - 1; ++i)
    {
      int size = elementLength(&stk->array[i]) < MIDDLE_LENGTH ? MIDDLE_LENGTH : MAX_LENGTH;

      if (isPoison(&stk->array[i]))
        size = POISON_LENGTH;

      for (int j = 0; j < size + 1; ++j)
        fputc('>', filePtr);
    }

  int size = elementLength(&stk->array[stk->lastElementIndex - 1]) < MIDDLE_LENGTH ? MIDDLE_LENGTH : MAX_LENGTH;

  if (isPoison(&stk->array[stk->lastElementIndex - 1]))
    size = POISON_LENGTH;

  for (int j = 0; j < size - 1; ++j)
    fputc('>', filePtr);

  fputc('^', filePtr);
}

#endif
