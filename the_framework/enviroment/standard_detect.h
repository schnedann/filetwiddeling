/******************************************************************************/
/** C++ Version                                                              **/
/** Danny Schneider, 2023                                                    **/
/******************************************************************************/

#ifndef STANDARD_DETECT_H
#define STANDARD_DETECT_H

//for Copyright see this file
#include "modified_bsd.h"

#ifndef __STDC_VERSION__
  #define __STDC_VERSION__ 199000L
#endif

// POC
#define dIS_LEAST_C94	(__STDC_VERSION__==199409L)
#define dIS_LEAST_C99	(__STDC_VERSION__==199901L)
#define dIS_LEAST_C11	(__STDC_VERSION__==201112L)
#define dIS_LEAST_C18	(__STDC_VERSION__==201710L)

// C++
#define dIS_LEAST_CXX98 (__cplusplus >= 199711L)
#define dIS_LEAST_CXX11 (__cplusplus >= 201103L)
#define dIS_LEAST_CXX14 (__cplusplus >= 201402L)
#define dIS_LEAST_CXX17 (__cplusplus >= 201703L)
#define dIS_LEAST_CXX20 (__cplusplus >= 202002L)
#define dIS_LEAST_CXX23 (__cplusplus >= 202100L)

namespace Enviroment {

namespace Standard {

static constexpr bool const IS_LEAST_C94    = (0<dIS_LEAST_C94);
static constexpr bool const IS_LEAST_C99    = (0<dIS_LEAST_C99);
static constexpr bool const IS_LEAST_C11    = (0<dIS_LEAST_C11);
static constexpr bool const IS_LEAST_C18    = (0<dIS_LEAST_C18);

static constexpr bool const IS_LEAST_CXX98  = (0<dIS_LEAST_CXX98);
static constexpr bool const IS_LEAST_CXX11  = (0<dIS_LEAST_CXX11);
static constexpr bool const IS_LEAST_CXX14  = (0<dIS_LEAST_CXX14);
static constexpr bool const IS_LEAST_CXX17  = (0<dIS_LEAST_CXX17);
static constexpr bool const IS_LEAST_CXX20  = (0<dIS_LEAST_CXX20);
static constexpr bool const IS_LEAST_CXX23  = (0<dIS_LEAST_CXX23);

} //namespace

} //namespace

#endif // STANDARD_DETECT_H
