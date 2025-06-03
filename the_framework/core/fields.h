#ifndef FIELDS_H
#define FIELDS_H

#include <initializer_list>

//for Copyright see this file
#include "modified_bsd.h"

#include "compile_guards.h"
#include "bit_func.h"

namespace Fields{

template<typename I> static constexpr I const npos = Math::Boolean::getfullmask<I>();

//--- Forward Declarations

template<typename I> struct field_se_st;
template<typename I> struct field_sl_st;
template<typename I> struct field_fl_st;
template<typename I> using field_se = struct field_se_st<I>;
template<typename I> using field_sl = struct field_sl_st<I>;
template<typename I> using field_fl = struct field_fl_st<I>;

template<typename I> constexpr bool verify_field_se(field_se<I> const& fse);
template<typename I> constexpr bool verify_field_sl(field_sl<I> const& fsl);
template<typename I> constexpr bool verify_field_fl(field_fl<I> const& ffl);

//---

/**
 * Start - End (first out of bounds)
 */
template<typename I> struct field_se_st{
private:

  /**
   * @brief compile_time_check
   */
  void compile_time_check(){
    Compile::Guards::IsUnsigned<I>();
  }

  /**
   * @brief parse_il
   * @param il
   */
  void parse_il(std::initializer_list<I> const& il){

    {
      auto it = il.begin();
      start = it[0];
      end   = it[1];
    }

    bool OK  = (2 == il.size());
         OK &= verify_field_se<I>(*this,npos<I>);

    if(!OK){
      start = npos<I>;
      end   = npos<I>;
    }
    return;
  }

public:

  I start{npos<I>};
  I end{npos<I>};
  field_se_st()=default;
  field_se_st(std::initializer_list<I> const& il){parse_il(il);}
  field_se_st& operator=(std::initializer_list<I> const& il){
    parse_il(il);
    return *this;
  }
};

/**
 * Start - Length
 */
template<typename I> struct field_sl_st{
private:
  void chktemplate(){
    Compile::Guards::IsUnsigned<I>();
  }

  void parse_il(std::initializer_list<I> const& il){

    {
      auto it = il.begin();
      start  = it[0];
      length = it[1];
    }

    bool OK  = (2 == il.size());
         OK &= verify_field_sl<I>(*this,npos<I>);

    if(!OK){
      start  = npos<I>;
      length = npos<I>;
    }
    return;
  }

public:
  I start{npos<I>};
  I length{npos<I>};
  field_sl_st()=default;
  field_sl_st(std::initializer_list<I> const& il){parse_il(il);}
  field_sl_st& operator=(std::initializer_list<I> const& il){
    parse_il(il);
    return *this;
  }
};

/**
 * First - Last (Index)
 */
template<typename I> struct field_fl_st{
private:
  void chktemplate(){
    Compile::Guards::IsUnsigned<I>();
  }

  void parse_il(std::initializer_list<I> const& il){

    {
      auto it = il.begin();
      first = it[0];
      last   = it[1];
    }

    bool OK  = (2 == il.size());
         OK &= verify_field_fl<I>(*this,npos<I>);

    if(!OK){
      first = npos<I>;
      last   = npos<I>;
    }
    return;
  }

public:
  I first{npos<I>};
  I last{npos<I>};
  field_fl_st()=default;
  field_fl_st(std::initializer_list<I> const& il){parse_il(il);}
  field_fl_st& operator=(std::initializer_list<I> const& il){
    parse_il(il);
    return *this;
  }
};

//----- Create Fields for complete Arrays

/**
 * @brief create_array_field
 * @param elements
 * @return
 */
template<typename I> field_sl<I> create_array_field_sl(I const elements){
  return field_sl<I>({0,elements});
}

/**
 * @brief create_array_field
 * @param elements
 * @return
 */
template<typename I> field_se<I> create_array_field_se(I const elements){
  return make_field_se(field_sl<I>({0,elements}));
}

/**
 * @brief create_array_field
 * @param elements
 * @return
 */
template<typename I> field_fl<I> create_array_field_fl(I const elements){
  return make_field_fl(field_sl<I>({0,elements}));
}

//----- Generate Start - End (first out of bounds)

/**
 * @brief make_field_se
 * @param ps
 * @param pe
 * @return
 */
template<typename I> constexpr field_se<I> make_field_se(I const ps, I const pe){
  Compile::Guards::IsUnsigned<I>();
  field_se<I> res;

  bool OK  = (npos<I> > ps);
       //OK &= (npos<I> > pe);
       OK &= (ps < pe);

  if(OK){
    res = {ps,pe};
  }
  return res;
}

/**
 * @brief make_field_se
 * @param fsl - Field Start / Length
 * @return
 */
template<typename I> constexpr field_se<I> make_field_se(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_se<I>(fsl.start,fsl.start+fsl.length);
}

/**
 * @brief make_field_se
 * @param ffl - Field First / Last
 * @return
 */
template<typename I> constexpr field_se<I> make_field_se(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_se<I>(ffl.first,ffl.last+1);
}

//----- Generate Start - Length

/**
 * @brief make_field_sl
 * @param ps
 * @param pl
 * @return
 */
template<typename I> constexpr field_sl<I> make_field_sl(I const ps, I const pl){
  Compile::Guards::IsUnsigned<I>();
  field_sl<I> res;

  bool OK  = (npos<I> > ps);
       OK &= ((npos<I>-ps) >= pl);

  if(OK){
    res = {ps,pl};
  }
  return res;
}

/**
 * @brief make_field_sl
 * @param fse - Field Start / End
 * @return
 */
template<typename I> constexpr field_sl<I> make_field_sl(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return make_field_sl<I>(fse.start,fse.end-fse.start);
}

/**
 * @brief make_field_sl
 * @param ffl - Field First / Last
 * @return
 */
template<typename I> constexpr field_sl<I> make_field_sl(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_sl<I>(ffl.first,(ffl.last-ffl.first)+1);
}

//----- Generate First - Last (Index)

/**
 * @brief make_field_fl
 * @param pf
 * @param pl
 * @return
 */
template<typename I> constexpr field_fl<I> make_field_fl(I const pf, I const pl){
  Compile::Guards::IsUnsigned<I>();
  field_fl<I> res;

  bool OK  = (npos<I> > pf);
       OK &= (npos<I> > pl);
       OK &= (pl >= pf);

  if(OK){
    res = {pf,pl};
  }
  return res;
}

/**
 * @brief make_field_fl
 * @param fse - Field Start / End
 * @return
 */
template<typename I> constexpr field_fl<I> make_field_fl(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return make_field_fl<I>(fse.start,fse.end-1);
}

/**
 * @brief make_field_fl
 * @param fsl - Field Start / Length
 * @return
 */
template<typename I> constexpr field_fl<I> make_field_fl(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_fl<I>(fsl.start,fsl.start+fsl.length-1);
}

//----- get_start

/**
 * @brief get_start
 * @param fsl
 * @return
 */
template<typename I> constexpr I get_start(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return fsl.start;
}

/**
 * @brief get_start
 * @param fse
 * @return
 */
template<typename I> constexpr I get_start(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return fse.start;
}

/**
 * @brief get_start
 * @param ffl
 * @return
 */
template<typename I> constexpr I get_start(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return ffl.start;
}

//----- get_end

/**
 * @brief get_end
 * @param fsl
 * @return
 */
template<typename I> constexpr I get_end(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_se<I>(fsl).end;
}

/**
 * @brief get_end
 * @param fse
 * @return
 */
template<typename I> constexpr I get_end(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return fse.end;
}

/**
 * @brief get_end
 * @param ffl
 * @return
 */
template<typename I> constexpr I get_end(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_se<I>(ffl).end;
}

//----- get_length

/**
 * @brief get_length
 * @param fsl
 * @return
 */
template<typename I> constexpr I get_length(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return fsl.length;
}

/**
 * @brief get_length
 * @param fse
 * @return
 */
template<typename I> constexpr I get_length(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return make_field_sl<I>(fse).length;
}

/**
 * @brief get_length
 * @param ffl
 * @return
 */
template<typename I> constexpr I get_length(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_sl<I>(ffl).length;
}

//----- get_last

/**
 * @brief get_last
 * @param fsl
 * @return
 */
template<typename I> constexpr I get_last(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return make_field_fl<I>(fsl).last;
}

/**
 * @brief get_last
 * @param fse
 * @return
 */
template<typename I> constexpr I get_last(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return make_field_fl<I>(fse).last;
}

/**
 * @brief get_last
 * @param ffl
 * @return
 */
template<typename I> constexpr I get_last(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return ffl.last;
}

//----- Verify a field

/**
 * @brief verify_field_se - check consistency of field
 * @param fse
 * @return
 */
template<typename I> constexpr bool verify_field_se(field_se<I> const& fse, I const elemcnt){
  Compile::Guards::IsUnsigned<I>();
  bool OK  = (fse.start < npos<I>);
       OK &= (fse.start < fse.end);
       OK &= (fse.end   < npos<I>);
       OK &= (((0<fse.end)?(fse.end-1):(0))<elemcnt); //Do NOT use other Field Functions here to prevent recursion!
  return OK;
}

/**
 * @brief verify_field_sl - check consistency of field
 * @param fsl
 * @return
 */
template<typename I> constexpr bool verify_field_sl(field_sl<I> const& fsl, I const elemcnt){
  return verify_field_se<I>(make_field_se(fsl),elemcnt);
}

/**
 * @brief verify_field_fl - check consistency of field
 * @param ffl
 * @return
 */
template<typename I> constexpr bool verify_field_fl(field_fl<I> const& ffl, I const elemcnt){
  return verify_field_se<I>(make_field_se(ffl),elemcnt);
}

//----- advance

template<typename I> constexpr field_sl<I>& advance(field_sl<I>& fsl, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto const max_offset = (Fields::npos<I> - Fields::get_end(fsl));
  auto const loffset = (offset<max_offset)?(offset):(max_offset);
  fsl.start += loffset;
  return fsl;
}

template<typename I> constexpr field_se<I>& advance(field_se<I>& fse, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto const max_offset = (Fields::npos<I> - Fields::get_end(fse));
  auto const loffset = (offset<max_offset)?(offset):(max_offset);
  fse.start += loffset;
  fse.end   += loffset;
  return fse;
}

template<typename I> constexpr field_fl<I>& advance(field_fl<I>& ffl, I const offset){
  Compile::Guards::IsUnsigned<I>();
  auto const max_offset = (Fields::npos<I> - Fields::get_end(ffl));
  auto const loffset = (offset<max_offset)?(offset):(max_offset);
  ffl.first += loffset;
  ffl.last  += loffset;
  return ffl;
}

//----- Move a field by offset

/**
 * @brief move_field
 * @param fse - Field Start / End
 * @param offset
 * @return
 */
template<typename I> constexpr field_se<I> move_field(field_se<I> const& fse, I const offset){
  Compile::Guards::IsUnsigned<I>();
  //return make_field_se<I>(fse.start+offset,fse.end+offset);
  auto fse_local = fse;
  return Fields::advance<I>(fse_local,offset);
}

/**
 * @brief move_field
 * @param fsl - Field Start / Length
 * @param offset
 * @return
 */
template<typename I> constexpr field_sl<I> move_field(field_sl<I> const& fsl, I const offset){
  Compile::Guards::IsUnsigned<I>();
  //return make_field_sl<I>(fsl.start+offset,fsl.length);
  auto fsl_local = fsl;
  return Fields::advance<I>(fsl_local,offset);
}

/**
 * @brief move_field
 * @param ffl - Field First / Last
 * @param offset
 * @return
 */
template<typename I> constexpr field_fl<I> move_field(field_fl<I> const& ffl, I const offset){
  Compile::Guards::IsUnsigned<I>();
  //return make_field_fl<I>(ffl.first+offset,ffl.last+offset);
  auto ffl_local = ffl;
  return Fields::advance<I>(ffl_local,offset);
}

//----- Calc overflow save Middle of field

/**
 * @brief middle_field
 * @param ffl - Field First / Last
 * @return
 */
template<typename I> constexpr I middle_field(field_fl<I> const& ffl){
  Compile::Guards::IsUnsigned<I>();
  return ffl.first + ((ffl.last-ffl.first)>>1);
}

/**
 * @brief middle_field
 * @param fse - Field Start / End
 * @return
 */
template<typename I> constexpr I middle_field(field_se<I> const& fse){
  Compile::Guards::IsUnsigned<I>();
  return middle_field<I>(make_field_fl<I>(fse));
}

/**
 * @brief middle_field
 * @param fsl - Field Start / Length
 * @return
 */
template<typename I> constexpr I middle_field(field_sl<I> const& fsl){
  Compile::Guards::IsUnsigned<I>();
  return middle_field<I>(make_field_fl<I>(fsl));
}

//-----

template<typename I> constexpr bool is_in_bounds(field_fl<I> const& ffl,
                                                 field_fl<I> const& bounds){
  Compile::Guards::IsUnsigned<I>();
  bool in_bounds = true;
  in_bounds &= (ffl.first >= bounds.first);
  in_bounds &= (ffl.last  <= bounds.last);
  return in_bounds;
}

template<typename I> constexpr bool is_in_bounds(field_se<I> const& fse,
                                                 field_se<I> const& bounds){
  Compile::Guards::IsUnsigned<I>();
  return is_in_bounds<I>(make_field_fl<I>(fse),make_field_fl<I>(bounds));
}

template<typename I> constexpr bool is_in_bounds(field_sl<I> const& fsl,
                                                 field_sl<I> const& bounds){
  Compile::Guards::IsUnsigned<I>();
  return is_in_bounds<I>(make_field_fl<I>(fsl),make_field_fl<I>(bounds));
}

//----- scale

template<typename I> constexpr field_sl<I>& scale(field_sl<I>& fsl, I const factor){
  Compile::Guards::IsUnsigned<I>();
  fsl.start  *= factor;
  fsl.length *= factor;
  return fsl;
}

template<typename I> constexpr field_se<I>& scale(field_se<I>& fse, I const factor){
  Compile::Guards::IsUnsigned<I>();
  fse.start *= factor;
  fse.end   *= factor;
  return fse;
}

template<typename I> constexpr field_fl<I>& scale(field_fl<I>& ffl, I const factor){
  Compile::Guards::IsUnsigned<I>();
  ffl.first *= factor;
  ffl.last  *= factor;
  return ffl;
}

//----- to_byte_field

template<typename T, typename I> constexpr field_sl<I>& to_byte_field(field_sl<I>& fsl){
  Compile::Guards::IsUnsigned<I>();
  fsl.start  *= sizeof(T);
  fsl.length *= sizeof(T);
  return fsl;
}

template<typename T, typename I> constexpr field_se<I>& to_byte_field(field_se<I>& fse){
  Compile::Guards::IsUnsigned<I>();
  fse.start *= sizeof(T);
  fse.end   *= sizeof(T);
  return fse;
}

template<typename T, typename I> constexpr field_fl<I>& to_byte_field(field_fl<I>& ffl){
  Compile::Guards::IsUnsigned<I>();
  ffl.first *= sizeof(T);
  ffl.last  *= sizeof(T);
  return ffl;
}

//-----

} //namespace

#endif
