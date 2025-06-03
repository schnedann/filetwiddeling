#ifndef Stringhelper_H
#define Stringhelper_H

#include <string>

#include "standard_detect.h"

#if dUSE_STDCPP_17
  #include <string_view>
  using string_nc = std::string_view;
#else
  using string_nc = std::string;
#endif
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <bitset>

//for Copyright see this file
#include "modified_bsd.h"

#include "dtypes.h"
#include "global_config.h"

#include "compile_guards.h"
#include "build_annotations.h"
#include "bit_func.h"
#include "iterator_ptr_wrapper.h"
#include "meta.h"

namespace Utility{

namespace Strings{

#if (0 == dUSE_STD_NO_RTTI)
/**
 * @brief get_type_as_str
 * @return
 */
template<class T> std::string get_type_as_str(){
  auto const name = typeid(T).name();
  std::stringstream ss;
  char* res = nullptr;
#if defined(__GNUC__)
  int status = -1;
  res = abi::__cxa_demangle(name, NULL, NULL, &status);
  char const * const demangled_name = (status==0)?(res):(name);
  ss << std::string(demangled_name);
#else
  ss << name;
#endif
  if(nullptr != res){
    free(res);
  }
  ss << "_" << Math::Boolean::getbitsoftype<T>() << "bit";

  return ss.str();
}
#endif

/**************************************************
 * Convert from / to String
 **************************************************/

/**
 * @brief X_2hexstr - Convert unsigned Number _x to HexString
 * @param px
 * @return
 */
template<class T> std::string X_2hexstr(T const px){
  Compile::Guards::IsUnsigned<T>();
  return static_cast<std::ostringstream*>( &(std::ostringstream() << std::hex << "0x" << static_cast<Meta::Types::replace8bitint_t<T>>(px)) )->str();
}

//-----

/**
 * @brief X_2str - Convert Number _x to String in Field with Width w and fillchar f
 * @param px
 * @param width
 * @param fill
 * @return
 */
template<class T, typename = std::enable_if_t<!std::is_same<bool,T>::value>> std::string X_2str(T const px, int const width=0, char const fill=' '){
  //Compile::Guards::isArithmetic<T>();
  return static_cast<std::ostringstream*>( &(std::ostringstream() << std::setw(width) << std::setfill(fill) << static_cast<Meta::Types::replace8bitint_t<T>>(px)) )->str();
}



//-----

/**
 * @brief str2_X - Convert String to type T
 */
template<class T> T str2_X(string_nc& str){
  T res;
  std::istringstream(str)>>res;
  return res;
}

//--------------------------------------------------

enum class conmode:bool{
  asDEC = false,
  asHEX = true
};
using conmode_t = enum conmode;

/**
 * @brief unum2str - Convert Number to String - decimal or hexadecimal
 * @param num
 * @param dechex
 * @return
 */
template<class T> std::string unum2str(T const num, conmode_t const dechex){
  Compile::Guards::IsUnsigned<T>();
  std::stringstream ss;
  if(dechex==conmode::asDEC){
    ss << static_cast<Meta::Types::replace8bitint_t<T>>(num);
  }else{
    ss << "0x" << std::hex << static_cast<Meta::Types::replace8bitint_t<T>>(num);
  }
  return ss.str();
}

//-----

/**
 * Convert String to Number - decimal or hexadecimal
 */
template<class T> T str2unum(string_nc const& str, conmode_t const dechex){
  Compile::Guards::IsUnsigned<T>();
  T num;
  std::stringstream ss;

  if(dechex==conmode::asHEX){
    size_t pos = str.find("0x",0);
    std::string croped = str.substr(pos,(str.length()-pos));
     ss << croped;
     ss >> std::hex >> num;
  }else{
     ss << str;
     ss >> num;
  }
  return num;
}

/**
 * Convert String to Number - decimal or hexadecimal
 */
template<class T> T str2num(std::string const& str, conmode_t const dechex){
  T num;
  std::stringstream ss;

  if(dechex==conmode::asHEX){
    size_t pos = str.find("0x",0);
    std::string croped = str.substr(pos,(str.length()-pos));
     ss << croped;
     ss >> std::hex >> num;
  }else{
     ss << str;
     ss >> num;
  }
  return num;
}

//--------------------------------------------------

/**
 * @brief unum2bin
 * @param x
 * @return
 */
template<typename T> std::string unum2bin(T const x){
  Compile::Guards::IsUnsigned<T>();
  std::stringstream ss;
  ss << std::bitset<Math::Boolean::getbitsoftype<T>()>{x};
  return ss.str();
}

/**
 * @brief num2bin - Convert unsigned Integer 2 binary as String
 * @param x
 * @param bits
 * @return
 */
template<typename T> std::string num2bin(T const x, u8 const bits=Math::Boolean::getbitsoftype<T>()){
  Compile::Guards::IsInteger<T>();

  using TT = typename std::remove_cv<T>::type;
  using UT = std::make_unsigned_t<TT>;
  UT ux;

  if(std::is_signed_v<T>){
     auto xptr = Iterators::itcast<T,UT>(&x);
     ux = Iterators::get<UT>(xptr);
  }else{
     ux = x;
  }

  size_t const strsize = bits;
  std::string res(strsize,'\0');
  UT mask = Math::Boolean::mask_msb<UT>(bits);
  for(uf8 ii=0; ii<bits; ++ii){
    res[ii] = ((ux&mask)>0)?('1'):('0');
    mask>>=1;
  }
  return res;
}

/**
 * @brief num2bin
 * @param name
 * @param x
 * @param bits
 * @return
 */
template<typename T> std::string num2bin(string_nc const& name, T const x, u8 const bits=Math::Boolean::getbitsoftype<T>()){
  std::stringstream ss;
  if(!name.empty()){
    ss << name << ": ";
  }
  ss << num2bin<T>(x,bits);
  return ss.str();
}

/**
 * @brief PRNBINVAR - Macro to Convert any variable to an (binary)String, including output of the variable name
 */
#define PRNBIN(_X)    (Utility::Strings::num2bin<decltype(_X)>((_X)))
#define PRNBINVAR(_X) (Utility::Strings::num2bin<decltype(_X)>((#_X),(_X)))

//--------------------------------------------------

/*std::string to_string(string_nc const& name, bool const px){
  std::stringstream ss;
  if(!name.empty()){
    ss << name << ": ";
  }
  ss << ((px)?("True"):("False"));
  return ss.str();
}*/

/**
 * @brief to_string
 * @note signed and unsigned 8Bit Integers are handled as 16Bit values to prevent char output
 * @param name
 * @param px
 * @return
 */
template<typename T> std::string to_string(string_nc const& name, T const px){
  using baseT = typename std::remove_reference<typename std::remove_cv<T>::type>::type;

  static constexpr bool const is_bool = std::is_same<bool,baseT>::value;
  static constexpr bool const is_int  = std::is_integral<baseT>::value;

#if (0 < DBGCONSTEXPR)
  static int  const width = (is_int)?(Meta::Math::CEIL_LOG10<mgetfullmask(mgetbitsoftype(u32),u32)>::value):(0);
#else
  static CONSTEXPR int  const width   = (is_int)?(Meta::Math::CEIL_LOG10<Math::Boolean::getfullmask<u32>()>::value):(0);
#endif

  std::stringstream ss;
  if(!name.empty()){
    ss << name << ": ";
  }

  /*if(is_bool){
    ss << ((static_cast<bool>(px))?("True"):("False"));
  }else*/
  if(is_int){
    ss << std::setw(width) << static_cast<Meta::Types::replace8bitint_t<baseT>>(px);
  }else{
    ss << px;
  }
  return ss.str();
}

template<typename T> std::string to_string(T const px){
  std::string dummy;
  return to_string<T>(dummy,px);
}

/**
 * @brief PRNVAR - Macro to Convert any variable to an String, including output of the variable name
 */
#define VAR(_X)    (Utility::Strings::to_string<decltype(_X)>((_X)))
#define PRNVAR(_X) (Utility::Strings::to_string<decltype(_X)>((#_X),(_X)))
// plus type-info
#define PRNVAR_EX(_X) (Utility::Strings::to_string<decltype(_X)>(Utility::Strings::stradd<3>({Utility::Strings::get_type_as_str<decltype(_X)>()," ",(#_X)}),(_X)))



//-----

/**
 * @brief to_hstring - Convert any Type to String as long as an known conversion exists
 * @note signed and unsigned 8Bit Integers are handled as 16Bit values to prevent char output,
 *       also any integer is converted in Hexadecimal notation
 * @param name
 * @param px
 * @return
 */
template<typename T> std::string to_hstring(string_nc const& name, T const px){
  std::stringstream ss;
  if(!name.empty()){
    ss << name << ": ";
  }
  bool is_int = std::is_integral<T>::value;
  //bool is_8bit_integer = is_int && (sizeof(T)==1);
  if(is_int){
    /*if(is_8bit_integer){
      bool is_u = std::is_unsigned<T>::value;
      if(is_u){
        ss << "0x" << std::hex << u16(x);
      }else{
        ss << "0x" << std::hex << s16(x);
      }
    }else{
      ss << "0x" << std::hex << x;
    }*/

    if(0>px){
      ss << "-";
    }
    ss << X_2hexstr<T>(Math::Boolean::IntAbs<T>(px));
  }else{
    ss << px;
  }
  return ss.str();
}

/**
 * @brief PRNHEXVAR - Macro to Convert any variable to an (hex)String, including output of the variable name
 */
#define HEXVAR(_X)    (Utility::Strings::to_hstring<decltype(_X)>((_X)))
#define PRNHEXVAR(_X) (Utility::Strings::to_hstring<decltype(_X)>((#_X),(_X)))

/**************************************************
 * Replace in String
 **************************************************/

/**
 * @brief comma2WS - change Commas to WhiteSpace
 * @return
 */
std::string& Comma2WS(        std::string& str);
std::string  Comma2WS_cpy(string_nc const& str);

/**
 * @brief replace - Replace all ocourences of String pattern in String str with String replace
 * @param str
 * @param pattern
 * @param replace
 * @return
 */
std::string& replace(       std::string& str, string_nc const& pattern, string_nc const& replace_str);
std::string replace_cpy(string_nc const& str, string_nc const& pattern, string_nc const& replace_str);

/**************************************************
 * ToUpper / ToLower
 **************************************************/

/**
 * @brief toupper - Convert to Upper-Case String
 * @param str
 * @return
 */
std::string& toupper(        std::string& str);
std::string  toupper_cpy(string_nc const& str);

/**
 * @brief tolower - Convert to Lower-Case String
 * @param str
 * @return
 */
std::string& tolower(        std::string& str);
std::string  tolower_cpy(string_nc const& str);

/**************************************************
 * Trimm Strings (Remove Whitespace from both Ends)
 **************************************************/

/**
 * @brief left_trim
 * @param str
 * @return
 */
std::string& left_trim(std::string& str);

/**
 * @brief right_trim
 * @param str
 * @return
 */
std::string& right_trim(std::string& str);

/**
 * @brief trim
 * @param str
 * @return
 */
std::string& trim(std::string& str);

/**************************************************
 *
 **************************************************/

/**
 * @brief fill - fill string str with char ch until size of str is len
 * @param str - string to fill up
 * @param ch  - char to paste
 * @param len - target length of str -> for fill to work len needs to be greater than str.size()
 * @return
 */
std::string fill(string_nc const& str,      char const  ch,   size_t const len);
std::string fill(string_nc const& str, string_nc const& fstr, size_t const len);

/**************************************************
 *
 **************************************************/

/**
  * @brief stradd
  * @param arr
  * @return
  */
template<size_t N> std::string stradd(std::array<std::string,N> const& arr){
  std::stringstream ss;

  for(auto const& str:arr){
    ss << str;
  }
  return ss.str();
}

//--------------------------------------------------

/**
 * @brief Smply - Generate Sting of multiple copies of itself
 * @return
 */
std::string Smply(string_nc const& str, size_t const count);

/**
 * @brief depth2indent - indent String with WhiteSpace x Times
 * @return
 */
std::string depth2indent(size_t const count, bool usetabs=false);

//--------------------------------------------------

/**
 *  Print all Elements of a Array in one single Line divided by a '|'
 */
template<typename T> std::string print_array_Line(T data[], size_t size, size_t per_row){
  std::stringstream ss;

  size_t ij = 1;
  ss << " | ";
  for(size_t ii = 0; ii < size; ++ii){
    ss << data[ii];
    if(0==(ij%per_row)){
      ss << " |\n";
      ij = 0;
    }else{
       ss << " | ";
    }
    ++ij;
  }
  ss << "\n";

  return ss.str();
}

//--------------------------------------------------

} //namespace

} //namespace

namespace stdext = Utility::Strings;

#endif // Stringhelper_H
