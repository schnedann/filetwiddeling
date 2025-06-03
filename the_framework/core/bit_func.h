/******************************************************************************/
/** ----- Bit Functions -----     Integer Bitmanipulation functions          **/
/** if not stated otherwise:                                                 **/
/** Assume Unsigned Integer Numbers and Two'sCompliment Arithmethic          **/
/** developed , collected and / or implemented by                            **/
/** Danny Schneider, 2017-2022                                               **/
/**                                                                          **/
/** Sources & Ideas for Algorithms used here:                                **/
/** http://aggregate.org/MAGIC/                                              **/
/**                                                                          **/
/**                                                                          **/
/**                                                                          **/
/******************************************************************************/

#ifndef BIT_FUNC_H
#define BIT_FUNC_H

//for Copyright see this file
#include "modified_bsd.h"

//--------------------------------------------------

#include <type_traits>
#include "dtypes.h"
#include "global_config.h"
#include "compile_guards.h"
#include "build_annotations.h"
#include "bitmacros.h"

//--------------------------------------------------

namespace Math::Boolean{

//--------------------------------------------------

/**
 * @brief is_even - Test Integer if it is Even (LSB==0)
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR bool is_even(T const pv) noexcept{
  Compile::Guards::IsInteger<T>();
  return (0==(pv&T(1)));
}

/**
 * @brief is_odd - Test Integer if it is Odd (LSB==1)
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR bool is_odd(T const pv) noexcept{
  Compile::Guards::IsInteger<T>();
  return (0!=(pv&T(1)));
}

//--------------------------------------------------

/**
 * @brief get_lowestbit - return lowest bit set in integer (only the lowest bit set)
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T get_lowestbit(T const pv) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ((pv^(pv-1))&pv);
}

/**
 * @brief get_low_one - Get Lowest Bit Set in an Integer (result is One Hot)
 * @note  Fastest posibility to encode / calculate Priorities!!!
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T get_low_one(T const pv) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (pv)&((~(pv))+1);
}

//-----

/**
 * @brief is_onehot - Is Integer a Power of 2 (is Integer One Hot encoded?)
 * @param pv
 * @return true if non negative integer is power of two
 */
template<typename T> NODISCARD CONSTEXPR bool is_onehot(T const pv) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (pv>0)?((pv&(pv-1))==0):(false);
}

//--------------------------------------------------

/**
 * @brief set_all_lower_bits - starting with the highest bit set, set all lower bits in an Integer
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T set_all_lower_bits(T const pv){
  Compile::Guards::IsUnsigned<T>();
  T res = pv;
#if (0 < dUSE_OPTIMIZED)
  constexpr u8 const bitsoftype = sizeof(T)<<3;
  constexpr u8 const stages = Meta::Math::FLOOR_LOG2<bitsoftype>::value;
#else
  constexpr u8 const bitsoftype = sizeof(T)<<3;
  T shift = 1;
  while(shift<bitsoftype){
    res |= res >> shift;
    shift <<= 1;
  }
#endif
  return res;
}

/**
 * @brief next_power_of_2 - For any pv get the next larger, one hot coded number (the next larger 2^x)
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T next_power_of_2(T const pv){
  Compile::Guards::IsUnsigned<T>();
  T res = pv-1;
  res = set_all_lower_bits<T>(res);
  return 1+res;
}

/**
 * @brief right_propagate_rightmost_1bit - set all bits right to the rightmost bit set
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T right_propagate_rightmost_1bit(T const pv){
  Compile::Guards::IsUnsigned<T>();
  return (pv | (pv-1));
}

/**
 * @brief isolate_rightmost_0bit
 * @param pv
 * @return only the rightmost zero bit of pv ist set (result expected to be one-hot)
 */
template<typename T> NODISCARD CONSTEXPR T isolate_rightmost_0bit(T const pv){
  Compile::Guards::IsUnsigned<T>();
  return (~pv & (pv+1));
}

/**
 * @brief set_rightmost_0bit
 * @param pv
 * @return pv with the rightmost zero bit now set
 */
template<typename T> NODISCARD CONSTEXPR T set_rightmost_0bit(T const pv){
  Compile::Guards::IsUnsigned<T>();
  return (pv | (pv+1));
}

/**
 * @brief get_rightmost_1bit
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T get_rightmost_1bit(T const pv){
  Compile::Guards::IsUnsigned<T>();
  T const _y = (pv-1);
  return ((pv|_y)^_y);
}

/**
 * @brief clr_rightmost_1bit
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T clr_rightmost_1bit(T const pv){
  Compile::Guards::IsUnsigned<T>();
  return (pv & (pv-1));
}

//--------------------------------------------------

/**
 * @brief getbitsoftype - number of bits of given Type T
 * @return
 */
template<typename T> NODISCARD CONSTEXPR size_t getbitsoftype() noexcept{
  return sizeof(T)<<3;
}

template<typename T> NODISCARD CONSTEXPR size_t getnibblesoftype() noexcept{
  return sizeof(T)<<1;
}

/**
 * @brief getmaxbits - satturate "bits" at value of number of bits of given Type T
 * @param bits
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T getmaxbits(u8 const bits) noexcept{
  return (getbitsoftype<T>()<bits)?(getbitsoftype<T>()):(bits);
}

static CONSTEXPR auto const TBits_X08 = getbitsoftype<u8>();
static CONSTEXPR auto const TBits_X16 = getbitsoftype<u16>();
static CONSTEXPR auto const TBits_X32 = getbitsoftype<u32>();
static CONSTEXPR auto const TBits_X64 = getbitsoftype<u64>();

//--------------------------------------------------

/**
 * Get Mask for single Bit 0..N-1
 *  0 - 1
 *  1 - 2
 *  2 - 4
 *  3 - 8
 */
template<typename T> NODISCARD CONSTEXPR T getmaskbit(u8 const bit) noexcept{
  Compile::Guards::IsUnsigned<T>();
  CONSTEXPR auto const TBits = getbitsoftype<T>();
  return T(1)<<((bit<TBits)?(bit):(bit%TBits));
}

static CONSTEXPR auto const Bit_value00 = getmaskbit<size_t>(  0);
static CONSTEXPR auto const Bit_value01 = getmaskbit<size_t>(  1);
static CONSTEXPR auto const Bit_value02 = getmaskbit<size_t>(  2);
static CONSTEXPR auto const Bit_value03 = getmaskbit<size_t>(  3);
static CONSTEXPR auto const Bit_value04 = getmaskbit<size_t>(  4);
static CONSTEXPR auto const Bit_value05 = getmaskbit<size_t>(  5);
static CONSTEXPR auto const Bit_value06 = getmaskbit<size_t>(  6);
static CONSTEXPR auto const Bit_value07 = getmaskbit<size_t>(  7);
static CONSTEXPR auto const Bit_value08 = getmaskbit<size_t>(  8);
static CONSTEXPR auto const Bit_value15 = getmaskbit<size_t>( 15);
static CONSTEXPR auto const Bit_value16 = getmaskbit<size_t>( 16);
static CONSTEXPR auto const Bit_value23 = getmaskbit<size_t>( 23);
static CONSTEXPR auto const Bit_value24 = getmaskbit<size_t>( 24);
static CONSTEXPR auto const Bit_value31 = getmaskbit<size_t>( 31);
static CONSTEXPR auto const Bit_value32 = getmaskbit<size_t>( 32);
static CONSTEXPR auto const Bit_value47 = getmaskbit<size_t>( 47);
static CONSTEXPR auto const Bit_value48 = getmaskbit<size_t>( 48);
static CONSTEXPR auto const Bit_value63 = getmaskbit<size_t>( 63);

/**
 * if we have 4 bits, then the mask for the bit 4 is: 10000,
 * which is the decimal value of 16. And 4 bits allow to represent
 * a "space" of 16 different values
 *
 * !!!WARNING!!! getbitspace<uAB>(AB) will not fit in type uAB
 * 0 -    1
 * 1 -   10
 * 2 -  100
 * 3 - 1000
 */
template<typename T> NODISCARD CONSTEXPR T getbitspace(u8 const bits) noexcept{
  return getmaskbit<T>(bits);
}

/**
 * One Hot coded Mask --> highest Bit 0...N-1 Set only
 * Parameter: Bits of Mask
 * 0 -    0
 * 1 -    1
 * 2 -   10
 * 3 -  100
 */
template<typename T> NODISCARD CONSTEXPR T mask_msb(u8 bits=Math::Boolean::getbitsoftype<T>()){
  return (bits>0)?(getbitspace<T>(bits-1)):(0);
}

/**
 * Overflow Save set all Bits for space of x Bits 0..N
 * 0 -   0
 * 1 -   1
 * 2 -  11
 * 3 - 111
 */
template<typename T> NODISCARD CONSTEXPR T getfullmask(u8 const bits = u8(Math::Boolean::getbitsoftype<T>())) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (bits<2)?(bits):((((getmaskbit<T>(bits-1)-1)<<1)|1));
//return (_x>1)?(((BITSPACE<T>(_x-1)-1)<<1)|1):(_x);
}

/**
 * Overflow Save set all Bits for space of x Bits 1..N
 * 0 -   0
 * 1 -   1
 * 2 -  11
 * 3 - 111
 */
template<typename T> NODISCARD CONSTEXPR T getfullmask_alt1(T const pv) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return right_propagate_rightmost_1bit<T>(getmaskbit<T>(pv-1));
}

//-----

/**
 * @brief maskbits - Apply Mask
 * @param pv
 * @param pm
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T maskbits(T const pv, T const pm){
  Compile::Guards::IsUnsigned<T>();
  return ((pv)&(pm));
}

//--------------------------------------------------

/**
 * @brief AND - Typechecking Wrapper for Bitwise And
 * @param px
 * @param py
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T AND(T const px, T const py) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ((px)&(py));
}

/**
 * @brief OR - Typechecking Wrapper for Bitwise Or
 * @param px
 * @param py
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T OR(T const px, T const py) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ((px)|(py));
}

/**
 * @brief XOR - Typechecking Wrapper for Bitwise Xor
 * @param px
 * @param py
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T XOR(T const px, T const py) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ((px)^(py));
}

/**
 * @brief NOT
 * @param px
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T NOT(T const px) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ~px;
}

//--------------------------------------------------
// Manipulate / Test Bit(s) in variables / HW-Register
//--------------------------------------------------

 /**
 * @brief SETBITS - in reg set set bits from mask
 * @param reg
 * @param mask
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T setbits(T const pv, T const mask) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (OR<T>((pv),(mask)));
}

/**
 * @brief CLRBITS - in reg clear set bits from mask
 * @param reg
 * @param mask
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T clrbits(T const pv, T const mask) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (AND<T>((pv),~(mask)));
}

/**
 * @brief TOGGLEBITS - bits set in reg and mask will be cleared, bits cleared in reg and mask will be set
 * @param reg
 * @param mask
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T togglebits(T const pv, T const mask) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (XOR<T>((pv),(mask)));
}

/**
 * @brief equal - test if reg and mask are binary equal
 * @param reg
 * @param mask
 * @return
 */
template<typename T> NODISCARD CONSTEXPR bool equal(T const reg, T const mask) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (((reg)^(mask))==0);
}

/**
 * @brief TestBits - test if all bits in mask are set in reg
 * @param reg
 * @param mask
 * @return
 */
template<typename T> NODISCARD CONSTEXPR bool TestBits(T const reg, T const mask) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return equal<T>(AND<T>((reg),(mask)),mask);
}

/**
 * @brief TestSingleBit - test if bit is set in reg
 * @param reg
 * @param bit - 0..N-1
 * @return
 */
template<typename T> NODISCARD CONSTEXPR bool TestSingleBit(T const reg, u8 const bit) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return (0<AND<T>((reg),(getmaskbit<T>(bit))));
}

//--------------------------------------------------

/**
 * @brief LogicShiftLeft
 * @param pv
 * @param shiftby
 * @param in_shift
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T LogicShiftLeft(T const pv, u8 const shiftby, T const in_shift=0) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return mSHL(pv,shiftby,in_shift,T);
}

/**
 * @brief LogicShiftRight
 * @param pv
 * @param shiftby
 * @param in_shift
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T LogicShiftRight(T const pv, u8 const shiftby, T const in_shift=0) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return mSHR(pv,shiftby,in_shift,T);
}

// ---

/**
 * @brief is_msb_set - In Integer test if MSB is '1'
 * @param pv
 * @return true if MSB Bit is one
 */
template<typename T> NODISCARD CONSTEXPR bool is_msb_set(T const pv){
  Compile::Guards::IsInteger<T>();
  using UT = typename std::make_unsigned<T>::type;
  auto const upv = static_cast<UT>(pv);
  bool sign_exists = (0<AND<UT>(upv,mask_msb<UT>()));
  return sign_exists;
}

/**
 * @brief is_negative - test if an integer is a negative number
 * @param pv
 * @return true if MSB Bit is one AND the Type is signed
 */
template<typename T> NODISCARD CONSTEXPR bool is_negative(T const pv){
  Compile::Guards::IsInteger<T>();
  return std::is_signed<T>() && is_msb_set<T>(pv);
}

// ---

/**
 * @brief msbpos - get position 0...(N-1) of highest one-bit
 * @param num
 * @return
 * @note a set sign will be included
 */
template<typename T> u8 msbpos(T const num){
  Compile::Guards::IsInteger<T>();
  CONSTEXPR auto const Tbits = Math::Boolean::getbitsoftype<T>();
  using UT = typename std::make_unsigned<T>::type;
  auto const unum = static_cast<UT>(num);
  u8 res = 0;
  for(unsigned int ii=0; ii<Tbits; ++ii){
    auto const mask = Math::Boolean::getmaskbit<UT>(ii);
    if(0<(Math::Boolean::AND<UT>(unum,mask))){
      res = u8(ii);
    }
  }
  return res;
}

/**
 * @brief bitcount - get number of bits ocupied by number num
 * @param num
 * @return 1 if the number value is 0 or 1 (to transfer that information we need a single bit!), of >1 for any other value.
 * @note a set sign will return all bits of the type occupied!
 */
template<typename T> u8 bitcount(T const num){
  Compile::Guards::IsInteger<T>();
  auto const tmp = msbpos<T>(num);
  return (0<tmp)?(1+tmp):(1);
}

// ---

/**
 * @brief ArithShiftL - equal to multiply by 2
 *                      ARITHSHL equals LogicShiftLeft with shift_in is zero
 * @param pv
 * @param shiftby
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T ArithShiftL(T const pv, u8 const shiftby) noexcept{
  Compile::Guards::IsInteger<T>();
  using UT = typename std::make_unsigned<T>::type;
  return static_cast<T>(LogicShiftLeft<UT>(static_cast<UT>(pv),shiftby,UT(0)));
}

/**
 * @brief ArithShiftR -  equal to divide by 2
 *                       Type is signed   - 10101010 arithshr by 4 = 11111010 (always ones extended, as MSB is one)
 *                       Type is unsigned - 10101010 arithshr by 4 = 00001010 (always zero extended)
 *                       Type generic     - 01010101 arithshr by 4 = 00000101 (always zero extended, as MSB is zero)
 * @param pv
 * @param shiftby
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T ArithShiftR(T const pv, u8 const shiftby) noexcept{
  Compile::Guards::IsInteger<T>();
  using UT = typename std::make_unsigned<T>::type;
  CONSTEXPR u8 const TBits = Math::Boolean::getbitsoftype<T>();
  constexpr UT const all_zero = 0;
  constexpr UT const all_ones = mBitMask(TBits,UT);
  auto const upv = static_cast<UT>(pv);
  auto const in_shift = (is_negative<T>(pv))?(all_ones):(all_zero);
  return static_cast<T>(LogicShiftRight<UT>(upv,shiftby,in_shift));
}

// ---

/**
 * @brief RotateLeft
 * @param pv
 * @param shiftby
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T RotateLeft(T const pv, u8 const shiftby) noexcept{
  Compile::Guards::IsInteger<T>();
  using UT = typename std::make_unsigned<T>::type;
  return static_cast<T>(mROTL(static_cast<UT>(pv),shiftby,UT));
}

/**
 * @brief RotateRight
 * @param pv
 * @param shiftby
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T RotateRight(T const pv, u8 const shiftby) noexcept{
  Compile::Guards::IsInteger<T>();
  using UT = typename std::make_unsigned_t<T>::type;
  return static_cast<T>(mROTR(static_cast<UT>(pv),shiftby,UT));
}

//--------------------------------------------------

/**
 * @brief Rangelimit
 * @param pv
 * @param limit
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T Rangelimit(T const pv, T const limit) noexcept{
  Compile::Guards::IsUnsigned<T>();
  return ((pv)%(limit));
}

//--------------------------------------------------

/**
 * @brief ZQ - Zweierkomplement - Two's Complement
 * @param pv
 * @return
 */
template<typename T> NODISCARD CONSTEXPR T ZQ(T const pv) noexcept{
  Compile::Guards::IsInteger<T>();
  return ((~(pv))+1);
}

/**
 * @brief IntAbs - if number is negative, get the same number with positiv sign
 * @param pv
 */
template<typename T> NODISCARD auto IntAbs(T const pv) -> decltype(std::make_unsigned<T>::type){
  Compile::Guards::IsUnsigned<T>();
  return static_cast<decltype(std::make_unsigned<T>::type)>((pv<0)?(ZQ<T>(pv)):(pv));
}

//--------------------------------------------------
// Alternate Versions - Alternate Versions - Alternate Versions
//--------------------------------------------------

/**
 * Get Lowest Bit Set in an Integer (One Hot)
 * Fast posibility to encode / calculate Priorities!!!
 */
/*template<typename T> NODISCARD T lowest_one_set(T const pv) noexcept{
  return ((pv)&((~(pv))+1));
}*/

//--------------------------------------------------

/**
 * Is Integer a Power of 2 (is Integer One Hot encoded?)
 */
/*template<typename T> NODISCARD bool is_power_of_2(T const pv) noexcept{
  return ((((pv)&((pv)-1))==0)?(true):(false));
}*/

//--------------------------------------------------

/**
 * @brief to_Gray_Code
 * @param pv
 * @return
 */
template<typename T> NODISCARD T to_Gray_Code(T const pv){
  return (pv ^ (pv >> 1));
}

//--------------------------------------------------

} //namespace

#endif //BIT_FUNC_H
