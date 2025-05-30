/******************************************************************************/
/** Operatin System Enviroment Detection                                     **/
/** see predefined macros @ Sourceforge                                      **/
/** https://sourceforge.net/p/predef/wiki/VersionNormalization/              **/
/** Danny Schneider, 2017-2020                                               **/
/******************************************************************************/

#ifndef OS_DETECT_H
#define OS_DETECT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include "dtypes.h"

#define dIS_WINDOWS 0
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__))
  #undef dIS_WINDOWS
  #define dIS_WINDOWS 1
#endif

#define dIS_UNIX 0
#if (defined(__unix__) || defined(__unix))
  #undef dIS_UNIX
  #define dIS_UNIX 1
#endif

#define dIS_LINUX 0
#if (defined(__linux__) || defined(__linux))
  #undef dIS_LINUX
  #define dIS_LINUX 1
#endif

#define dIS_MINGW 0
#if ((defined(__MINGW32__) || defined(__MINGW64__)) && (0<dIS_WINDOWS))
  #undef dIS_MINGW
  #define dIS_MINGW 1
#endif

#define dIS_POSIX 0
#if (defined(_POSIX_VERSION) || (0<dIS_UNIX))
  #undef dIS_POSIX
  #define dIS_POSIX 1
#endif

namespace Enviroment{

namespace OS {

static constexpr bool const is_Windows = (0<dIS_WINDOWS);
static constexpr bool const is_Unix    = (0<dIS_UNIX);
static constexpr bool const is_Linux   = (0<dIS_LINUX);
static constexpr bool const is_MinGW   = (0<dIS_MINGW);

} // namespace Compiler

} //namespace

#endif //OS_DETECT_H
