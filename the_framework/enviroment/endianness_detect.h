/**
 * Detect endianness at run-time
 * https://sourceforge.net/p/predef/wiki/Endianness/
 */

#ifndef ENDIANNESS_DETECT_H
#define ENDIANNESS_DETECT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include <cstdint>

#include "global_config.h"
#include "build_annotations.h"

#if (0<dUSE_ENDIANESS_AS_STRING)
  #include <string>
#endif

namespace Enviroment {

namespace Endianness {

  enum class endianness_t:uint8_t{
    ENDIAN_UNKNOWN = 0,
    ENDIAN_BIG,
    ENDIAN_LITTLE,
    ENDIAN_BIG_WORD,   /* Middle-endian, Honeywell 316 style */
    ENDIAN_LITTLE_WORD /* Middle-endian, PDP-11 style */
  };

  extern endianness_t const detected;

  bool is_little();
  bool is_big();

#if (0<dUSE_ENDIANESS_AS_STRING)
  std::string get_endianess_as_string();
#endif

} //namespace

} //namespace

#endif // ENDIANNESS_DETECT_H
