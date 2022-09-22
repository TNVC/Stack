#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logging.h"

#define SEPARATOR "============================================="

#define START_LOG                                           \
  do                                                        \
    {                                                       \
  fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
                                                            \
  time_t LOG_TIME_TEMP = 0;                                 \
  time(&LOG_TIME_TEMP);                                     \
  fprintf(LOG_FILE, "%s", ctime(&LOG_TIME_TEMP));           \
                                                            \
  fprintf(LOG_FILE, SEPARATOR "START" SEPARATOR "\n");      \
                                                            \
  fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
  } while (0)

#define END_LOG                                           \
  do                                                      \
  {                                                       \
    fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");          \
                                                          \
    fprintf(LOG_FILE, SEPARATOR " END " SEPARATOR "\n");  \
                                                          \
    fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");          \
  } while (0)


FILE *LOG_FILE = nullptr;

int  LOG_LEVEL = initLog();

int initLog()
{
  LOG_FILE = fopen(LOG_FILE_NAME, "a");

  if (LOG_FILE == nullptr)
    return 0x00;

  setvbuf(LOG_FILE, nullptr, _IOFBF, 0);

  atexit(destroyLog);

  START_LOG;

  return 0xFF;
}

void destroyLog()
{
  END_LOG;

  fclose(LOG_FILE);
}
