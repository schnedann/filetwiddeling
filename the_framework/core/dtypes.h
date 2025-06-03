/******************************************************************************/
/** Compact Datatype Definitions                                             **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2022                                               **/
/******************************************************************************/
#ifndef DTYPES_H
#define DTYPES_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#ifdef __cplusplus
  #include <cstdint>
  #include <cstddef>
#include <array>

//--------------------------------------------------
// normal Types
//--------------------------------------------------

using  u8 =  uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using  s8 =  int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

//--------------------------------------------------
// fast Types
//--------------------------------------------------

using  uf8 = uint_fast8_t ;
using uf16 = uint_fast16_t;
using uf32 = uint_fast32_t;
using uf64 = uint_fast64_t;

using  sf8 =  int_fast8_t ;
using sf16 =  int_fast16_t;
using sf32 =  int_fast32_t;
using sf64 =  int_fast64_t;

//--------------------------------------------------
// unions for several unsigned types
//--------------------------------------------------

typedef union cutype16_u{
  std::array<u8,2> a;
  u16 b;
}cutype16_T;

typedef union cutype32_u{
  std::array<u8,4>  a;
  std::array<u16,2> b;
  u32 c;
}cutype32_T;

typedef union cutype64_u{
  std::array<u8,8>  a;
  std::array<u16,4> b;
  std::array<u32,2> c;
  u64 d;
}cutype64_T;

#else
  #include <stdint.h>
  #include <stddef.h>

//--------------------------------------------------
// normal Types
//--------------------------------------------------

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

//--------------------------------------------------
// fast Types
//--------------------------------------------------

typedef uint_fast8_t   uf8;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;

typedef  int_fast8_t    sf8;
typedef  int_fast16_t  sf16;
typedef  int_fast32_t  sf32;
typedef  int_fast64_t  sf64;

//--------------------------------------------------
// unions for several unsigned types
//--------------------------------------------------

typedef union cutype16_u{
  u8  a[2];
  u16 b;
}cutype16_T;

typedef union cutype32_u{
  u8  a[4];
  u16 b[2];
  u32 c;
}cutype32_T;

typedef union cutype64_u{
  u8  a[8];
  u16 b[4];
  u32 c[2];
  u64 d;
}cutype64_T;

#endif

//--------------------------------------------------

#endif
