#ifndef ANSICONSOLECOLOR_H
#define ANSICONSOLECOLOR_H

#include <string>
#include <string_view>
#include <span>

#include "dtypes.h"

namespace Utility::AnsiColor {

//clears all colors and styles (to white on black)
static auto const reset_all = std::string_view("\033[0m");

//---

//bold on
static auto const bold_on = std::string_view("\033[1m");
//bold off
static auto const bold_off = std::string_view("\033[22m");


//italics on
static auto const italic_on = std::string_view("\033[3m");
//italics off
static auto const italic_off = std::string_view("\033[23m");


//underline on
static auto const underline_on = std::string_view("\033[4m");
//underline off
static auto const underline_off = std::string_view("\033[24m");


//slow Blink
static auto const slowblink_on = std::string_view("\033[5m");
//slow Blink off
static auto const slowblink_off = std::string_view("\033[25m");


//fast Blink
static auto const fastblink_on = std::string_view("\033[6m");
//fast Blink off
static auto const fastblink_off = std::string_view("\033[25m");


//inverse on - reverses foreground & background colors
static auto const inverse_on = std::string_view("\033[7m");
//inverse off
static auto const inverse_off = std::string_view("\033[27m");


//strikethrough on
static auto const strikethrough_on = std::string_view("\033[9m");
//strikethrough off
static auto const strikethrough_off = std::string_view("\033[29m");

//---

static auto const fgblack = std::string_view("\033[30m");
//set foreground color to black

static auto const fgred = std::string_view("\033[31m");
//set foreground color to red

static auto const fggreen = std::string_view("\033[32m");
//set foreground color to green

static auto const fgyellow = std::string_view("\033[33m");
//set foreground color to yellow

static auto const fgblue = std::string_view("\033[34m");
//set foreground color to blue

static auto const fgmagenta = std::string_view("\033[35m");
//set foreground color to magenta (purple)

static auto const fgcyan = std::string_view("\033[36m");
//set foreground color to cyan

static auto const fghighgrey = std::string_view("\033[37m");
//set foreground color to white

static auto const fgdefault = std::string_view("\033[39m");
//set foreground color to default (white)

static auto const fggrey       = std::string_view("\033[90m");
static auto const fghighred    = std::string_view("\033[91m");
static auto const fghighgreen  = std::string_view("\033[92m");
static auto const fghighyellow = std::string_view("\033[93m");
static auto const fghighblue   = std::string_view("\033[94m");
static auto const fgpink       = std::string_view("\033[95m");
static auto const fghighcyan   = std::string_view("\033[96m");
static auto const fgwhite      = std::string_view("\033[97m");

static constexpr std::string_view const blau           = "\033[38;5;63m";
static constexpr std::string_view const orange         = "\033[38;5;202m";
static constexpr std::string_view const redish_pink    = "\033[38;5;167m";
static constexpr std::string_view const mint_green     = "\033[38;5;148m";
static constexpr std::string_view const dark_grey      = "\033[38;5;236m";
static constexpr std::string_view const brown          = "\033[38;5;130m";
static constexpr std::string_view const light_yellow   = "\033[38;5;190m";
static constexpr std::string_view const light_brownish = "\033[38;5;180m";
static constexpr std::string_view const light_beige    = "\033[38;5;255m";
static constexpr std::string_view const light_grey     = "\033[38;5;250m";
static constexpr std::string_view const bright_cyan    = "\033[38;5;46m";
static constexpr std::string_view const bright_red     = "\033[38;5;196m";
static constexpr std::string_view const bright_orange  = "\033[38;5;208m";
static constexpr std::string_view const bright_yellow1 = "\033[38;5;111m";
static constexpr std::string_view const bright_yellow2 = "\033[38;5;220m";

//--------------------------------------------------
// Background
//--------------------------------------------------

static auto const bgblack = std::string_view("\033[40m");
//set background color to black

static auto const bgred = std::string_view("\033[41m");
//set background color to red

static auto const bggreen = std::string_view("\033[42m");
//set background color to green

static auto const bgyellow = std::string_view("\033[43m");
//set background color to yellow

static auto const bgblue = std::string_view("\033[44m");
//set background color to blue

static auto const bgmagenta = std::string_view("\033[45m");
//set background color to magenta (purple)

static auto const bgcyan = std::string_view("\033[46m");
//set background color to cyan

static auto const bgwhite = std::string_view("\033[47m");
//set background color to white

static auto const bgdefault = std::string_view("\033[49m");
//set background color to default (white)

//--------------------------------------------------

enum class colorsel_e:u8{
  none = 0,
  reset,
  black,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  high_grey,
  grey,
  high_red,
  high_green,
  high_yellow,
  high_blue,
  pink,
  high_cyan,
  white
};
using colorsel_T = enum colorsel_e;

enum class modify_e:u8{
  none = 0,
  bold,
  italic,
  underline,
  slow_blink,
  fast_blink,
  inverse
};
using modify_e_T = enum modify_e;

std::string colorize(std::span<std::string_view> const arr, colorsel_T const csel, modify_e const msel=modify_e::none);
std::string colorize(std::string_view const sv, colorsel_T const csel, modify_e const msel=modify_e::none);

} //namespace

#endif // ANSICONSOLECOLOR_H
