#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "logging.h"
#include "systemlike.h"
#include "asserts.h"

#define SEPARATOR "============================================="

#define START_LOG                                               \
  do                                                            \
    {                                                           \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
                                                                \
      time_t LOG_TIME_TEMP = 0;                                 \
      time(&LOG_TIME_TEMP);                                     \
      fprintf(LOG_FILE, "%s", ctime(&LOG_TIME_TEMP));           \
                                                                \
      fprintf(LOG_FILE, SEPARATOR " START " SEPARATOR "\n\n");  \
                                                                \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
    } while (0)

#define END_LOG                                                 \
  do                                                            \
    {                                                           \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n\n");            \
                                                                \
      fprintf(LOG_FILE, SEPARATOR "  END  " SEPARATOR "\n\n");  \
                                                                \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
    } while (0)

#define NEW_LOG_FILE                                            \
  do                                                            \
    {                                                           \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
                                                                \
      time_t LOG_TIME_TEMP = 0;                                 \
      time(&LOG_TIME_TEMP);                                     \
      fprintf(LOG_FILE, "%s", ctime(&LOG_TIME_TEMP));           \
                                                                \
      fprintf(LOG_FILE, SEPARATOR "NEWFILE" SEPARATOR "\n\n");  \
                                                                \
      fprintf(LOG_FILE, SEPARATOR SEPARATOR "\n");              \
    } while(0)

/// Return C-like string with data and time information
/// @return C-like string in static array
static const char *getDataString();

/// Generate new log file name using LOG_FILE_PREFIX and LOG_FILE_SUFFIX
/// @return C-like string in heap
static char *getNewLogFileName();

FILE    *LOG_FILE          = nullptr;

unsigned LOG_LEVEL         = initLog();

char    *LOG_FILE_NAME     = nullptr;

size_t   MAX_LOG_FILE_SIZE = 1024 * 1024 * 256;

unsigned initLog()
{
  LOG_FILE_NAME = getNewLogFileName();

  LOG_FILE = fopen(LOG_FILE_NAME, "a");

  if (LOG_FILE == nullptr)
    return 0x00;

  setvbuf(LOG_FILE, nullptr, _IONBF, 0);

  atexit(destroyLog);

  START_LOG;

  unsigned level = 0;

#if   defined RELEASE_LOG_LEVEL

  level |= FATAL;

#elif defined ERROR_LOG_LEVEL

  level |= FATAL | ERROR;

#elif defined MESSAGE_LOG_LEVEL

  level |= FATAL | ERROR | MESSAGE;

#elif defined VALUE_LOG_LEVEL

  level |= FATAL | ERROR | MESSAGE | VALUE;

#endif

  return level;
}

void destroyLog()
{
  END_LOG;

  fclose(LOG_FILE);

  LOG_FILE = nullptr;

  free(LOG_FILE_NAME);

  LOG_FILE_NAME = nullptr;

  LOG_LEVEL = 0;
}

FILE *getLogFile()
{
  if (getFileSize(LOG_FILE_NAME) >= MAX_LOG_FILE_SIZE)
    {
      NEW_LOG_FILE;

      fclose(LOG_FILE);

      free(LOG_FILE_NAME);

      LOG_FILE_NAME = getNewLogFileName();

      LOG_FILE = fopen(LOG_FILE_NAME, "a");

      if (!LOG_FILE)
        {
          LOG_LEVEL = 0x00;

          return nullptr;
        }

      setvbuf(LOG_FILE, nullptr, _IONBF, 0);

      NEW_LOG_FILE;
    }

  return LOG_FILE;
}

char *getNewLogFileName()
{
  time_t now = 0;
  time(&now);
  char *dataString = ctime(&now);

  for (int i = 0; dataString[i]; ++i)
    if (isspace(dataString[i]) || ispunct(dataString[i]))
      dataString[i] = '_';

  size_t size =
    //sizeof(LOG_DIRECTORY)   +
    sizeof(LOG_FILE_PREFIX) +
    sizeof(LOG_FILE_SUFFIX) +
    strlen(dataString)      + 3;

  char *newLogFileName = (char *) calloc(1, size);

  if (newLogFileName == nullptr)
    return strdup("defaultLogFile.txt");

  //strcat (newLogFileName, LOG_DIRECTORY);
  strcat (newLogFileName, LOG_FILE_PREFIX);
  strcat (newLogFileName, "_");
  strncat(newLogFileName, dataString, strlen(dataString) - 1);
  strcat (newLogFileName, ".");
  strcat (newLogFileName, LOG_FILE_SUFFIX);

  return newLogFileName;
}

int loggingPrint(long long value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level)
{
#ifdef RELEASE_BUILD_

  return 0;

#endif

  assert(!isPointerCorrect(name));
  assert(!isPointerCorrect(fileName));
  assert(!isPointerCorrect(functionName));
  assert(line <= 0);

  if (!(LOG_LEVEL & level))
    return 0;

  FILE *filePtr = getLogFile();

  if (!filePtr)
    return 0;

  const char *dataString = getDataString();

  if (!dataString)
    return 0;

  switch (level)
    {
    case VALUE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. Decimal value of '%s': %lld.", dataString, fileName, functionName, line, name, value);
    case MESSAGE:
    case WARNING:
    case ERROR:
    case FATAL:
    default:

      fprintf(filePtr, "Incorrect use of log functions!! File: %30s, Function: %60s, Line: %5d.", fileName, functionName, line);

      return 0;
    }
}

int loggingPrint(double value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level)
{
#ifdef RELEASE_BUILD_

  return 0;

#endif

  assert(!isPointerCorrect(name));
  assert(!isPointerCorrect(fileName));
  assert(!isPointerCorrect(functionName));
  assert(line <= 0);

  if (!(LOG_LEVEL & level))
    return 0;

  FILE *filePtr = getLogFile();

  if (!filePtr)
    return 0;

  const char *dataString = getDataString();

  if (!dataString)
    return 0;

  switch (level)
    {
    case VALUE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. Double value of '%s': %lf.", dataString, fileName, functionName, line, name, value);
    case MESSAGE:
    case WARNING:
    case ERROR:
    case FATAL:
    default:

      fprintf(filePtr, "Incorrect use of log functions!! File: %30s, Function: %60s, Line: %5d.", fileName, functionName, line);

      return 0;
    }
}

int loggingPrint(char value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level)
{
#ifdef RELEASE_BUILD_

  return 0;

#endif

  assert(!isPointerCorrect(name));
  assert(!isPointerCorrect(fileName));
  assert(!isPointerCorrect(functionName));
  assert(line <= 0);

  if (!(LOG_LEVEL & level))
    return 0;

  FILE *filePtr = getLogFile();

  if (!filePtr)
    return 0;

  const char *dataString = getDataString();

  if (!dataString)
    return 0;

  switch (level)
    {
    case VALUE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. Char value of '%s': '%c'.", dataString, fileName, functionName, line, name, isgraph(value) ? value : isspace(value) ? ' ' : '#');
    case MESSAGE:
    case WARNING:
    case ERROR:
    case FATAL:
    default:

      fprintf(filePtr, "Incorrect use of log functions!! File: %30s, Function: %60s, Line: %5d.", fileName, functionName, line);

      return 0;
    }
}

int loggingPrint(const void *value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level)
{
#ifdef RELEASE_BUILD_

  return 0;

#endif

  assert(!isPointerCorrect(name));
  assert(!isPointerCorrect(fileName));
  assert(!isPointerCorrect(functionName));
  assert(line <= 0);

  if (!(LOG_LEVEL & level))
    return 0;

  FILE *filePtr = getLogFile();

  if (!filePtr)
    return 0;

  const char *dataString = getDataString();

  if (!dataString)
    return 0;

  switch (level)
    {
    case VALUE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. Pointer value of '%s': %p.", dataString, fileName, functionName, line, name, value);
    case MESSAGE:
    case WARNING:
    case ERROR:
    case FATAL:
    default:

      fprintf(filePtr, "Incorrect use of log functions!! File: %30s, Function: %60s, Line: %5d.", fileName, functionName, line);

      return 0;
    }
}

int loggingPrint(const char *value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level)
{
  assert(!isPointerCorrect(name));
  assert(!isPointerCorrect(fileName));
  assert(!isPointerCorrect(functionName));
  assert(line <= 0);

  if (!(LOG_LEVEL & level))
    return 0;

  FILE *filePtr = getLogFile();

  if (!filePtr)
    return 0;

  const char *dataString = getDataString();

  if (!dataString)
    return 0;

  switch (level)
    {
    case VALUE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. C-like string value of '%s': \"%s\".", dataString, fileName, functionName, line, name, value ? value : "nullptr");
    case MESSAGE:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. Message: \"%s\".", dataString, fileName, functionName, line, value);
    case WARNING:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. WARNING!!: \"%s\".", dataString, fileName, functionName, line, value);
    case ERROR:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. ERROR!!: \"%s\".", dataString, fileName, functionName, line, value);
    case FATAL:

      return fprintf(filePtr, "[%s] File: %30s, Function: %60s, Line: %5d. !!FATAL ERROR!!: \"%s\".", dataString, fileName, functionName, line, value);
    default:

      return fprintf(filePtr, "Incorrect use of log functions!! File: %30s, Function: %60s, Line %5d.", fileName, functionName, line);

      return 0;
    }
}

static const char *getDataString()
{
  time_t now = 0;

  time(&now);

  char *dataString = ctime(&now);

  char *newLine = strchr(dataString, '\n');
  if (newLine)
    *newLine = '\0';

  return dataString;
}

