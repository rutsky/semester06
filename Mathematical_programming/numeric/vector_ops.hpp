/*
 * vector_ops.hpp
 * Operations with vectors.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 21.05.2009
 */

#ifndef NUMERIC_VECTOR_OPS_HPP
#define NUMERIC_VECTOR_OPS_HPP
 
#include "numeric_common.hpp"

#include <algorithm>
#include <vector>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>

#include "subvector.hpp"

namespace numeric
{
  // TODO: Need vector extending operations.
  
  template< class VectorType >
  inline
  vector_subvector<VectorType> subvectori( VectorType &data, size_t start, size_t size )
  {
    basic_range<size_t, long> range(start, size);
    return subvector(data, range.begin(), range.end());
  }

  template< class VectorType >
  inline
  vector_subvector<VectorType const> subvectori( VectorType const &data, size_t start, size_t size )
  {
    basic_range<size_t, long> range(start, size);
    return subvector(data, range.begin(), range.end());
  }
  
  // FIXME: Using scalar type from E1! Must use type with more precision.
  template< class E1, class E2 >
  inline
  vector<typename E1::value_type> paste( vector_expression<E1> const &v1, vector_expression<E2> const &v2 )
  {
    vector<typename E1::value_type> result(v1().size() + v2().size());
    subvectori(result, 0,           v1().size()) = v1();
    subvectori(result, v1().size(), v2().size()) = v2();
    return result;
  }
  
  template< class E >
  inline
  vector<typename E::value_type> paste( vector_expression<E> const &v, typename E::value_type const &val )
  {
    vector<typename E::value_type> result(v().size() + 1);
    subvectori(result, 0, v().size()) = v();
    result(v().size()) = val;
    return result;
  }

  template< class E >
  inline
  vector<typename E::value_type> paste( typename E::value_type const &val, vector_expression<E> const &v )
  {
    vector<typename E::value_type> result(v().size() + 1);
    result(0) = val;
    subvectori(result, 1, v().size()) = v();
    return result;
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_VECTOR_OPS_HPP
