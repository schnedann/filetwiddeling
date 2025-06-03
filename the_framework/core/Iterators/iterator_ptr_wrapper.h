/******************************************************************************/
/** Compact Datatype Definitions                                             **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2021                                               **/
/******************************************************************************/

#ifndef ITERATOR_PTR_WRAPPER_H
#define ITERATOR_PTR_WRAPPER_H

#include <algorithm>
#include <memory>

//for Copyright see this file
#include "modified_bsd.h"

#include "dtypes.h"
#include "compile_guards.h"
#include "bit_func.h"
#include "fields.h"

namespace Iterators {

template<typename I> static constexpr I const npos = Fields::npos<I>;

//--------------------------------------------------
// Convinient Pointers --> lean Iterators
//--------------------------------------------------

template<typename T> using  iterator_t = T*;
template<typename T> using citerator_t = T const *;

template<typename T> iterator_t<T> make_iterator(T& var){
  return static_cast<iterator_t<T>>(&var);
}

template<typename T> citerator_t<T> make_citerator(T const& var){
  return static_cast<citerator_t<T>>(&var);
}

//--------------------------------------------------
// C-String lean Iterators
//--------------------------------------------------

using cstr_it  =  iterator_t<char>;
using cstr_cit = citerator_t<char>;

//--------------------------------------------------
// void lean Iterators
//--------------------------------------------------

using void_it  =  iterator_t<void>;
using void_cit = citerator_t<void>;

//--------------------------------------------------
// byte lean Iterators
//--------------------------------------------------

using byte_it  =  iterator_t<u8>;
using byte_cit = citerator_t<u8>;

//--------------------------------------------------
// have owning and pointing Pointers
//--------------------------------------------------

template<typename T> using owning_ptr_t = std::unique_ptr<T>;
template<typename T> using weak_ptr_t   =  iterator_t<T>;
template<typename T> using cweak_ptr_t  = citerator_t<T>;

template<typename T> weak_ptr_t<T> make_owning_ptr(T const& obj){
  return std::make_unique<T>(obj);
}

template<typename T> weak_ptr_t<T> to_waek(owning_ptr_t<T> const& oit){
  return oit.get();
}

template<typename T> weak_ptr_t<T> make_weak_ptr(T& var){
  return static_cast<weak_ptr_t<T>>(&var);
}

template<typename T> cweak_ptr_t<T> make_cweak_ptr(T const& var){
  return static_cast<cweak_ptr_t<T>>(&var);
}

//--------------------------------------------------
// Cast Iterator from Type T to new type NT
//--------------------------------------------------

template<typename T, typename NT> iterator_t<NT> itcast(iterator_t<T> const it){
  return reinterpret_cast<iterator_t<NT>>(it);
}
template<typename T, typename NT> citerator_t<NT> itcast(citerator_t<T> const cit){
  return reinterpret_cast<citerator_t<NT>>(cit);
}

//-----

template<typename T> byte_it byte_itcast(iterator_t<T> const it){
  return reinterpret_cast<byte_it>(it);
}
template<typename T> byte_cit byte_itcast(citerator_t<T> const cit){
  return reinterpret_cast<byte_cit>(cit);
}

//--------------------------------------------------
// Cast Iterator from Type T to uintptr_t
//               and via versa
//--------------------------------------------------

template<typename T> uintptr_t it2uintcast(iterator_t<T> const it){
  return reinterpret_cast<uintptr_t>(it);
}
template<typename T> uintptr_t it2uintcast(citerator_t<T> const it){
  return reinterpret_cast<uintptr_t>(it);
}

template<typename T> iterator_t<T> uint2itcast(uintptr_t const uint_it){
  return reinterpret_cast<iterator_t<T>>(uint_it);
}
template<typename T> citerator_t<T> uint2citcast(uintptr_t const uint_it){
  return reinterpret_cast<citerator_t<T>>(uint_it);
}

//--------------------------------------------------
// distance between Iterators
//--------------------------------------------------

/*
 * @brief calculate difference between 2 Iterators (Pointers)
 *        if b_it is left of a_it, the result is automaticly corrected
 * @param a_it - 1st Iterator / Pointer
 * @param b_it - 2nd Iterator / Pointer
 * @return distance between Iterators a_it and b_it
 */
template<typename T> size_t get_distance(citerator_t<T> const a_it,citerator_t<T> const b_it){
  //Convert Pointers to unsigned Integers
  auto const uintptr_a = it2uintcast<T>(a_it);
  auto const uintptr_b = it2uintcast<T>(b_it);
  //Calculate Distance
  auto res = uintptr_a - uintptr_b;
  //Detect Overflow
  if(res > uintptr_a){
    res = Math::Boolean::ZQ<uintptr_t>(res);
  }
  return size_t(res);
}

template<typename T1, typename T2, typename I> bool is_overlap(citerator_t<T1> const a_it, Fields::field_sl<I> a_fsl,
                citerator_t<T2> const b_it, Fields::field_sl<I> b_fsl){
  Compile::Guards::IsUnsigned<I>();
  static constexpr size_t const type_bytes1 = sizeof(T1);
  static constexpr size_t const type_bytes2 = sizeof(T2);
  //Convert Pointers to unsigned Integers
  auto const uintptr_a = it2uintcast<T1>(a_it);
  auto const uintptr_b = it2uintcast<T2>(b_it);
  auto start_a = uintptr_a + uintptr_t(type_bytes1 * Fields::get_start<I>(a_fsl));
  auto start_b = uintptr_b + uintptr_t(type_bytes2 * Fields::get_start<I>(b_fsl));
  auto end_a   = uintptr_a + uintptr_t(type_bytes1 * Fields::get_end<I>(a_fsl));
  auto end_b   = uintptr_b + uintptr_t(type_bytes2 * Fields::get_end<I>(b_fsl));

  bool ovlp = true;

  if(end_a<=start_b){
    ovlp = false;
  }
  if(end_b<=start_a){
    ovlp = false;
  }

  return ovlp;
}

/**
 * @brief is_overlap
 * @param a_it
 * @param a_fsl
 * @param b_it
 * @param b_fsl
 *
 *  (a)______S....E
 *        (b)_____________S.........E
 *        Start_a
 *              End_a
 *                     Start_b
 *                                 End_b
 *
 * @return
 */
template<typename T, typename I> bool is_overlap(citerator_t<T> const a_it, Fields::field_sl<I> a_fsl,
                                                 citerator_t<T> const b_it, Fields::field_sl<I> b_fsl){
  Compile::Guards::IsUnsigned<I>();
  return is_overlap<T,T,I>(a_it,a_fsl,b_it,b_fsl);
}

/**
 * @brief is_back2back
 * @param a_it	- 1st Iterator (base of first field)
 * @param a_fsl	- field start-length relative to 1st Iterator
 * @param b_it	- 2nd Iterator (base of second field)
 * @param b_fsl - field start-length relative to 2nd Iterator
 * @return true if both fields do not overlap but touch
 */
template<typename T, typename I> bool is_back2back(citerator_t<T> const a_it, Fields::field_sl<I> a_fsl,
                                                   citerator_t<T> const b_it, Fields::field_sl<I> b_fsl){
  Compile::Guards::IsUnsigned<I>();
  //Convert Pointers to unsigned Integers
  auto const uintptr_a = it2uintcast<T>(a_it);
  auto const uintptr_b = it2uintcast<T>(b_it);
  auto start_a = uintptr_a + uintptr_t(Fields::get_start<I>(a_fsl));
  auto start_b = uintptr_b + uintptr_t(Fields::get_start<I>(b_fsl));
  auto end_a   = uintptr_a + uintptr_t(Fields::get_end<I>(a_fsl));
  auto end_b   = uintptr_b + uintptr_t(Fields::get_end<I>(b_fsl));

  bool touchting = false;

  if(end_a==start_b){
    touchting = true;
  }
  if(end_b==start_a){
    touchting = true;
  }

  return touchting;
}

//--------------------------------------------------
// convert a non const Iterator to a const Iterator
//--------------------------------------------------

template<typename T> citerator_t<T> constify(iterator_t<T> const it){
  return reinterpret_cast<citerator_t<T>>(it);
}

//--------------------------------------------------
// test Iterator not null
//--------------------------------------------------

template<typename T> bool is_valid(citerator_t<T> const cit){
  return (nullptr != cit);
}
/*template<typename T> bool is_valid(iterator_t<T> const it){
  return is_valid<T>(constify<T>(it));
}*/

template<typename T> constexpr iterator_t<T> init_invalid(){
  return nullptr;
}

//--------------------------------------------------
// alignment stuff
//--------------------------------------------------

//--- Query is aligned

/**
 * @brief is_aligned - caclulate if an Iterator is properly aligned
 * @param uiptr
 * @note AT is the Alignment Type (an system integer type)
 * @return true if iterator is properly aligned with type AT
 */
template<typename AT> bool is_aligned(uintptr_t const uiptr){
  bool res = true;
  constexpr static u8 const TBytes = sizeof(AT);

  //Bytes are always aligned...
  if(TBytes>1){
    //We expect any not Byte alligned Pointer to be even
    res &= Math::Boolean::is_even<uintptr_t>(uiptr);
    //We expect any alignment to be OneHot, so the Masktest succeeds
    res &= Math::Boolean::is_onehot<u8>(TBytes);
    //All Bits lower than MSB of TBytes needs to be zero
    //If TBytes is OneHot, then TBytes-1 is a mask with all lower bits set
    // e.g. 64 Bit = 8 Byte, so any address with lower bits representing 1...7 are not aligned
    res &= (0==Math::Boolean::maskbits<uintptr_t>(uiptr,uintptr_t(TBytes-1)));
  }
  return res;
}

/**
 * @brief is_aligned - caclulate if an Iterator is properly aligned
 * @param it
 * @note AT is the Alignment Type (an system integer type)
 * @return true if iterator is properly aligned with type AT
 */
template<typename T, typename AT> bool is_aligned(citerator_t<T> const cit){
  uintptr_t const uiptr = it2uintcast(cit);
  return is_aligned<AT>(uiptr);
}

/**
 * @brief is_aligned - caclulate if an Iterator is properly aligned
 * @param it
 * @note AT is the Alignment Type (an system integer type)
 * @return true if iterator is properly aligned with type AT
 */
template<typename T, typename AT> bool is_aligned(iterator_t<T> const it){
  uintptr_t const uiptr = it2uintcast(it);
  return is_aligned<AT>(uiptr);
}

//--- Query Alignment Offset in Bytes

/**
 * @brief align_offset - caclulate alignment offset in bytes
 * @param uiptr
 * @return for an X Byte Type the result is 0..(X-1)
 */
template<typename AT> u8 align_offset(uintptr_t const uiptr){
  u8 res = 0;
  constexpr static u8 const TBytes = sizeof(AT);
  res = Math::Boolean::maskbits<uintptr_t>(uiptr,uintptr_t(TBytes-1));
  return u8(res);
}

/**
 * @brief align_offset - caclulate alignment offset in bytes
 * @param it
 * @return for an X Byte Type the result is 0..(X-1)
 */
template<typename T, typename AT> u8 align_offset(citerator_t<T> const cit){
  uintptr_t const uiptr = it2uintcast(cit);
  return align_offset<AT>(uiptr);
}

/**
 * @brief align_offset - caclulate alignment offset in bytes
 * @param it
 * @return for an X Byte Type the result is 0..(X-1)
 */
template<typename T, typename AT> u8 align_offset(iterator_t<T> const it){
  uintptr_t const uiptr = it2uintcast(it);
  return align_offset<AT>(uiptr);
}

//--- Get smaller aligned Adress (floor behaviour) by zeroing lower bits of address

/**
 * @brief floor_align - Align Iterator to next lower aligned adress
 * @param it
 * @return
 */
template<typename AT> iterator_t<AT> floor_align(iterator_t<AT> const it){
  constexpr static u8 const TBytes = sizeof(AT);
  return iterator_t<AT>(Math::Boolean::maskbits<uintptr_t>(Iterators::it2uintcast<AT>(it),uintptr_t(~(TBytes-1))));
}

/**
 * @brief floor_align - Align Iterator to next lower aligned adress
 * @param cit
 * @return
 */
template<typename AT> citerator_t<AT> floor_align(citerator_t<AT> const cit){
  constexpr static u8 const TBytes = sizeof(AT);
  return citerator_t<AT>(Math::Boolean::maskbits<uintptr_t>(Iterators::it2uintcast<AT>(cit),uintptr_t(~(TBytes-1))));
}

//--------------------------------------------------
// Get / Set via Iterator
//--------------------------------------------------

/**
 * @brief set_at() - Set Data at Iterator-Offset
 * @return true on Error
 */
template<typename T, typename I> bool set_at(iterator_t<T> const it, I const offset, T const& data){
  Compile::Guards::IsUnsigned<I>();
  bool OK  = is_valid<T>(it);
       OK &= (offset<npos<I>);
  if(OK){
    it[offset] = data;
  }
  return !OK;
}

//--------------------------------------------------

/**
 * @brief ref_at
 * @param cit
 * @param offset
 * @return
 */
template<typename T, typename I> T const& ref_at(citerator_t<T> const cit, I const offset){
  Compile::Guards::IsUnsigned<I>();
  return cit[offset];
}

/**
 * @brief set - Set Element at Iterator / Pointer
 * @param it
 * @param val
 * @return
 */
template<typename T> bool set(iterator_t<T> const it, T const val){
  bool OK = is_valid(it);
  if(OK){
    *it = val;
  }
  return !OK;
}

/**
 * @brief get - Get Element at Iterator / Pointer
 * @param cit
 * @return
 */
template<typename T> T get(citerator_t<T> const cit){
  return (is_valid(cit))?(*cit):(T());
}

//forward decl
template<typename T, typename I> citerator_t<T> advance(citerator_t<T> const cit, I const offset);

/**
 * @brief get_at - Get Data from Iterator-Offset
 * @param cit
 * @param offset
 * @return
 */
template<typename T, typename I> T get_at(citerator_t<T> const cit, I const offset){
  Compile::Guards::IsUnsigned<I>();
  return get<T>(Iterators::advance<T,I>(cit,offset));
}

template<typename T, typename I> T& get_ref_at(iterator_t<T> const it, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto moved_it = Iterators::advance(it,offset);
  return *moved_it;
}

template<typename T, typename I> T const& get_ref_at(citerator_t<T> const cit, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto moved_it = Iterators::advance(cit,offset);
  return *moved_it;
}

//--------------------------------------------------
// Move an Iterator
//--------------------------------------------------

/**
 * @brief advance
 * @param cit
 * @param offset
 * @return
 */
template<typename T, typename I> citerator_t<T> advance(citerator_t<T> const cit, I const offset){
  Compile::Guards::IsUnsigned<I>();
  return &cit[offset];
}

/**
 * @brief advance
 * @param it
 * @param offset
 * @return
 */
template<typename T, typename I> iterator_t<T> advance(iterator_t<T> const it, I const elements){
  Compile::Guards::IsUnsigned<I>();
  return &it[elements];
}

/**
 * @brief radvance
 * @param cit
 * @param offset
 * @return
 */
template<typename T, typename I> citerator_t<T> radvance(citerator_t<T> const cit, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto cpy_cit = cit;
  cpy_cit -= reinterpret_cast<citerator_t<T>>(offset);
  return cpy_cit;
}

/**
 * @brief radvance
 * @param it
 * @param offset
 * @return
 */
template<typename T, typename I> iterator_t<T> radvance(iterator_t<T> const it, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto cpy_it = it;
  cpy_it -= reinterpret_cast<iterator_t<T>>(offset);
  return cpy_it;
}

//--------------------------------------------------

} //namespace

#endif // ITERATOR_PTR_WRAPPER_H
