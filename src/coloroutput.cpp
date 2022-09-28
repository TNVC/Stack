#include <stdio.h>
#include "coloroutput.h"

#pragma GCC diagnostic ignored "-Wformat-security"

const char *FOREGROUND_COLOR_CODES[] = {
  "\033[30m", // BLACK
  "\033[31m", // RED
  "\033[32m", // GREEN
  "\033[33m", // YELLOW
  "\033[34m", // BLUE
  "\033[35m", // MAGENTA
  "\033[36m", // CYAN
  "\033[37m", // WHITE
  "\033[90m", // BRIGHT_BLACK
  "\033[91m", // BRIGHT_RED
  "\033[92m", // BRIGHT_GREEN
  "\033[93m", // BRIGHT_YELLOW
  "\033[94m", // BRIGHT_BLUE
  "\033[95m", // BRIGHT_MAGENTA
  "\033[96m", // BRIGHT_CYAN
  "\033[97m", // BRIGHT_WHITE
};

const char *BACKGROUND_COLOR_CODES[] = {
  "\033[40m" , // BLACK
  "\033[41m" , // RED
  "\033[42m" , // GREEN
  "\033[43m" , // YELLOW
  "\033[44m" , // BLUE
  "\033[45m" , // MAGENTA
  "\033[46m" , // CYAN
  "\033[47m" , // WHITE
  "\033[100m", // BRIGHT_BLACK
  "\033[101m", // BRIGHT_RED
  "\033[102m", // BRIGHT_GREEN
  "\033[103m", // BRIGHT_YELLOW
  "\033[104m", // BRIGHT_BLUE
  "\033[105m", // BRIGHT_MAGENTA
  "\033[106m", // BRIGHT_CYAN
  "\033[107m", // BRIGHT_WHITE
};


const char *FONT_STYLE_CODES[] = {
  "\033[0m" , // NORMAL
  "\033[1m" , // BOLD
  "\033[2m" , // FAINT
  "\033[3m" , // ITALIC
  "\033[4m" , // UNDERLINE
  "\033[5m" , // BLINK
  "\033[7m" , // INVERT_COLORS
  "\033[21m", // DOUBLY_UNDERLINE
};

void setForegroundColor(int color)
{
  if (color < 0 || color > COLORS_COUNT)
    return;

  printf(FOREGROUND_COLOR_CODES[color]);
}

void setBackgroundColor(int color)
{
  if (color < 0 || color > COLORS_COUNT)
    return;

  printf(BACKGROUND_COLOR_CODES[color]);
}

void setFontStyle(int style)
{
  if (style < 0 || style > STYLES_COUNT)
    return;

  printf(FONT_STYLE_CODES[style]);
}

void resetConsole()
{
  printf("\033[0m");
}
