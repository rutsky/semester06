/*
 * simplex_alg.hpp
 * Simplex algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef SIMPLEX_ALG_HPP
#define SIMPLEX_ALG_HPP

#include <vector>

#include "numeric.hpp"

namespace numeric
{
namespace simplex
{
  // TODO: Move implementation lower.
  
  // Find linear indepenent rows and columns using Gauss method.
  // Returns number of founded rows and columns.
  template< class RowsOutIterator, class ColsOutIterator >
  inline std::pair<size_t, size_t> 
      li_submatrix( matrix_type const &M, RowsOutIterator rowsOut, ColsOutIterator colsOut )
  {
    matrix_type W = apply_to_all(M, functor::abs<value_type>());
    
    size_t nIndependentRows(0), nIndependentCols(0);
    
    for (size_t r = 0; r < W.size1(); ++r)
    {
      // Searching for maximum element in row.
      //value_type max(0);
      for (size_t c = 0; c < W.size2(); ++c)
      {
        
      }
    }
    
    return std::make_pair(nIndependentRows, nIndependentCols);
  }
  
  // Types of linear programming solving results.
  enum result_type
  {
    rt_min_found,   // Function has minimum and it was founded.
    rt_not_limited, // Function is not limited from below.
    rt_none,        // Set of admissible points is empty.
  };

  // Finds next basic vector, that closer to goal of linear programming problem.
  inline result_type find_next_basic_vector( vector_type const &c, matrix_type const &A, vector_type const &b, 
                                      vector_type const &basicV, vector_type &nextBasicV )
  {
    // TODO: Assert that:
    //   rank(A) is A.size2(),
    //   basicV is basic vector.
    
    return rt_none; // debug
  }

  
  // Solves linear programming problem described in augment form:
  //   min (c^T * x), where x: x >= 0, A * x = b
  inline result_type solve_augment( vector_type const &c, matrix_type const &A, vector_type const &b, 
                             vector_type const &startBasicV,
                             vector_type &result )
  {
    return rt_none; // debug
  }
} // End of namespace 'simplex'.
} // End of namespace 'numeric'.

#endif // SIMPLEX_ALG_HPP
