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
  
  using ublas::vector;
  using ublas::vector_expression;
  using ublas::matrix;
  using ublas::matrix_expression;

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
} // End of namespace 'numeric'.

#endif // NUMERIC_NUMERIC_COMMON_HPP
