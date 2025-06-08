#include "ansiconsolecolor.h"

#include <array>

#include "Stringhelper.h"

std::string Utility::AnsiColor::colorize(std::span<std::string_view> const arr, colorsel_T const csel, modify_e const msel){
  auto const str = Utility::Strings::stradd(arr);

  return Utility::AnsiColor::colorize(str,csel,msel);
}


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
                                  (csel == colorsel_e::white)?(fgwhite):(
                                  ""))))))))))))));

  std::string_view const modifier = (msel == modify_e::bold)?(bold_on):(
                                    (msel == modify_e::inverse)?(inverse_on):(
                                    (msel == modify_e::italic)?(italic_on):(
                                    (msel == modify_e::slow_blink)?(slowblink_on):(
                                    (msel == modify_e::fast_blink)?(fastblink_on):(
                                    (msel == modify_e::underline)?(underline_on):(
                                    ""))))));

  std::array<std::string_view,4> arr = {
    color,
    modifier,
    sv,
    reset_all
  };
  return Utility::Strings::stradd(arr);
}
