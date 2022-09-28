#if not defined ASSERTS_H && not defined RELEASE_BUILD_
#define ASSERTS_H_

#include <stdio.h>
#include <stddef.h>

#define assert(EXPRESSION)                                              \
  do                                                                    \
    {                                                                   \
      if (!(EXPRESSION))                                                \
        {                                                               \
          printf("Expression: %s, File: %s, Function: %s, Line: %d\n",  \
                 #EXPRESSION, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
                                                                        \
          abort();                                                      \
        }                                                               \
    } while (0)

#endif
