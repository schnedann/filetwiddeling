#ifndef POSIX_DETECT_H
#define POSIX_DETECT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

//Code by https://sourceforge.net/p/predef/wiki/Standards/

#if defined(unix) || defined(__unix__) || defined(__unix)
# define PREDEF_PLATFORM_UNIX
#endif
#if defined(PREDEF_PLATFORM_UNIX)
# include <unistd.h>
# if defined(_XOPEN_VERSION)
#  if (_XOPEN_VERSION >= 3)
#   define PREDEF_STANDARD_XOPEN_1989
#  endif
#  if (_XOPEN_VERSION >= 4)
#   define PREDEF_STANDARD_XOPEN_1992
#  endif
#  if (_XOPEN_VERSION >= 4) && defined(_XOPEN_UNIX)
#   define PREDEF_STANDARD_XOPEN_1995
#  endif
#  if (_XOPEN_VERSION >= 500)
#   define PREDEF_STANDARD_XOPEN_1998
#  endif
#  if (_XOPEN_VERSION >= 600)
#   define PREDEF_STANDARD_XOPEN_2003
#  endif
#  if (_XOPEN_VERSION >= 700)
#   define PREDEF_STANDARD_XOPEN_2008
#  endif
# endif
#endif

#endif // POSIX_DETECT_H
