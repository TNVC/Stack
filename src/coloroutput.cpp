#include <stdio.h>
#include "coloroutput.h"

#pragma GCC diagnostic ignored "-Wformat-security"

const char *FOREGROUND_COLOR_CODES[] = {
    "\033[30m",
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m",
    "\033[90m",
    "\033[91m",
    "\033[92m",
    "\033[93m",
    "\033[94m",
    "\033[95m",
    "\033[96m",
    "\033[97m",
};

const char *BACKGROUND_COLOR_CODES[] = {
  "\033[40m" ,
  "\033[41m" ,
  "\033[42m" ,
  "\033[43m" ,
  "\033[44m" ,
  "\033[45m" ,
  "\033[46m" ,
  "\033[47m" ,
  "\033[100m",
  "\033[101m",
  "\033[102m",
  "\033[103m",
  "\033[104m",
  "\033[105m",
  "\033[106m",
  "\033[107m",
};


const char *FONT_STYLE_CODES[] = {
  "\033[0m" ,
  "\033[1m" ,
  "\033[2m" ,
  "\033[3m" ,
  "\033[4m" ,
  "\033[5m" ,
  "\033[7m" ,
  "\033[21m",
};

void setForegroundColor(int color)
{
  if (color < 0 || color > 15)
    return;

  printf(FOREGROUND_COLOR_CODES[color]);
}

void setBackgroundColor(int color)
{
  if (color < 0 || color > 15)
    return;

  printf(BACKGROUND_COLOR_CODES[color]);
}

void setFontStyle(int style)
{
  if (style < 0 || style > 8)
    return;

  printf(FONT_STYLE_CODES[style]);
}

void resetConsole()
{
  printf("\033[0m");
}
