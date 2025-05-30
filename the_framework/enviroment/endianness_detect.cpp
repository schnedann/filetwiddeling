
#include <array>

//#include "build_annotations.h"
#include "endianness_detect.h"

namespace {

static constexpr std::array<uint8_t,sizeof(uint32_t)> const detection_number_lst = {0x00U,0x01U,0x02U,0x03U};

Enviroment::Endianness::endianness_t detect() noexcept{
  //constexpr std::array<uint8_t,sizeof(uint32_t)> const UNUSED_VAR(detection_number_lst) = {0x00U,0x01U,0x02U,0x03U};

  auto const addr = uintptr_t(detection_number_lst.data());

  return (*reinterpret_cast<uint32_t*>(addr) == UINT32_C(0x00010203))?(Enviroment::Endianness::endianness_t::ENDIAN_BIG):(
         (*reinterpret_cast<uint32_t*>(addr) == UINT32_C(0x03020100))?(Enviroment::Endianness::endianness_t::ENDIAN_LITTLE):(
         (*reinterpret_cast<uint32_t*>(addr) == UINT32_C(0x02030001))?(Enviroment::Endianness::endianness_t::ENDIAN_BIG_WORD):(
         (*reinterpret_cast<uint32_t*>(addr) == UINT32_C(0x01000302))?(Enviroment::Endianness::endianness_t::ENDIAN_LITTLE_WORD):(
         Enviroment::Endianness::endianness_t::ENDIAN_UNKNOWN))));
}

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=
// Get Endianess as String
//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

#if (0<dUSE_ENDIANESS_AS_STRING)

static constexpr size_t const number_of_Endiannesses = 5;

//NOLINTBEGIN(*-avoid-c-arrays)
static constexpr char const endianness_str_00[] = "Unknown Endian";
static constexpr char const endianness_str_01[] = "Big Endian";
static constexpr char const endianness_str_02[] = "Little Endian";
static constexpr char const endianness_str_03[] = "Big_Word Endian";
static constexpr char const endianness_str_04[] = "Little_Word Endian";
//NOLINTEND(*-avoid-c-arrays)

static constexpr std::array<char const*,number_of_Endiannesses> const str_lst = {
  //NOLINTBEGIN(*-array-to-pointer-decay)
  endianness_str_00
 ,endianness_str_01
 ,endianness_str_02
 ,endianness_str_03
 ,endianness_str_04
  //NOLINTEND(*-array-to-pointer-decay)

};

static constexpr std::array<size_t,number_of_Endiannesses> const str_length_lst = {
   sizeof(endianness_str_00)-1
  ,sizeof(endianness_str_01)-1
  ,sizeof(endianness_str_02)-1
  ,sizeof(endianness_str_03)-1
  ,sizeof(endianness_str_04)-1
};

#endif

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

} //namespace

bool Enviroment::Endianness::is_little(){
  return (Enviroment::Endianness::endianness_t::ENDIAN_LITTLE==Enviroment::Endianness::detected);
}

bool Enviroment::Endianness::is_big(){
  return (Enviroment::Endianness::endianness_t::ENDIAN_BIG==Enviroment::Endianness::detected);
}

Enviroment::Endianness::endianness_t const Enviroment::Endianness::detected = detect();

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=
// Get Endianess as String
//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

#if (0<dUSE_ENDIANESS_AS_STRING)

std::string Enviroment::Endianness::get_endianess_as_string(){
  auto const idx = size_t(Enviroment::Endianness::detected);
  return std::string(str_lst.at(idx),
                     str_length_lst.at(idx));
}
#endif

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=
