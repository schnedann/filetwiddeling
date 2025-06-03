
#ifndef META_H
#define META_H

//for Copyright see this file
#include "modified_bsd.h"

#include <iostream>
#include <memory>
#include <utility>

#include "dtypes.h"
#include "compile_guards.h"

namespace Meta{

namespace Varadic{

template<typename ...Args> auto sum(Args ...args){
  return (args + ...);
}

//---

template<typename ...Args> void printf(Args&&... args){
  (std::cout << ... << std::forward<Args>(args)) << '\n';
  return;
}

//---

// compile-time endianness swap based on http://stackoverflow.com/a/36937049
template<class T, std::size_t... N>
constexpr T bswap_impl(T i, std::index_sequence<N...>) {
  return (((i >> N*sizeof(std::uint8_t) & std::uint8_t(-1)) << (sizeof(T)-1-N)*sizeof(std::uint8_t)) | ...);
}
template<class T, class U = std::make_unsigned_t<T>>
constexpr U bswap(T i) {
  return bswap_impl<U>(i, std::make_index_sequence<sizeof(T)>{});
}

//---

  /**
   * sum sizes of types in varadic template
   */
  template<std::size_t N, typename Ti, typename ...T> struct varadic_size;

  template<std::size_t N, typename Ti, typename ...T> struct varadic_size{
    static std::size_t const value = sizeof(Ti)+varadic_size<N-1,T...>::value;
  };

  template<typename Ti, typename ...T> struct varadic_size<0,Ti,T...>{
    static std::size_t const value = sizeof(Ti);
  };

//---

  /**
   * Print for all std:: containers...
   * https://eli.thegreenplace.net/2014/variadic-templates-in-c/
   */

  template <template <typename, typename...> class ContainerType,
            typename ValueType, typename... Args>
  void print_container(const ContainerType<ValueType, Args...>& c) {
    for (const auto& v : c) {
      std::cout << v << ' ';
    }
    std::cout << '\n';
  }

} //namespace

//---

namespace Constants{

template<typename T> struct UINTMAX{
  static const T value = ((((1ull<<((sizeof(T)<<3)-1))-1)<<1)|1);
};

} //namespace

//---

namespace Math {

//--- Power x^y

template<u64 M, u64 N> struct POWER{
  static constexpr u64 value = M * POWER<M,(N-1)>::value;
};

template<u64 M> struct POWER<M,0>{
  static constexpr u64 value = 1;
};

//--- Devision with Rounding upwards (towards Infinite)

template<typename T, T A, T B> struct CEILDIV{
  constexpr static T value = ((A+B-1)/B);
};

//--- Logarithm to Base 2

template<u64 VAL> struct FLOOR_LOG2_Impl{
  constexpr static u8 value = 1 + FLOOR_LOG2_Impl<VAL/2>::value;
};
template<> struct FLOOR_LOG2_Impl<0>{
  constexpr static u8 value = 0;
};

template<u64 VAL> struct FLOOR_LOG2{
  constexpr static u8 value = (VAL>1)?(FLOOR_LOG2_Impl<VAL>::value-1):(0);
};

template<u64 VAL> struct CEIL_LOG2_Impl{
  constexpr static u8 value = 1 + CEIL_LOG2_Impl<((VAL+1)/2)>::value;
};
template<> struct CEIL_LOG2_Impl<1>{
  constexpr static u8 value = 0;
};
template<> struct CEIL_LOG2_Impl<0>{
  constexpr static u8 value = 0;
};

template<u64 VAL> struct CEIL_LOG2{
  constexpr static u8 value = (VAL>1)?(CEIL_LOG2_Impl<VAL>::value):(0);
};

//---

template<u64 VAL> struct FLOOR_LOG10_Impl{
  constexpr static u8 value = 1 + FLOOR_LOG10_Impl<VAL/10>::value;
};
template<> struct FLOOR_LOG10_Impl<0>{
  constexpr static u8 value = 0;
};

template<u64 VAL> struct FLOOR_LOG10{
  constexpr static u8 value = (VAL>1)?(FLOOR_LOG10_Impl<VAL>::value-1):(0);
};

template<u64 VAL> struct CEIL_LOG10_Impl{
  constexpr static u8 value = 1 + CEIL_LOG10_Impl<((VAL+1)/10)>::value;
};
template<> struct CEIL_LOG10_Impl<1>{
  constexpr static u8 value = 0;
};
template<> struct CEIL_LOG10_Impl<0>{
  constexpr static u8 value = 0;
};

template<u64 VAL> struct CEIL_LOG10{
  constexpr static u8 value = (VAL>1)?(CEIL_LOG10_Impl<VAL>::value):(0);
};

} //namespace

namespace Types {

/**************************************************
 * Remove const from any Type
 **************************************************/

template<typename T> struct DeConst {
  using R = T;
};
template<typename T> struct DeConst<T const> {
  using R = T;
};

template<typename T> struct DeConst<T const*> {
  using R = T*;
};

/**************************************************
 * Remove Pointer Property from Type if exists
 **************************************************/

//default Implementation
template<typename U> struct remove_pointer { // in general case
  using type = U;
};

//Spezialisation
template<typename T> struct remove_pointer<T*> { // for U = T*
  using type = T;
};

template<typename T> struct remove_pointer<std::shared_ptr<T>> { // for U = std::shared_ptr<T>
  using type = T;
};

template<typename T> struct remove_pointer<std::unique_ptr<T>> { // for U = std::unique_ptr<T>
  using type = T;
};

//convenience Wrapper
template <typename W> using RemovePointer = typename remove_pointer<W>::type;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//--- Select Type by condition

template<bool cond, typename TRUE, typename FALSE> struct if_else{
  using type = FALSE;
};

template<typename TRUE, typename FALSE> struct if_else<true,TRUE,FALSE>{
  using type = TRUE;
};

template<bool cond, typename TRUE, typename FALSE> using if_else_T = typename if_else<cond,TRUE,FALSE>::type;

//--- construct nested Initializer List

template<typename T, size_t LEVELS> struct nested_init_lists_st{
  using type = std::initializer_list<typename nested_init_lists_st<T,(LEVELS-1)>::type>;
  //using type = if_else_T<(LEVELS>1),std::initializer_list<typename nested_init_lists_st<T,(LEVELS-1)>::type>,std::initializer_list<T>>;
};

template<typename T> struct nested_init_lists_st<T,1>{
  using type = std::initializer_list<T>;
};

template<typename T> struct nested_init_lists_st<T,0>{
  using type = std::initializer_list<T>;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template<typename T> struct replace8bitint{

  using type = if_else_T<std::is_same<u8,T>::value,u16,
               if_else_T<std::is_same<s8,T>::value,s16,
               if_else_T<std::is_same<u8&,T>::value,u16&,
               if_else_T<std::is_same<s8&,T>::value,s16&,
               if_else_T<std::is_same<u8 const,T>::value,u16 const,
               if_else_T<std::is_same<s8 const,T>::value,s16 const,
               if_else_T<std::is_same<u8 const&,T>::value,u16 const&,
               if_else_T<std::is_same<s8 const&,T>::value,s16 const&,
               T>>>>>>>>;
};

template<typename T> using replace8bitint_t = typename Meta::Types::replace8bitint<T>::type;
//template<typename T> using replace8bitint_t = typename Meta::Types::replace8bitint<typename std::remove_cv<T>::type>::type;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

template<typename T> struct multype{
  constexpr void typecheck(){
    Compile::Guards::IsInteger<T>();
  }

  using type = if_else_T<std::is_same< u8,T>::value,u16,
               if_else_T<std::is_same< s8,T>::value,s16,
               if_else_T<std::is_same<u16,T>::value,u32,
               if_else_T<std::is_same<s16,T>::value,s32,
               if_else_T<std::is_same<u32,T>::value,u64,
               if_else_T<std::is_same<s32,T>::value,s64,
               void>>>>>>;
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

} //namespace Types

//---



//---

/*namespace datastruct{

template<typename T1, typename T2> struct pair{
  T1 first;
  T2 second;
};*/

};

#endif
