/**
 * Baseclass and Interfaces for Iteratorclasses
 * Curiously recurring template pattern...
 */

#ifndef ITERATOR_BASE_API_H
#define ITERATOR_BASE_API_H

//for Copyright see this file
#include "modified_bsd.h"

#include "dtypes.h"
#include "iterator_ptr_wrapper.h"

namespace Iterator_Base_API {

/**
 * @brief Common Base for Iterators
 */
class Iterator_Base{
private:

  //...

public:

  Iterator_Base()=default;
  virtual ~Iterator_Base();

  Iterator_Base(Iterator_Base const& other)=default;
  Iterator_Base(Iterator_Base&& other) noexcept =default;

  Iterator_Base& operator=(Iterator_Base const& other)=default;
  Iterator_Base& operator=(Iterator_Base&& other) noexcept =default;

  //-----

};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

/**
 * @brief The Common_Interface class
 */
template<typename CRTP> class Common_Interface{
  explicit operator bool()const{
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->query_valid();
  }

  bool is_aligned() const{
   auto instance = Iterators::itcast<CRTP>(this);
   return instance->query_aligned();
  }

  bool is_unaligned() const{
   auto instance = Iterators::itcast<CRTP>(this);
   return instance->query_unaligned();
  }
};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

/**
 * @brief The Equality_Interface class
 */
template<typename CRTP> class Equality_Interface{
  bool operator==(CRTP const& other) const{
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->equal(other);
  }

  bool operator!=(CRTP const& other) const{
    auto instance = Iterators::itcast<CRTP>(this);
    return !(instance->equal(other));
  }
};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

/**
 * @brief The AddSub_Interface class (reposition of iterator)
 */
template<typename CRTP> class AddSub_Interface{
  CRTP operator+(size_t const offset){
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->add_cpy(offset);
  }

  CRTP operator-(size_t const offset){
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->sub_cpy(offset);
  }

  CRTP& operator+=(size_t const offset){
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->add(offset);
  }

  CRTP& operator-=(size_t const offset){
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->sub(offset);
  }

};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

/**
 * @brief The PrePost_Interface class
 */
template<typename CRTP> class PrePost_Interface{
public:

  CRTP const operator++(int){ // postfix
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->postincrement();
  }

  CRTP& operator++(){         // prefix
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->preincrement();
  }


  CRTP const operator--(int){ // postfix
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->postdecrement();
  }

  CRTP& operator--(){         // prefix
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->predecrement();
  }

};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

/**
 * @brief The Subscript_Interface class
 */
template<typename CRTP> class Subscript_Interface{
public:

  CRTP& operator[](size_t const idx) const{
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->postincrement(idx);
  }

  CRTP& operator[](size_t const idx){
    auto instance = Iterators::itcast<CRTP>(this);
    return instance->preincrement(idx);
  }

};

//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=



//--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=--=

} //namespace

#endif // ITERATOR_BASE_API_H
