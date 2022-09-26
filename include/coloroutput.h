#ifndef COLOROUTPUT_H_
#define COLOROUTPUT_H_

enum Colors {
  BLACK         ,
  RED           ,
  GREEN         ,
  YELLOW        ,
  BLUE          ,
  MAGENTA       ,
  CYAN          ,
  WHITE         ,
  BRIGHT_BLACK  ,
  BRIGHT_RED    ,
  BRIGHT_GREEN  ,
  BRIGHT_YELLOW ,
  BRIGHT_BLUE   ,
  BRIGHT_MAGENTA,
  BRIGHT_CYAN   ,
  BRIGHT_WHITE  ,
};

enum FontStyle {
  NORMAL          ,
  BOLD            ,
  FAINT           ,
  ITALIC          ,
  UNDERLINE       ,
  BLINK           ,
  INVERT_COLORS   ,
  DOUBLY_UNDERLINE,
};

void setForegroundColor(int color);

void setBackgroundColor(int color);

void setFontStyle(int style);

void resetConsole();

#endif
