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

#include <boost/bind.hpp>

#include "xmath.hpp"
#include "xmath_boost.hpp"

namespace numeric
{
  using namespace xmath;
  
  namespace ublas = boost::numeric::ublas;
  
  using ublas::vector;
  using ublas::vector_expression;
  using ublas::zero_vector;
  using ublas::scalar_vector;
  
  using ublas::matrix;
  using ublas::matrix_expression;
  using ublas::zero_matrix;
  using ublas::identity_matrix;
  using ublas::scalar_matrix;
  using ublas::row;
  using ublas::matrix_row;
  using ublas::column;
  
  using ublas::prod;
  using ublas::inner_prod;
  using ublas::trans;
  
  using ublas::basic_range;

  // TODO: Move boost stuff to separate file.
  template< class OP, class E > 
  inline
  typename ublas::vector_unary_traits<E, OP>::result_type
      apply_to_all( ublas::vector_expression<E> const &e ) 
  {
    typedef typename ublas::vector_unary_traits<E, OP>::expression_type expression_type;
    return expression_type(e());
  }
  
  template <class OP, class E>
  inline
  typename ublas::matrix_unary1_traits<E, OP>::result_type
      apply_to_all( ublas::matrix_expression<E> const & e )
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
    
    template< class T >
    class adjust
    {
    public:
      typedef T value_type;
      typedef T result_type;
      
      adjust()
      {}
      
      static result_type apply( const value_type &x )
      {
        // TODO
        return xmath::adjust(x);
      }
    };
  } // End of namespace 'functor'.
  
  // Some people says, that copy_if was dropped from the STL by accident.
  template< class SrcFwdIterator, class DstOutIterator, class Pred >
  inline
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
  
  template< class SrcInpIterator, class DstOutIterator >
  inline
  DstOutIterator copy_n( SrcInpIterator first, size_t n, DstOutIterator res )
  {
    for (size_t i = 0; i < n; ++i)
      *res++ = *first++;
  
    return res;
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
  
  // TODO: Use xmath.hpp implementation.
  template< class S > inline bool equal        ( S a, S b, S eps = 0. ) { return abs(a - b) <= eps; }
  template< class S > inline bool greater      ( S a, S b, S eps = 0. ) { return a - b >  -eps; }
  template< class S > inline bool greater_equal( S a, S b, S eps = 0. ) { return a - b >= -eps; }
  template< class S > inline bool less         ( S a, S b, S eps = 0. ) { return a - b <  +eps; }
  template< class S > inline bool less_equal   ( S a, S b, S eps = 0. ) { return a - b <= +eps; }
  
  template< class S > inline bool equal_zero   ( S a,      S eps = 0. ) { return equal(a, 0., eps); }

  template< class V1, class V2, class S >
  inline
  bool by_component_equal( V1 const &a, V2 const &b, S eps = 0. )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V1>));
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V2>));
    BOOST_ASSERT(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
      if (!equal(a(i), b(i), eps))
        return false;
        
    return true;
  }
  
  template< class V1, class V2, class S >
  inline
  bool by_component_less( V1 const &a, V2 const &b, S eps = 0. )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V1>));
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V2>));
    BOOST_ASSERT(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
      if (!less(a(i), b(i), eps))
        return false;
        
    return true;
  }
  
  template< class V1, class V2, class S >
  inline
  bool by_component_less_equal( V1 const &a, V2 const &b, S eps = 0. )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V1>));
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V2>));
    BOOST_ASSERT(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
      if (!less_equal(a(i), b(i), eps))
        return false;
        
    return true;
  }
  
  template< class V1, class V2, class S >
  inline
  bool by_component_greater( V1 const &a, V2 const &b, S eps = 0. )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V1>));
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V2>));
    BOOST_ASSERT(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
      if (!greater(a(i), b(i), eps))
        return false;
        
    return true;
  }
  
  template< class V1, class V2, class S >
  inline
  bool by_component_greater_equal( V1 const &a, V2 const &b, S eps = 0. )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V1>));
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V2>));
    BOOST_ASSERT(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)
      if (!greater_equal(a(i), b(i), eps))
        return false;
        
    return true;
  }
  
  // Changes vector size on the fly.
  template< class Vector >
  struct vector_resizer
  {
    vector_resizer()
      : size_(0)
    {}
    
    vector_resizer( size_t toSize )
      : size_(toSize)
    {}
    
    vector_resizer( vector_resizer const &vr )
      : size_(vr.size_)
    {}
    
    template< class V >
    Vector operator () ( vector_expression<V> const &v )
    {
      Vector result = v;
      result.resize(size_, true);
      return result;
    }
    
  private:
    size_t size_;
  };
} // End of namespace 'numeric'.

#endif // NUMERIC_NUMERIC_COMMON_HPP
