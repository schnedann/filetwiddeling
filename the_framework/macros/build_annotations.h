#ifndef BUILD_ANNOTATIONS_H
#define BUILD_ANNOTATIONS_H

//for Copyright see this file
#include "modified_bsd.h"

#include "global_config.h"
#include "standard_detect.h"
#include "needful_macros.h"

#define GNU_UNUSED __attribute__((unused))

#define PREFIX_UNUSED(_x) UNUSED##_x

#define UNUSED_PARA(_x) PREFIX_UNUSED(_x) GNU_UNUSED

#define UNUSED_VAR(_x) PREFIX_UNUSED(_x) GNU_UNUSED

#define UNUSED_RETVAL(...) static_cast<void>(__VA_ARGS__);

//----- constexpr backward compatibility

#if (0<dIS_LEAST_CXX11)
  #define CONSTEXPR11 constexpr
#else
  #define CONSTEXPR11
#endif //(IS_LEAST_CXX11)

#if (0<dIS_LEAST_CXX14)
  #define CONSTEXPR14 CONSTEXPR11
#else
  #define CONSTEXPR14
#endif //(IS_LEAST_CXX14)

#if (0<dIS_LEAST_CXX17)
  #define CONSTEXPR17 CONSTEXPR14
#else
  #define CONSTEXPR17
#endif //(IS_LEAST_CXX17)

#if (0<dIS_LEAST_CXX20)
  #define CONSTEXPR20 CONSTEXPR17
#else
  #define CONSTEXPR20
#endif //(IS_LEAST_CXX20)

#ifndef DBGCONSTEXPR
  #define DBGCONSTEXPR 0
  #pragma message(MSG_INFORM "defining DBGCONSTEXPR = 0")
#endif

#if (0 == DBGCONSTEXPR)
  #define CONSTEXPR CONSTEXPR11
#else
  #define CONSTEXPR
  #pragma message(MSG_INFORM "Constexpr is turned of for Debugging!")
#endif
//-----

#if (0<dIS_LEAST_CXX17)
  #define NODISCARD [[nodiscard]]
#else
  #define NODISCARD
#endif //(IS_LEAST_CXX17)

#if (0<dIS_LEAST_CXX14)
  #define DEPRECATED [[deprecated]]
#else
  #define DEPRECATED
#endif //(IS_LEAST_CXX14)

#if (0<dIS_LEAST_CXX20)
  #define LIKELY   [[likely]]
  #define UNLIKELY [[unlikely]]
#else
  #define LIKELY
  #define UNLIKELY
#endif //(IS_LEAST_CXX20)

//-----

#endif // BUILD_ANNOTATIONS_H
