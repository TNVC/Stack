#ifndef LOGGING_H_
#define LOGGING_H_

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME "log.txt"
#endif

extern FILE *LOG_FILE;

extern int   LOG_LEVEL;

int initLog();

void destroyLog();

#endif
