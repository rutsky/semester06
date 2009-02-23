/*
 * numeric.hpp
 * Numerical methods.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_HPP
#define NUMERIC_HPP

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

// Using boost::uBLAS algebra types.
namespace numeric
{
  namespace ublas = boost::numeric::ublas;

  typedef double value_type; // TODO
  typedef ublas::vector<double> vector_type;
  typedef ublas::matrix<double> matrix_type;
  
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
} // End of namespace 'numeric'.

#endif // NUMERIC_HPP
