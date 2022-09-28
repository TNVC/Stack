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
  NORMAL          , // Reset style and color
  BOLD            ,
  FAINT           ,
  ITALIC          ,
  UNDERLINE       ,
  BLINK           ,
  INVERT_COLORS   ,
  DOUBLY_UNDERLINE,
};

const int COLORS_COUNT = 15;
const int STYLES_COUNT =  8;

/// Set foreground color
/// @param [in] color Code of color
void setForegroundColor(int color);

/// Set background color
/// @param [in] color Code of color
void setBackgroundColor(int color);

/// Set font style
/// @param [in] style Code of style
void setFontStyle(int style);

/// Set consol output to default
void resetConsole();

#endif
