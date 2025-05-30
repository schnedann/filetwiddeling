#ifndef ARCHITECTURE_DETECT_H
#define ARCHITECTURE_DETECT_H

//for Copyright see this file
#include "modified_4Cbsd.h"

#if defined(__x86_64__) || defined(_M_X64)
#define ARCH_IS_x86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define ARCH_IS_x86_32
#elif defined(__ARM_ARCH_2__)
#define ARCH_IS_ARM2
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define ARCH_IS_ARM3
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define ARCH_IS_ARM4T
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define ARCH_IS_ARM5
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define ARCH_IS_ARM6T2
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define ARCH_IS_ARM6
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_IS_ARM7
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_IS_ARM7A
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define ARCH_IS_ARM7R
#elif defined(__ARM_ARCH_7M__)
#define ARCH_IS_ARM7M
#elif defined(__ARM_ARCH_7S__)
#define ARCH_IS_ARM7S
#elif defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_IS_ARM64
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define ARCH_IS_MIPS
#elif defined(__sh__)
#define ARCH_IS_SUPERH
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define ARCH_IS_POWERPC
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define ARCH_IS_POWERPC64
#elif defined(__sparc__) || defined(__sparc)
#define ARCH_IS_SPARC
#elif defined(__m68k__)
#define ARCH_IS_M68K
#else
#define ARCH_IS_UNKNOWN
#endif

#endif // ARCHITECTURE_DETECT_H
