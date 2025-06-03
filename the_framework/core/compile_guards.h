/******************************************************************************/
/** Compile Time Assertions                                                  **/
/** collected and / or implemented by                                        **/
/** Danny Schneider, 2017-2021                                               **/
/******************************************************************************/

#ifndef COMPILE_GUARDS_H
#define COMPILE_GUARDS_H

//for Copyright see this file
#include "modified_bsd.h"

#include <type_traits>
#include <concepts>

#include "dtypes.h"

namespace Compile{

namespace Guards{

template<class T> concept cIntegral         = std::is_integral<T>::value;
template<class T> concept cSignedIntegral   = cIntegral<T> && std::is_signed<T>::value;
template<class T> concept cUnsignedIntegral = cIntegral<T> && !cSignedIntegral<T>;
template<class T> concept cMathType         = cIntegral<T> || std::is_floating_point_v<T>;

//--- extend type traits

namespace Impl {
  template<typename T> struct is_integer_st{
      static constexpr bool const value = (std::is_integral_v<T> && !std::is_same_v<bool,T>);
  };
  template<typename T> using is_integer_T = struct is_integer_st<T>;
  template<typename T> constexpr bool is_integer_v = is_integer_T<T>::value;
}

template<typename T> constexpr bool IsMathType(){
  return Impl::is_integer_v<T> || std::is_floating_point_v<T>;
}

//--- Asserting Wrappers

/**
 * @brief IsSigned - Assert
 */
template<typename T> constexpr void IsSigned(){
  static_assert (Impl::is_integer_v<T>    , "Error, Integer expected");
  static_assert (std::is_signed<T>(), "Error, Signed Integer expected");
  return;
}

/**
 * @brief IsUnsigned - Assert
 */
template<typename T> constexpr void IsUnsigned(){
  static_assert (Impl::is_integer_v<T>    , "Error, Integer expected");
  static_assert (std::is_unsigned_v<T>, "Error, Unsigned Integer expected");
  return;
}

/**
 * @brief IsInteger - Assert
 */
template<typename T> constexpr void IsInteger(){
  static_assert (Impl::is_integer_v<T>, "Error, Integer expected");
  return;
}

/**
 * @brief isFloatingPoint - Assert Floating Point Type
 */
template<typename T> constexpr void isFloatingPoint(){
  static_assert (std::is_floating_point_v<T>, "Error, Type not a Floating Point Type");
  return;
}

/**
 * @brief isFundamental - Assert arithmetic type (provided by the base Language), void, or nullptr_t
 */
template<typename T> constexpr void isFundamental(){
  static_assert (std::is_fundamental_v<T>, "Error, arithmetic type, void, or nullptr_t expected");
  return;
}

/**
 * @brief isArithmetic - Assert integral type or a floating-point type
 */
template<typename T> constexpr void isArithmetic(){
  static_assert (std::is_arithmetic_v<T>, "Error, integral type or a floating-point type or a cv-qualified version thereof expected");
  return;
}

} //namespace

} //namespace

#endif
