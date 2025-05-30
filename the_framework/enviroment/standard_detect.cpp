#include "needful_macros.h"
#include "standard_detect.h"

#pragma message (MSG_INFORM "CPP Std: " NF_STR_CONV(__cplusplus))

#if (0<dIS_LEAST_CXX23)

#pragma message(MSG_INFORM "C++23 selected")

#elif (0<dIS_LEAST_CXX20)

#pragma message(MSG_INFORM "C++20 selected")

#elif (0<dIS_LEAST_CXX17)

#pragma message(MSG_INFORM "C++17 selected")

#elif (0<dIS_LEAST_CXX14)

#pragma message(MSG_INFORM "C++14 selected")

#elif (0<dIS_LEAST_CXX11)

#pragma message(MSG_INFORM "C++11 selected")

#elif (0<dIS_LEAST_CXX98)

#pragma message(MSG_INFORM "C++98 selected")

#endif
