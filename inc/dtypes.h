#ifndef DTYPES_H
#define DTYPES_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include <cstdint>
#include <cstddef>
#include <array>

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

typedef union cutype16_u{
  std::array<u8,2> a;
  u16 b;
}cutype16_t;

typedef union cutype32_u{
  std::array<u8,4>  a;
  std::array<u16,2> b;
  u32 c;
}cutype32_t;

typedef union cutype64_u{
  std::array<u8,8>  a;
  std::array<u16,4> b;
  std::array<u32,2> c;
  u64 d;
}cutype64_t;

//--------------------------------------------------

#endif
