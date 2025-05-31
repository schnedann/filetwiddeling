/***************************************************
 * Bit-Manipulation Macros
 * Danny Schneider, 2014
 * http://graphics.stanford.edu/~seander/bithacks.html
 * http://gurmeet.net/puzzles/fast-bit-counting-routines/
 * http://aggregate.org/MAGIC/
 ***************************************************/

#ifndef BITMACROS_H
#define BITMACROS_H

//for Copyright see this file
#include "modified_bsd.h"

#if __cplusplus
  #include <type_traits>
#endif

//----- Macros
//      m... -> normal Macro
//      ma.. -> auto Assignment to first Operand

// --- Portable Bitmask for X Bits Generation ---
// => e.g. mBitMask(32) wont use shift 32,
// so its safe on a 32Bit Computer
#if __cplusplus
  #define mBitMask(pb,tT) ((((typename std::make_unsigned<tT>::type(1)<<((pb)-1))-1)<<1)|typename std::make_unsigned<tT>::type(1))
#else
  #define mBitMask(pb,tT) (((((tT)1U<<((pb)-1))-1)<<1)|(tT)1U)
#endif

// --- Logic ---

#define mgetbitsoftype(pT) (sizeof(pT)<<3)
#define mgetmaskbit(pbit,pT) (pT(1)<<(pbit))
#define mgetfullmask(pbits,pT) ((((mgetmaskbit(((pbits)-1),pT))-1)<<1)|1)

#define mINVERT(px)     (~(px))
#define maINVERT(px)    ((px)~=(px))

#define mXOR(px,py)     ((px)^((py)))
#define maXOR(px,py)    ((px)^=((py)))

#define mOR(px,py)      ((px)|(py))
#define maOR(px,py)     ((px)|=(py))

#define mAND(px,py)     ((px)&(py))
#define maAND(px,py)    ((px)&=(py))

#define mNAND(px,py)     (~mAND((px),(py)))
#define maNAND(px,py)    ((px)=mNAND((px),(py)))

// --- Bit Manipulation ---

#define maSetBit(pv,pb) ((pv)|=(1U<<(pb)))
#define mGetBit(pv,pb)  ((pv)&(1U<<(pb)))
#define maClrBit(pv,pb) ((pv)&=~(1U<<(pb)))

// ---

#define maskBits(px,py)                   (mAND((px),(py)))
#define maskBitsExcept(px,py,pbe)         (mAND(mAND((px),(py)),mINVERT(pbe)))
#define maskTestBits(px,py,pb)            (mAND((px),(py))==(pb))
#define maskTestBitsExcept(px,py,pb,pbe)  (maskBitsExcept(px,py,pbe)==(pb))

// ---

#define maskBitsSet(px,py)                (maOR((px),(py)))
#define maskBitsSetExcept(px,py,pbe)      (maOR((px),mAND((py),mINVERT(pbe))))
#define maskBitsClear(px,py)              (maAND((px),mINVERT(py)))
#define maskBitsClearExcept(px,py,pbe)    (maAND((px),mINVERT(mAND((py),mINVERT(pbe)))))
#define maskBitsToggle(px,py)             (maXOR((px),(py)))
#define maskBitsToggleExcept(px,py,pbe)   (maXOR((px),mAND((py),mINVERT(pbe))))

//---

#define mtypebits(tT)            (sizeof(tT)<<3)

// --- Shift Operations ---

/*
 * pv - value to be shifted
 * pb - shift by number of bits
 *      0                       -> no shift, pv is returned
 *      1...((sizeof(tT)<<3)-1) -> shifted value (if si=0)
 *      >=(sizeof(tT)<<3)       -> 0, as all bits are shifted out of pv
 * si - shift in value (replacing the original bits at position)
 * tT - type of pv (unsigned Integer expected!)
 *
 * @note the normal shift behaviour of C / C++ is like pv<<(pb&(sizeof(tT)<<3)),
 *       so pb of 32 in case of an u32 is a zero shift,
 *       pb of 33 in case of an u32 is a shift by one, ...
 */

#if __cplusplus
  #define m2xxX(pb,tT)           (typename std::make_unsigned<tT>::type(1)<<(pb))
  #define mask_highbits(pb,tT)   (typename std::make_unsigned<tT>::type(0)-m2xxX(pb,tT))
#else
  #define m2xxX(pb,tT)           ((tT)1U<<(pb))
  #define mask_highbits(pb,tT)   ((tT)0U-m2xxX(pb,tT))
#endif

#define mSHR_IMPL(pv,pb,si,tT) (mOR(mAND(((pv)>>(pb)),mBitMask((mtypebits(tT)-(pb)),tT)),mAND((si),mask_highbits((mtypebits(tT)-(pb)),tT))))
#define mSHL_IMPL(pv,pb,si,tT) (mOR(mAND(((pv)<<(pb)),mask_highbits((pb),tT)),mAND((si),mBitMask((pb),tT))))

#define mSHR(pv,pb,si,tT)      (((pb)==0)?(pv):(((pb)>=mtypebits(tT))?(0):(mSHR_IMPL((pv),(pb),(si),tT))))
#define maSHR(pv,pb,si,tT)     ((pv)=mSHR((pv),(pb),(si),tT))

#define mSHL(pv,pb,si,tT)      (((pb)==0)?(pv):(((pb)>=mtypebits(tT))?(0):(mSHL_IMPL((pv),(pb),(si),tT))))
#define maSHL(pv,pb,si,tT)     ((pv)=(mSHL((pv),(pb),(si),tT)))

// --- Rotate

#define mROTL(pv,pb,tT) ((pb)==0)?(pv):(((pb)>=mtypebits(tT))?(pv):(mOR(((pv)<<(pb)),((pv)>>(mtypebits(tT)-(pb))))))
#define mROTR(pv,pb,tT) ((pb)==0)?(pv):(((pb)>=mtypebits(tT))?(pv):(mOR(((pv)>>(pb)),((pv)<<(mtypebits(tT)-(pb))))))

// --- Get / Change a Bit (to State i) ---

#define maSetBinary(pv,pb,pi) ((pv)=(((pv)&(~(1U<<(pb))))|((pi)<<(pb))))
#define mGetBinary(pv,pb)     (((pv)>>(pb))&1U)

// --- Word <--> Byte Access ---
#define mHByte(pv) (((pv)>>8)&0x00FFu)
#define mLByte(pv) ((pv)&0x00FFu)

#define mSwapByte(pv)    (mOR(mAND(mSHR((pv),8),mBitMask(8)),mSHL(mAND((_w),mBitMask(8)),8,0)))
#define mToWord(pb1,pb2) (mOR(mAND((pb1),mBitMask(8)),mSHL(mAND((pb2),mBitMask(8)),8,0)))

//-----

#endif //BITMACROS_H
