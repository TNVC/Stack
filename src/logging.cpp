#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "logging.h"
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

/// Get file size
/// @param [in] filename Name of file
/// @return Size of file with name filename
static size_t getFileSize(const char *filename);

/// Generate new log file name using LOG_FILE_PREFIX and LOG_FILE_SUFFIX
/// @return C-like string in heap
static char *getNewLogFileName();

FILE *LOG_FILE = nullptr;

int  LOG_LEVEL = initLog();

char *LOG_FILE_NAME = nullptr;

size_t MAX_LOG_FILE_SIZE = 1024 * 1024 * 256;

int initLog()
{
  LOG_FILE_NAME = getNewLogFileName();

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

  LOG_FILE = nullptr;

  free(LOG_FILE_NAME);

  LOG_FILE_NAME = nullptr;
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

      if (LOG_FILE == nullptr)
        {
          LOG_LEVEL = 0x00;

          return nullptr;
        }

      setvbuf(LOG_FILE, nullptr, _IOFBF, 0);

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


static size_t getFileSize(const char *filename)
{
  assert(filename != nullptr);

  struct stat temp = {};

  stat(filename, &temp);

  return (size_t)temp.st_size;
}
