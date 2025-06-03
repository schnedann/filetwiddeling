#include "math_division.h"

/* Prototype Code...

s64 div_ceil(s64 const numerator, s64 const denominator){
  std::lldiv_t res = std::lldiv(numerator, denominator);
  return (res.rem)?(res.quot+1):(res.quot);
}

s64 div_to_zero(s64 const n, s64 const d){
 return n / d;
} //as per C++11 standard note 80

s64 div_up(s64 const n, s64 const d) {
  return (n / d) + (((n < 0) ^ (d > 0)) && (n % d));
} //i.e. +1 iff (not exact int && positive result)

s64 div_down(s64 const n, s64 const d) {
  return n / d - (((n > 0) ^ (d > 0)) && (n % d));
} //i.e. +1 iff (not exact int && negative result)

s64 div_to_nearest(s64 const n, s64 const d) {
  return (2*n - d + 2*(true&&((n<0)^(d>0)))*d) / (2*d);
} //i.e. +-0.5 as per pre-rounding result sign, then div_to-zero
//it however rounds numbers like +/- 3.5 towards 0 and not even.

*/

/**
 * Signed Division with rounding Mode Ceil()
 * eg. Gnu Compiler rounds to Zero normaly
 */
s16 Math::Division::sdivceil(s16 const px, s16 const py){
  s16 div = px / py;
  if (((px ^ py) >= 0) && (px % py != 0)){
      div++;
  }
  return div;
}

/**
 * Unsigned Division with rounding Mode Ceil()
 * eg. Gnu Compiler rounds to Zero normaly
 */
u16 Math::Division::udivceil(u16 const px, u16 const py){
  u16 div = 0;
  if(py>0){
    if(px>0){
      div = px / py;
      if (px % py != 0){
        div++;
      }
    }else{
      div = 1;
    }
  }
  return div;
}
