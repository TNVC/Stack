#ifndef LOGGING_H_
#define LOGGING_H_

#ifndef LOG_DIRECTORY
#define LOG_DIRECTORY "log/"
#endif

#ifndef LOG_FILE_PREFIX
#define LOG_FILE_PREFIX "log"
#endif

#ifndef LOG_FILE_SUFFIX
#define LOG_FILE_SUFFIX "txt"
#endif

extern int LOG_LEVEL;

/// Init logs
/// @note Autocallable
int initLog();

/// Destroy logs
/// @note Autocallable
void destroyLog();

/// Getter for LOG_FILE
/// @return LOG_FILE or NULL if fail to open file
/// @note If log file bigger than 1GB close it and open new file and save descriptor in LOG_FILE\n
/// If was error in open file set LOG_LEVEL to 0
FILE *getLogFile();

#endif
