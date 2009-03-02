/*
 * simplex_alg.hpp
 * Simplex algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_SIMPLEX_ALG_HPP
#define NUMERIC_SIMPLEX_ALG_HPP

#include <vector>

#include "numeric_common.hpp"

#include "li_vectors.hpp"
#include "iterator.hpp"

namespace numeric
{
/*
template< class VectorType, class MatrixType >
class simplex
{
public:
  typedef VectorType vector_type; 
  typedef MatrixType matrix_type; 
  
  BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
  BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
*/

namespace simplex
{
  // TODO: Move implementation lower.
  
  // Types of linear programming solving results.
  enum result_type
  {
    rt_min_found,   // Function has minimum and it was founded.
    rt_not_limited, // Function is not limited from below.
    rt_none,        // Set of admissible points is empty.
  };

  // Finds next basic vector, that closer to goal of linear programming problem.
  template< class VectorType, class MatrixType >
  inline result_type find_next_basic_vector( VectorType const &c, MatrixType const &A, VectorType const &b, 
                                             VectorType const &basicV, VectorType &nextBasicV )
  {
    typedef typename VectorType::value_type  value_type;
    typedef ublas::vector<value_type>        vector_type;
    typedef ublas::matrix<value_type>        matrix_type;
    typedef ublas::scalar_traits<value_type> scalar_traits;
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    // TODO: Assert that value types in all input is compatible.
  
    BOOST_ASSERT(is_linear_independent(matrix_rows_begin(A), matrix_rows_end(A)));
    
    // TODO: Assert that:
    //   basicV is basic vector.
    
    
    
    return rt_none; // debug
  }

  
  // Solves linear programming problem described in augment form:
  //   min (c^T * x), where x: x >= 0, A * x = b
  /*
  inline result_type solve_augment( vector_type const &c, matrix_type const &A, vector_type const &b, 
                             vector_type const &startBasicV,
                             vector_type &result )
  {
    return rt_none; // debug
  }
  */
} // End of namespace 'simplex'.
} // End of namespace 'numeric'.

#endif // NUMERIC_SIMPLEX_ALG_HPP
