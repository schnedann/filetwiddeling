#include "terminal.h"

#include <utility>


#include "enviroment_detection.h"

#if (0<dIS_LINUX)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#if (0<dIS_WINDOWS)
#include <windows.h>
#endif

std::pair<size_t,size_t> Terminal::size(){

#if (0<dIS_LINUX)
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  size_t width  = w.ws_col;
  size_t height = w.ws_row;
#endif

#if (0<dIS_WINDOWS)
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

  size_t width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
  size_t height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

  return {width,height};
}
