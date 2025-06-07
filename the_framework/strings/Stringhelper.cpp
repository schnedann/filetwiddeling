//for Copyright see this file
#include "modified_bsd.h"

#include <string>

#include "standard_detect.h"

#if dIS_LEAST_CXX17
  #include <string_view>
#endif

#include <algorithm>

#include "Stringhelper.h"
#include "bit_func.h"
#include "math_division.h"

using namespace std;

//---

/**
 * @brief Utility::Strings::Smply - Generate Sting of multiple copies of String str count times
 * @param str
 * @param count
 * @return
 */
std::string Utility::Strings::Smply(string_nc const& str, size_t const count){
  stringstream ss;
  for(size_t ii=0;ii<count;++ii){
    ss << str;
  }
  return ss.str();
}

/**
 * @brief Utility::Strings::depth2indent - Assemble indention
 * @param depth
 * @return
 */
std::string Utility::Strings::depth2indent(size_t const depth, bool usetabs){
  stringstream ss;
  char const ins_ch = (usetabs)?('\t'):(' ');
  for(size_t ii=0;ii<depth;++ii){
    ss << ins_ch;
  }
  return ss.str();
}

/**
 * @brief Utility::Strings::comma2WS - replace Commas with Whitespace
 * @param str
 * @return
 */
string& Utility::Strings::Comma2WS(std::string& str){
  string::size_type pos = str.find(',');
  while(pos!=string::npos){
    str.replace(pos, 1, string(" "));
    pos = str.find(',');
  }
  return str;
}

string Utility::Strings::Comma2WS_cpy(string_nc const& str){
  std::string res(str);
  return Utility::Strings::Comma2WS(res);
}

//--------------------------------------------------

/**
 * @brief Utility::Strings::replace - In String str1 replace pattern str2 with str3
 * @param str1
 * @param str2
 * @param str3
 * @return
 */
std::string& Utility::Strings::replace(std::string& str, string_nc const& pattern, string_nc const& replace_str){
  for(string::iterator it = str.begin(); it != str.end();){
    string::size_type pos = str.find(pattern);
    if(pos==string::npos){
      it=str.end();
    }else{
      str.replace(pos,pattern.size(),replace_str);
    }
  }
  return str;
}


std::string Utility::Strings::replace_cpy(string_nc const& str, string_nc const& pattern, string_nc const& replace_str){
  std::string res(str);
  return Utility::Strings::replace(res,pattern,replace_str);
}

//--------------------------------------------------

/**
 * @brief Utility::Strings::extfill - Fill String str with char ch until str.size() == len
 * @param str
 * @param ch
 * @param len
 * @return
 */
std::string Utility::Strings::fill(string_nc const& str, char const ch, size_t const len){
  stringstream ss;
  ss << str;
  if(len>str.size()){
    string fill;
    auto const cnt = size_t(len-str.size());
    fill.append(cnt,ch);
    ss << fill;
  }
  return ss.str();
}

/**
 * @brief Utility::Strings::extfill - Fill String str with fstr until str.size() >= len
 * @param str
 * @param fstr
 * @param len
 * @return
 */
string Utility::Strings::fill(string_nc const& str, string_nc const& fstr, size_t const len){
  stringstream ss;
  ss << str;
  if(len>str.size()){
    string fill;
    auto const cnt = Math::Division::ceildiv<size_t>((len-str.size()),fstr.size());
    for(size_t ii=0; ii<cnt; ++ii){
      fill.append(fstr);
    }
    ss << fill;
  }
  return ss.str();
}

//--------------------------------------------------

/**
 *
 */
std::string& Utility::Strings::toupper(std::string& str){
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

/**
 *
 */
std::string& Utility::Strings::tolower(std::string& str){
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

std::string Utility::Strings::toupper_cpy(string_nc const& str){
  auto strcpy = std::string(str);
  static_cast<void>(toupper(strcpy));
  return strcpy;
}

std::string Utility::Strings::tolower_cpy(string_nc const& str){
  auto strcpy = std::string(str);
  static_cast<void>(tolower(strcpy));
  return strcpy;
}

//--------------------------------------------------

/**
 * @brief left_trim
 * @param str
 * @return
 */
std::string& Utility::Strings::left_trim(std::string& str){
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch)->bool {
    return !std::isspace(ch);
  }));
  return str;
}

/**
 * @brief right_trim
 * @param str
 * @return
 */
std::string& Utility::Strings::right_trim(std::string& str){
  str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)->bool {
      return !std::isspace(ch);
  }).base(), str.end());
  return str;
}

/**
 * @brief trim
 * @param str
 * @return
 */
std::string& Utility::Strings::trim(std::string& str){
  return right_trim(left_trim(str));
}

//--------------------------------------------------

std::string Utility::Strings::stradd(std::span<std::string_view> const arr){
  std::stringstream ss;
  for(auto const& str:arr){
    ss << str;
  }
  return ss.str();
}
