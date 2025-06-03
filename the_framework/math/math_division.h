#ifndef MATH_DIVISION_H
#define MATH_DIVISION_H

//for Copyright see this file
#include "modified_bsd.h"

#include "dtypes.h"
#include "compile_guards.h"

namespace Math{

namespace Division{

template<typename T> static constexpr T const ceildiv_divbyzero{0}; // X/0=0
template<typename T> static constexpr T const ceildiv_zerodivby{1}; // 0/X=1

/**
 * @brief ceildiv - Integer Division with Rouning up
 *
 * @param  x
 * @param  y
 * @note Alternate Code: return ((y>0)?(1+((x-1)/y)):(0));
 * @return
 */
template<typename T> constexpr T ceildiv(T const px, T const py){
  Compile::Guards::IsInteger<T>();
  return ((py>0)?((px>0)?((px+py-1)/py):(ceildiv_zerodivby<T>)):(ceildiv_divbyzero<T>));
}

// so far Code does not working with signed numners...
template<typename T> constexpr T ceildiv_v2(T const px, T const py){
  Compile::Guards::IsInteger<T>();
  return ((py)!=0)?(((px)>0)?(((((px)-1)/(py))+1)):((px)/(py))):(ceildiv_divbyzero<T>);
}

/**
 * @brief Fast ceil(x/y) for Integers
 *        [huge CPU's like x86 even do \ and % with the same instruction...]
 *        Warning -> Y must be greater than zero !!! no Test for valid input
 * @param x
 * @param y
 * @return
 */
template<class T> T ceildiv_v3(T const px, T const py){
  Compile::Guards::IsInteger<T>();
  return (py!=0)?((px!=0)?((px/py)+(((px%py)>0)?(1):(0))):(ceildiv_zerodivby<T>)):(ceildiv_divbyzero<T>);
}

//----------------------------------------

/**
 * @brief nearrounddiv - rounds towards nearest integer
 *        e.g.  5/7 = 1 and  3/9=0
 *        e.g. 20/9 = 2 and 13/5=3
 * @param  x
 * @param  y
 * @return normal up/down rounding of integer division (C Compiler normally rounds towards zero for positive numbers)
 */
template<typename T> constexpr T nearrounddiv(T const px, T const py){
  Compile::Guards::IsUnsigned<T>();
  return ((px+(py>>1))/py);
}

//----------------------------------------

s16 sdivceil(s16 const px, s16 const py);
u16 udivceil(u16 const px, u16 const py);

//----------------------------------------

/**
 * @brief remainder
 * @param num
 * @param divisor
 * @return
 */
template<typename T> constexpr T remainder(T const num, T const divisor){
  return (num - (divisor * (num / divisor)));
}

//----------------------------------------


} //namespace

} //namespace

#endif // MATH_DIVISION_H
