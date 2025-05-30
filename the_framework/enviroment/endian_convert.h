#ifndef ENDIAN_CONVERT_H
#define ENDIAN_CONVERT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include "dtypes.h"
#include "compile_guards.h"
#include "compiler_detect.h"
#include "bit_func.h"
#include "iterator_ptr_wrapper.h"

namespace Enviroment {

namespace Endianness {

/**
 * @brief swap - Interchange the two halfs of an Integer (byte granularity)
 * @param px
 * @return 0xAB                               -> 0xAB
 *         0xAABB                             -> 0xBBAA
 *         0xAABBCCDD                         -> 0xCCDDAABB
 *         0xAABBCCDDEEFFGGHH                 -> 0xEEFFGGHHAABBCCDD
 *         0xAABBCCDDEEFFGGHHIIJJKKLLMMNNOOPP -> IIJJKKLLMMNNOOPPAABBCCDDEEFFGGHH
 */
template<typename T> T swap(T const px){
  ::Compile::Guards::IsUnsigned<T>();
  T res = px;
  constexpr auto const Tbytes = sizeof(T);
  if(Tbytes>1){
    if(Enviroment::Compiler::is_GNUCompiler){
      if(Tbytes>Math::Boolean::TBits_X08){
        res = __builtin_bswap128(res);
      }else if(Tbytes>4){
        res = __builtin_bswap64(res);
      }else if(Tbytes>2){
        res = __builtin_bswap32(res);
      }else{
        res = __builtin_bswap16(res);
      }
    }else{
      res = Math::Boolean::RotateLeft<T>(px,(Tbytes<<2));
    }
  }

  return res;
}

/**
 * @brief little2big
 * @param px
 * @return
 */
template<typename T> T little2big(T const px, bool const do_convert=true){
  ::Compile::Guards::IsUnsigned<T>();
  static constexpr u8 const TBits  = u8(Math::Boolean::getbitsoftype<T>());
  static constexpr u8 const hTBits = u8(Math::Boolean::getbitsoftype<T>()>>1);
  static constexpr u8 const qTBits = u8(Math::Boolean::getbitsoftype<T>()>>2);

  T res;
  //ctrl if we need to convert
  if(do_convert){
    T tmp = px;
    if(TBits > Math::Boolean::TBits_X32){ //64Bit
      auto it = Iterators::itcast<T,u32>(Iterators::make_iterator<T>(tmp));
      it[0] = little2big<u32>(it[0]);
      it[1] = little2big<u32>(it[1]);
      res = Math::Boolean::RotateLeft<u64>(tmp,hTBits);
    }else if(TBits > Math::Boolean::TBits_X16){ //32Bit
      auto it = Iterators::itcast<T,u16>(Iterators::make_iterator<T>(tmp));
      it[0] = Math::Boolean::RotateLeft<u16>(it[0],qTBits);
      it[1] = Math::Boolean::RotateLeft<u16>(it[1],qTBits);
      res = Math::Boolean::RotateLeft<u32>(tmp,hTBits);
    }else if(TBits > Math::Boolean::TBits_X08){ //16Bit
      res = Math::Boolean::RotateLeft<u16>(tmp,hTBits);
    }else{ //8Bit
      res = px;
    }
  }else{
    res = px;
  }
  return res;
}

/**
 * @brief big2little
 * @param px
 * @return
 */
template<typename T> T big2little(T const px, bool const do_convert=true){
  Compile::Guards::IsInteger<T>();
  static constexpr u8 const TBits  = u8(Math::Boolean::getbitsoftype<T>());
  static constexpr u8 const hTBits = u8(Math::Boolean::getbitsoftype<T>()>>1);
  static constexpr u8 const qTBits = u8(Math::Boolean::getbitsoftype<T>()>>2);

  T res;
  if(do_convert){
    if(TBits > Math::Boolean::TBits_X32){ //64Bit
      res = Math::Boolean::RotateLeft<u64>(px,hTBits);
      auto it = Iterators::itcast<T,u32>(Iterators::make_iterator<T>(res));
      it[0] = little2big<u32>(it[0]);
      it[1] = little2big<u32>(it[1]);
    }else if(TBits > Math::Boolean::TBits_X16){ //32Bit
      res = Math::Boolean::RotateLeft<u32>(px,hTBits);
      auto it = Iterators::itcast<T,u16>(Iterators::make_iterator<T>(res));
      it[0] = Math::Boolean::RotateLeft<u16>(it[0],qTBits);
      it[1] = Math::Boolean::RotateLeft<u16>(it[1],qTBits);
    }else if(TBits > Math::Boolean::TBits_X08){ //16Bit
      res = Math::Boolean::RotateLeft<u16>(px,hTBits);
    }else{ //8Bit
      res = px;
    }
  }else{
    res = px;
  }
  return res;
}

} //namespace

} //namespace


#endif // ENDIAN_CONVERT_H
