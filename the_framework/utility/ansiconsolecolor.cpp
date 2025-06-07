#include "ansiconsolecolor.h"

#include <array>

#include "Stringhelper.h"

std::string Utility::AnsiColor::colorize(std::string_view const sv, colorsel_T const csel, modify_e const msel){
  std::string_view const color = ((csel == colorsel_e::red)?(fgred):(
                                  (csel == colorsel_e::green)?(fggreen):(
                                  (csel == colorsel_e::yellow)?(fgyellow):(
                                  (csel == colorsel_e::blue)?(fgblue):(
                                  (csel == colorsel_e::magenta)?(fgmagenta):(
                                  (csel == colorsel_e::cyan)?(fgcyan):(
                                  (csel == colorsel_e::high_grey)?(fghighgrey):(
                                  (csel == colorsel_e::grey)?(fggrey):(
                                  (csel == colorsel_e::high_red)?(fghighred):(
                                  (csel == colorsel_e::high_green)?(fghighgreen):(
                                  (csel == colorsel_e::high_yellow)?(fghighyellow):(
                                  (csel == colorsel_e::high_blue)?(fghighblue):(
                                  fgwhite)))))))))))));
  std::array<std::string_view,3> arr = {
    color,
    sv,
    reset_all
  };
  return Utility::Strings::stradd(arr);
}
