/*
 * linear_system.hpp
 * System of linear equations related stuff.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 22.02.2009
 */

#ifndef NUMERIC_LINEAR_SYSTEM_HPP
#define NUMERIC_LINEAR_SYSTEM_HPP

#include "numeric_common.hpp"

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include "invert_matrix.hpp"

namespace numeric
{
namespace linear_system
{
  template< class M, class EX, class EB > 
  inline
  bool solve( matrix_expression<M> const &A, vector_expression<EB> const &b,
              vector_expression<EX> &x )
  {
    typedef typename M::value_type scalar_type; // TODO
    typedef vector<scalar_type>    vector_type;
    typedef matrix<scalar_type>    matrix_type;
    
    BOOST_ASSERT(A().size1() == A().size2());
    BOOST_ASSERT(A().size1() == b().size());
    
    size_t const size = A().size1();
    
    matrix_type invA(size, size);
    if (!invert_matrix(A(), invA))
    {
      output_matrix_octave(std::cout, A); // debug
      // Linear system matrix has zero determinant.
      return false;
    }
    
    x() = prod(invA, b());
    
    BOOST_ASSERT(eq_zero(norm_inf(vector_type(prod(A(), x()) - b())), 1e-4)); // TODO: Precision.
    
    return true;
  }
} // End of namespace 'linear_system'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_SYSTEM_HPP
