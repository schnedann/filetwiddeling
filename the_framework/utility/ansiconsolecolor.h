#ifndef ANSICONSOLECOLOR_H
#define ANSICONSOLECOLOR_H

#include <string>

namespace Utility {

namespace AnsiColor {

static auto const reset_all = std::string_view("\033[0m");
//clears all colors and styles (to white on black)

static auto const bold_on = std::string_view("\033[1m");
//bold on

static auto const italic_on = std::string_view("\033[3m");
//italics on

static auto const underline_on = std::string_view("\033[4m");
//underline on

static auto const inverse_on = std::string_view("\033[7m");
//inverse on - reverses foreground & background colors

static auto const strikethrough_on = std::string_view("\033[9m");
//strikethrough on

static auto const bold_off = std::string_view("\033[22m");
//bold off

static auto const italic_off = std::string_view("\033[23m");
//italics off

static auto const underline_off = std::string_view("\033[24m");
//underline off

static auto const inverse_off = std::string_view("\033[27m");
//inverse off

static auto const strikethrough_off = std::string_view("\033[29m");
//strikethrough offf

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

} //namespace

} //namespace

#endif // ANSICONSOLECOLOR_H
