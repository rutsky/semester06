/*
 * numeric_common.hpp
 * Numerical methods.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_NUMERIC_COMMON_HPP
#define NUMERIC_NUMERIC_COMMON_HPP

#include <iostream> // debug

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>

#include <boost/numeric/ublas/traits.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/hermitian.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/numeric/ublas/detail/concepts.hpp>

namespace numeric
{
  namespace ublas = boost::numeric::ublas;
  
  using ublas::scalar_traits;
  
  using ublas::vector;
  using ublas::vector_expression;
  using ublas::zero_vector;
  using ublas::scalar_vector;
  
  using ublas::matrix;
  using ublas::matrix_expression;
  using ublas::zero_matrix;
  using ublas::row;
  using ublas::column;
  
  using ublas::prod;
  using ublas::inner_prod;
  
  using ublas::basic_range;

  // TODO: Move boost stuff to separate file.
  template< class OP, class E > 
  BOOST_UBLAS_INLINE
  typename ublas::vector_unary_traits<E, OP>::result_type
      apply_to_all( ublas::vector_expression<E> const &e, OP const &op = OP() ) 
  {
    typedef typename ublas::vector_unary_traits<E, OP>::expression_type expression_type;
    return expression_type(e());
  }
  
  template <class OP, class E>
  BOOST_UBLAS_INLINE
  typename ublas::matrix_unary1_traits<E, OP>::result_type
      apply_to_all( ublas::matrix_expression<E> const & e, OP const &op = OP() )
  {
    typedef typename ublas::matrix_unary1_traits<E, OP>::expression_type expression_type;
    return expression_type(e());
  }
  
  namespace functor
  {
    template< class T >
    class abs
    {
    public:
      typedef T value_type;
      typedef T result_type;
      
      abs()
      {}
      
      static result_type apply( const value_type &x )
      {
        return ublas::type_traits<T>::type_abs(x);
      }
    };
  } // End of namespace 'functor'.
  
  // Some people says, that copy_if was dropped from the STL by accident.
  template< class SrcFwdIterator, class DstOutIterator, class Pred >
  DstOutIterator copy_if( SrcFwdIterator first, SrcFwdIterator beyond, DstOutIterator res, Pred Pr )
  {
    while (first != beyond)
    {
      if (Pr(*first))
        *res++ = *first;
      ++first;
    }
  
    return res;
  }
  
  template< typename T, int size >
  size_t array_size( T (&)[size] ) 
  { 
    return size; 
  };
  
  template< class T >
  T & make_min( T &a, T const &b )
  {
    if (a > b) a = b;
    return a;
  }
  
  template< class T >
  T & make_max( T &a, T const &b )
  {
    if (a < b) a = b;
    return a;
  }
  
  template< class T >
  T abs( T const &v )
  {
    return v < 0 ? -v : v;
  }

  template< class T >
  T sqr( T const &x )
  {
    return x * x;
  }

  struct DummyOutputIterator
  {
    // FIXME! This iterator is probably not correct!
    DummyOutputIterator const & operator *  () const { return *this; }
    DummyOutputIterator       & operator *  ()       { return *this; }
    
    template< class T >
    DummyOutputIterator       & operator =  ( T const & ) { return *this; }
    
    DummyOutputIterator const & operator ++ ()      const { return *this; }
    DummyOutputIterator       & operator ++ ()            { return *this; }
    DummyOutputIterator const & operator ++ ( int ) const { return *this; }
    DummyOutputIterator       & operator ++ ( int )       { return *this; }
  };
  
  struct CountingOutputIterator
  {
    // TODO: This is freaky.
    CountingOutputIterator( size_t &counter )
      : counter_(counter)
    {}
    
    CountingOutputIterator( CountingOutputIterator const &x )
      : counter_(x.counter_)
    {}
    
    size_t count() const { return counter_; }
    
    // FIXME! This iterator is probably not correct!
    CountingOutputIterator const & operator *  () const { return *this; }
    CountingOutputIterator       & operator *  ()       { return *this; }
    
    template< class T >
    CountingOutputIterator       & operator =  ( T const & ) { ++counter_; return *this; }
    
    CountingOutputIterator const & operator ++ ()      const { return *this; }
    CountingOutputIterator       & operator ++ ()            { return *this; }
    CountingOutputIterator const & operator ++ ( int ) const { return *this; }
    CountingOutputIterator       & operator ++ ( int )       { return *this; }
    
  private:
    size_t &counter_;
  };
  
  // TODO: There must be better ways than implementing own class.
  struct true_predicate
  {
    // TODO: Inherit from std::...predicate... ot something.
    bool operator()() const { return true; }
    template< class T >
    bool operator()( T const & ) const { return true; }
  };
  struct false_predicate
  {
    // TODO: Inherit from std::...predicate... ot something.
    bool operator()() const { return false; }
    template< class T >
    bool operator()( T const & ) const { return false; }
  };
} // End of namespace 'numeric'.

#endif // NUMERIC_NUMERIC_COMMON_HPP
