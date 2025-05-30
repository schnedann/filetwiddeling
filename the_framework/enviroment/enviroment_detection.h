/******************************************************************************/
/** Convinience Header for enviromental detection                            **/
/** Danny Schneider, 2023                                                    **/
/******************************************************************************/

#ifndef ENVIROMENT_DETECTION_H
#define ENVIROMENT_DETECTION_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#include "endianness_detect.h"   //Machine Endianness
#include "standard_detect.h"     //C++ Standards
#include "architecture_detect.h" //Target Architecture Detection
#include "compiler_detect.h"     //GCC / Clang detection
#include "os_detect.h"           //OS Enviroment detection
#include "posix_detect.h"

#endif // ENVIROMENT_DETECTION_H
