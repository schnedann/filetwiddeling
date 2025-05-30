/******************************************************************************/
/** Configuration Flags                                                      **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2022                                               **/
/******************************************************************************/

//for Copyright see this file
#include "modified_4Cbsd.h"

#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#define dUSE_ENDIANESS_AS_STRING 1
#define dUSE_DEBUG_CODE   0
#define dUSE_DEBUG_OUTPUT 1
#define DBGCONSTEXPR      0
#define dUSE_AVOIDMODULO  0

#define dDEBUG_OUTPUT            0
#define dDEBUG_CODE              0
#define dUSE_ALTVERSIONS         0
#define dUSE_OPTIMIZED           0

#define dUSE_INSSORT_OPT         1
#define dUSE_MEDIANOFTHREE       1

#if defined(__cplusplus)

namespace Global_Config {

  static constexpr bool const cUSE_ENDIANESS_AS_STRING = (0<dUSE_ENDIANESS_AS_STRING );

  static constexpr bool const cUSE_DBGOUT              = (0<dDEBUG_OUTPUT            );
  static constexpr bool const cUSE_NPOS                = true;
  static constexpr bool const cUSE_DEBUG_CODE          = (0<dDEBUG_CODE              );
  static constexpr bool const cUSE_ALTVERSIONS         = (0<dUSE_ALTVERSIONS         );
  static constexpr bool const cUSE_OPTIMIZED           = (0<dUSE_OPTIMIZED           );

  static constexpr bool const cUSE_INSSORT_OPT         = (0<dUSE_INSSORT_OPT         );
  static constexpr bool const cUSE_MEDIANOFTHREE       = (0<dUSE_MEDIANOFTHREE       );
}

#endif

#endif // GLOBAL_CONFIG_H
