/******************************************************************************/
/**                                                                          **/
/** Danny Schneider, 2017-2023                                               **/
/******************************************************************************/

#ifndef ITERATOR_UNALIGNED_DATA_H
#define ITERATOR_UNALIGNED_DATA_H

//for Copyright see this file
#include "modified_bsd.h"

#include "dtypes.h"
#include "compile_guards.h"
#include "fields.h"
#include "bit_func.h"
#include "registers.h"
#include "endianness_detect.h"
#include "endian_convert.h"
#include "iterator_ptr_wrapper.h"
#include "iterator_base_api.h"
#include "meta.h"

namespace Iterators {

/**
 * @brief The unaligned_iterator_proxy class
 */
class unaligned_iterator_proxy:virtual public Iterator_Base_API::Iterator_Base,
                                       public Iterator_Base_API::PrePost_Interface<unaligned_iterator_proxy>,
                                       public Iterator_Base_API::Subscript_Interface<unaligned_iterator_proxy>{
  private:

  //...

  public:

  unaligned_iterator_proxy()=default;
  ~unaligned_iterator_proxy()=default;

  unaligned_iterator_proxy(unaligned_iterator_proxy const& other)=default;
  unaligned_iterator_proxy(unaligned_iterator_proxy&& other) noexcept =default;

  unaligned_iterator_proxy& operator=(unaligned_iterator_proxy const& other)=default;
  unaligned_iterator_proxy& operator=(unaligned_iterator_proxy&& other) noexcept =default;

  //-----

};

/**
 * @brief The unaligned_iterator class - automatic handling of unaligned data
 * @note AT   - Aligned Type e.g. you want to access an array of u8 as if  it where u32, where the start of u8 values míght not be aligned with the u32 values
 * @note  T   - possible unaligned Type (u8 in the example)
 * @note ITER - if ITER is const, the unaligned_iterator will read but not write
 *              if ITER is Non const, the unaligned_iterator will write
 */
template<typename AT, typename I, typename ITER> class unaligned_iterator:
    virtual public Iterator_Base_API::Iterator_Base,
    public Iterator_Base_API::Common_Interface<unaligned_iterator<AT,I,ITER>>,
    public Iterator_Base_API::PrePost_Interface<unaligned_iterator<AT,I,ITER>>,
    public Iterator_Base_API::Subscript_Interface<unaligned_iterator<AT,I,ITER>>{
public:
  using T = typename Meta::Types::RemovePointer<ITER>;

private:

  void compile_time_checks(){
    Compile::Guards::IsInteger<AT>();
    Compile::Guards::IsUnsigned<I>();
  }

  static constexpr u8   const ATBits       = Math::Boolean::getbitsoftype<AT>();
  static constexpr bool const is_read_only = std::is_const<ITER>::value;

  u8 alignement_offset{0}; //in Bytes
  ITER base{nullptr};      //floor aligned base-iterator

  I read_idx{0};           //Working Index Read
  I write_idx{0};          //Working Index Write

  AT buffer{}; //Buffer for Elements with Type ITER references

  static bool const is_little = []()->bool{
    return Enviroment::Endianness::is_little();
  };

//-----

  /**
   * @brief read
   * @param value2
   * @return
   */
  AT read(AT const value2){
    //Extract 1st Part of Value from Buffer + set it in Result
    I const start_buffer  = (alignement_offset<<3);
    I const length_buffer = ATBits-start_buffer;
    AT res = Registers::RegGetValue(buffer,start_buffer,length_buffer);

    //Extract 2nd Part of Value
    I const start_value2  = 0;
    I const length_value2 = start_buffer;
    AT const part2 = Registers::RegGetValue(value2,start_value2,length_value2);

    //Blend into result
    I const start_part2  = length_buffer;
    I const length_part2 = start_buffer;
    Registers::RegSetValue(res,part2,start_part2,length_part2);

    buffer = value2;

    return;
  }

  AT write(){

  }

  //----- The Common_Interface

  /**
   * @brief query_valid
   * @return true if basepointer is valid
   */
  bool query_valid() const{
    return Iterators::is_valid(base);
  }

  /**
   * @brief query_aligned
   * @return true if alignment offset is zero
   */
  bool query_aligned() const{
    return (0 == alignement_offset);
  }

  /**
   * @brief query_unaligned
   * @return true if alignment offset is non-zero
   */
  bool query_unaligned() const{
    return !query_aligned();
  }

  //----- The Equality_Interfac

  /**
   * @brief equal - check if read and write-position are identical
   * @param other
   * @return
   */
  bool equal(unaligned_iterator const& other) const{
    //Read Index
    auto const it1   = Iterators::advance(base,read_idx);
    auto const it2   = Iterators::advance(other.base,other.read_idx);
    auto const addr1 = Iterators::it2uintcast(it1);
    auto const addr2 = Iterators::it2uintcast(it2);
    bool eq  = (addr1 == addr2);

    if(!is_read_only){
      //Write Index
      auto const it3   = Iterators::advance(base,write_idx);
      auto const it4   = Iterators::advance(other.base,other.write_idx);
      auto const addr3 = Iterators::it2uintcast(it3);
      auto const addr4 = Iterators::it2uintcast(it4);
      eq &= (addr3 == addr4);
    }
    return eq;
  }

  //----- The AddSub_Interface

  /**
   * @brief reposition_with_offset - move basepointer to new position
   * @param offset
   * @param substract
   * @note read and write are restted
   */
  void reposition_with_offset(size_t const offset, bool const substract){
    if(substract){
      //substract offset
      base = Iterators::radvance(base,offset);
    }else{
      //add offset
      base = Iterators::advance(base,offset);
    }
    //reset read_write index
    read_idx  = 0;
    write_idx = 0;

    return;
  }

  /**
   * @brief add - reposition Iterator by offset in positive direction
   * @param offset
   * @return
   */
  unaligned_iterator& add(size_t const offset){
    reposition_with_offset(offset,false);
    return *this;
  }

  /**
   * @brief sub - reposition Iterator by offset in negative direction
   * @param offset
   * @return
   */
  unaligned_iterator& sub(size_t const offset){
    reposition_with_offset(offset,true);
    return *this;
  }

  /**
   * @brief add_cpy - construct a repositioned iterator with adding
   * @param offset
   * @return
   */
  unaligned_iterator add_cpy(size_t const offset){
    unaligned_iterator cpy = *this;
    cpy += offset;
    return cpy;
  }

  /**
   * @brief sub_cpy - construct a repositioned iterator with substracting
   * @param offset
   * @return
   */
  unaligned_iterator sub_cpy(size_t const offset){
    unaligned_iterator cpy = *this;
    cpy -= offset;
    return cpy;
  }

  //----- The PrePost Interface

  /**
   * @brief postincrement
   * @return
   */
  unaligned_iterator postincrement(){
    unaligned_iterator cpy = *this;
    UNUSED_RETVAL(add(1));
    return cpy;
  }

  /**
   * @brief postdecrement
   * @return
   */
  unaligned_iterator postdecrement(){
    unaligned_iterator cpy = *this;
    UNUSED_RETVAL(sub(1));
    return cpy;
  }

  /**
   * @brief preincrement
   * @return
   */
  unaligned_iterator& preincrement(){
    return add(1);
  }

  /**
   * @brief predecrement
   * @return
   */
  unaligned_iterator& predecrement(){
    return sub(1);
  }

  //-----

  /**
   * @brief preprocessing - executed prior to first read / write
   * @param startpos
   */
  void preprocessing(I const startpos=0){
    idx = startpos;

    if(this->is_unaligned()){
      //Pre-Read (we want to have big endian in any case...)
      buffer = Enviroment::Endianness::little2big(Iterators::get_at(base,idx),Enviroment::Endianness::is_little());

      ++idx;
    }
    return;
  }

  /**
   * @brief postprocessing - executed prior to last write
   */
  void postprocessing(){
    if(this->is_unaligned()){
      if(is_read_only){
        //Post-Write
      }
    }
    return;
  }

  //-----



  //-----

public:
  unaligned_iterator()=delete;
  explicit unaligned_iterator(ITER it):
      alignement_offset(Iterators::align_offset<AT>(it)),
      base(Iterators::floor_align<AT>(it)){};
  ~unaligned_iterator()=default;

  unaligned_iterator(unaligned_iterator const& other)=default;
  unaligned_iterator(unaligned_iterator&& other)=default;

  unaligned_iterator& operator=(unaligned_iterator const& other)=default;
  unaligned_iterator& operator=(unaligned_iterator&& other)=default;

  //-----



  //-----

  /**
   * @brief read_next
   * @return
   */
  T read_next(){


    //Aligned Read (we want to have big endian in any case...)
    auto rd = Enviroment::Endianness::little2big(Iterators::get_at(base,idx),Enviroment::Endianness::is_little());
    //Assemble Result from 2 aligned reads
    auto res = read(rd);
    //prepare next read
    ++read_idx;
    return;
  }

  /**
   * @brief write_next
   * @param element
   */
  void write_next(T const& element){
    if(!is_read_only){

      //prepare next write
      ++idx;
    }
    return;
  }

  //-----

  AT* operator->(){
    return;
  }



  //----- Comparison operators

  friend bool operator<(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    auto const uint_it_a = Iterators::it2uintcast(Iterators::advance(lhs.base, lhs.idx));
    auto const uint_it_b = Iterators::it2uintcast(Iterators::advance(rhs.base, rhs.idx));
    return uint_it_a < uint_it_b;
  }
  friend bool operator>(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    return rhs < lhs;
  }
  friend bool operator<=(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    return !(lhs > rhs);
  }
  friend bool operator>=(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    return !(lhs < rhs);
  }

  //----- (in)equality operator

  friend bool operator==(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    auto const uint_it_a = Iterators::it2uintcast(Iterators::advance(lhs.base, lhs.idx));
    auto const uint_it_b = Iterators::it2uintcast(Iterators::advance(rhs.base, rhs.idx));
    return uint_it_a == uint_it_b;
  }
  friend bool operator!=(unaligned_iterator const& lhs, unaligned_iterator const& rhs){
    return !(lhs == rhs);
  }

  //-----

};

} //namespace

#endif // ITERATOR_UNALIGNED_DATA_H
