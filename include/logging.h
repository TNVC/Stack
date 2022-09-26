/// @brief Define one of next defination for declarate log level:\n
/// RELEASE_LOG_LEVEL - logging only fatal errors\n
/// ERROR_LOG_LEVEL   - logging only errors\n
/// MESSAGE_LOG_LEVEL - logging errors and messages\n
/// VALUE_LOG_LEVEL   - logging errors, messages and values
///
#ifndef LOGGING_H_
#define LOGGING_H_

#define LOG_INFO(EXPRESSION) #EXPRESSION, __FILE__, __func__, __LINE__

#ifndef LOG_DIRECTORY
#define LOG_DIRECTORY "log/"
#endif

#ifndef LOG_FILE_PREFIX
#define LOG_FILE_PREFIX "log"
#endif

#ifndef LOG_FILE_SUFFIX
#define LOG_FILE_SUFFIX "txt"
#endif

unsigned enum LogLevel {
  FATAL   = (0x01 << 0),
  ERROR   = (0x01 << 1),
  WARNING = (0x01 << 2),
  MESSAGE = (0x01 << 3),
  VALUE   = (0x01 << 4),
};

extern unsigned LOG_LEVEL;

/// Init logs
/// @note Autocallable
unsigned initLog();

/// Destroy logs
/// @note Autocallable
void destroyLog();

/// Getter for LOG_FILE
/// @return LOG_FILE or NULL if fail to open file
/// @note If log file bigger than 1GB close it and open new file and save descriptor in LOG_FILE\n
/// If was error in open file set LOG_LEVEL to 0
FILE *getLogFile();

#ifndef RELEASE_BUILD_

#define logValue(value)                                     \
  loggingPrint(value      , LOG_INFO(value)     , VALUE)

#define logMessage(message)                                 \
  loggingPrint(message    , LOG_INFO(message)   , MESSAGE)

#define logWarning(warning)                                 \
  logggingPrint(#warning  , LOG_INFO(warning)   , WARNING)

#define logError(expression)                                \
  loggingPrint(#expression, LOG_INFO(expression), ERROR)

#else

#define logValue(value)      ;

#define logMessage(message)  ;

#define logWarning(warning)  ;

#define logError(expression) ;

#endif

#define logFatal(expression)                              \
  loggingPrint(#expression, LOG_INFO(expression), FATAL)


/// Print log info for decimal
/// @param [in] value Deciamal value
/// @param [in] name C-like string with value
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function wher was call function
/// @param [in] line Number of line where was call function
/// @param [in] level Level of current log`s print
/// @return Count of print chars
int loggingPrint(long long value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level);

/// Print log info for double
/// @param [in] value Double value
/// @param [in] name C-like string with value
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function wher was call function
/// @param [in] line Number of line where was call function
/// @param [in] level Level of current log`s print
/// @return Count of print chars
int loggingPrint(double value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level);

/// Print log info for char
/// @param [in] value Char value
/// @param [in] name C-like string with value
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function wher was call function
/// @param [in] line Number of line where was call function
/// @param [in] level Level of current log`s print
/// @return Count of print chars
int loggingPrint(char value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level);

/// Print log info for pointer
/// @param [in] value Pointer value
/// @param [in] name C-like string with value
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function wher was call function
/// @param [in] line Number of line where was call function
/// @param [in] level Level of current log`s print
/// @return Count of print chars
int loggingPrint(const void *value,const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level);

/// Print log info for C-like string
/// @param [in] value C-like string value
/// @param [in] name C-like string with value
/// @param [in] fileName Name of file where was call function
/// @param [in] functionName Name of function wher was call function
/// @param [in] line Number of line where was call function
/// @param [in] level Level of current log`s print
/// @return Count of print chars
int loggingPrint(const char *value, const char *name,
                 const char *fileName, const char *functionName, int line, unsigned level);

#endif
