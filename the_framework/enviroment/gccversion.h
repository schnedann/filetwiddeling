/******************************************************************************/
/** Gnu Compiler Version                                                     **/
/** see predefined macros @ Sourceforge                                      **/
/** https://sourceforge.net/p/predef/wiki/VersionNormalization/              **/
/** Danny Schneider, 2017-2020                                               **/
/******************************************************************************/

#ifndef GCCVERSION_H
#define GCCVERSION_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include "dtypes.h"

/*
 * https://sourceforge.net/p/predef/wiki/Compilers/
 */
#if defined(__GNUC__)
  #if defined(__GNUC_PATCHLEVEL__)
    #define cGNUC_VERSION (__GNUC__ * 10000 \
                         + __GNUC_MINOR__ * 100 \
                         + __GNUC_PATCHLEVEL__)
  #else
    #define cGNUC_VERSION (__GNUC__ * 10000 \
                         + __GNUC_MINOR__ * 100)
  #endif
#else
    #define cGNUC_VERSION 0
#endif

namespace Enviroment{

namespace Compiler {

static constexpr bool const is_GNUCompiler = (0<cGNUC_VERSION);

static constexpr u32 const gccversion = cGNUC_VERSION;

} // namespace Compiler

} //namespace

#endif
