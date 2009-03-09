/*
 * simplex_alg.hpp
 * Simplex algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_SIMPLEX_ALG_HPP
#define NUMERIC_SIMPLEX_ALG_HPP

#include <iterator>
#include <algorithm>
#include <numeric>
#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/bind.hpp>

#include "numeric_common.hpp"

#include "li_vectors.hpp"
#include "iterator.hpp"
#include "submatrix.hpp"

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
  enum simplex_result_type
  {
    srt_min_found,   // Function has minimum and it was founded.
    srt_not_limited, // Function is not limited from below.
    srt_none,        // Set of admissible points is empty.
  };
  
  // Types of searching first basic vector results.
  enum first_basic_vector_result_type
  {
    fbrt_found, // Found first basic vector.
    fbrt_none,  // Set of admissible points is empty.
  };
  
  // Types of searching next basic vector results.
  enum next_basic_vector_result_type
  {
    nbrt_next_basic_vector_found, // Found next basic vector.
    nbrt_min_found,               // Current basic vector is solution of problem.
    nbrt_not_limited,             // Function is not limited from below.
    nbrt_none,                    // Set of admissible points is empty.
  };
  
  namespace
  {
    template< class MatrixType, class VectorType >
    bool assert_basic_vector( MatrixType const &A, VectorType const &b, VectorType const &x )
    {
      typedef typename MatrixType::value_type         value_type;
      typedef ublas::vector<value_type>               vector_type;
      typedef ublas::matrix<value_type>               matrix_type;
      typedef ublas::scalar_traits<value_type>        scalar_traits_type;
      typedef ublas::basic_range<size_t, long>        range_type;
      typedef std::vector<size_t>                     range_container_type;
      typedef linear_independent_vectors<vector_type> li_vectors_type;
      
      // TODO: Assert that value types in all input is compatible.
      BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
      BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
      
      range_type const N(0, A.size2()), M(0, A.size1());
      
      // TODO
      BOOST_ASSERT(N.size() > 0);
      BOOST_ASSERT(M.size() > 0);
      
      BOOST_ASSERT(M.size() < N.size());
      BOOST_ASSERT(is_linear_independent(matrix_rows_begin(A), matrix_rows_end(A)));
      BOOST_ASSERT(x.size() == N.size());
      BOOST_ASSERT(b.size() == M.size());
      
      range_container_type Nkp;
      copy_if(N.begin(), N.end(), std::back_inserter(Nkp), 
          boost::bind<bool>(std::logical_not<bool>(), boost::bind<bool>(&scalar_traits_type::equals, 0.0, boost::bind<value_type>(x, _1))));
      BOOST_ASSERT(Nkp.size() > 0);
      BOOST_ASSERT(Nkp.size() <= M.size());
      
      li_vectors_type basicVectorLICols;
      BOOST_ASSERT(is_linear_independent(matrix_columns_begin(submatrix(A, M.begin(), M.end(), Nkp.begin(), Nkp.end())),
                                         matrix_columns_end  (submatrix(A, M.begin(), M.end(), Nkp.begin(), Nkp.end()))));
      
      for (size_t r = 0; r < M.size(); ++r)
      {
        value_type const result = std::inner_product(ublas::row(A, r).begin(), ublas::row(A, r).end(), x.begin(), 0);
        BOOST_ASSERT(scalar_traits_type::equals(result, b[r]));
      }
      
      return true;
    }
  } // End of anonymous namespace.

  // Finds next basic vector, that closer to goal of linear programming problem.
  template< class MatrixType, class VectorType >
  inline 
  first_basic_vector_result_type
    find_first_basic_vector( MatrixType const &A, VectorType const &b, VectorType const &c,  
                             VectorType &basicV )
  {
    typedef typename VectorType::value_type    value_type;
    typedef ublas::vector<value_type>          vector_type;
    typedef ublas::matrix<value_type>          matrix_type;
    typedef ublas::scalar_traits<value_type>   scalar_traits_type;
    typedef ublas::scalar_vector<value_type>   scalar_vector_type;
    typedef ublas::basic_range<size_t, long>   range_type;
    typedef ublas::identity_matrix<value_type> identity_matrix_type;
    typedef ublas::matrix_row<matrix_type>     matrix_row_type;
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    // TODO: Assert that value types in all input is compatible.

    range_type const N(0, A.size2()), M(0, A.size1());
    
    // TODO
    BOOST_ASSERT(N.size() > 0);
    BOOST_ASSERT(M.size() > 0);
    
    BOOST_ASSERT(M.size() < N.size());
    BOOST_ASSERT(is_linear_independent(matrix_rows_begin(A), matrix_rows_end(A)));
    BOOST_ASSERT(basicV.size()     == N.size());
    BOOST_ASSERT(c.size()          == N.size());
    BOOST_ASSERT(b.size()          == M.size());
    
    vector_type newC(N.size() + M.size()), newB(M.size()), newBasicV(N.size() + M.size()), newResultV(N.size() + M.size());
    matrix_type newA(M.size(), N.size() + M.size());
    
    // Filling new 'c'.
    ublas::project(newC, ublas::range(0, N.size())) = scalar_vector_type(N.size(), 0);
    ublas::project(newC, ublas::range(N.size(), N.size() + M.size())) = scalar_vector_type(M.size(), 1);
    
    // Filling new 'A' and new 'b'.
    for (size_t r = 0; r < M.size(); ++r)
    {
      value_type const factor = (b[r] >= 0 ? 1 : -1);
      
      // TODO:
      //ublas::project(matrix_row_type(ublas::row(newA, r)), ublas::range(0, N.size())) = factor * ublas::row(A, r);
      matrix_row_type row(newA, r);
      ublas::vector_range<matrix_row_type>(row, ublas::range(0, N.size())) = factor * ublas::row(A, r);
      
      newB[r] = factor * b[r];
    }
    project(newA, ublas::range(0, M.size()), ublas::range(N.size(), N.size() + M.size())) = identity_matrix_type(M.size());
    
    // Filling new basic vector.
    ublas::project(newBasicV, ublas::range(0, N.size())) = scalar_vector_type(N.size(), 0);
    ublas::project(newBasicV, ublas::range(N.size(), N.size() + M.size())) = newB;
    BOOST_ASSERT(assert_basic_vector(newA, newB, newBasicV));
    
    // Solving auxiliary problem.
    simplex_result_type const result = solve_augment_with_basic_vector(newA, newB, newC, newBasicV, newResultV);
    BOOST_ASSERT(result == srt_min_found); // it always has solution
    
    if (scalar_traits_type::equals(ublas::vector_norm_inf<vector_type>::apply(ublas::project(newBasicV, ublas::range(N.size(), N.size() + M.size()))), 0))
    {
      // Found basic vector.
      basicV = ublas::project(newResultV, ublas::range(0, N.size()));
      assert_basic_vector(A, b, basicV);
      return fbrt_found;
    }
    else
    {
      // Set of admissable points is empty.
      return fbrt_none;
    }
  }
  
  // Finds next basic vector, that closer to goal of linear programming problem.
  template< class MatrixType, class VectorType >
  inline 
  next_basic_vector_result_type 
    find_next_basic_vector( MatrixType const &A, VectorType const &b, VectorType const &c, 
                            VectorType const &basicV, VectorType &nextBasicV )
  {
    typedef typename MatrixType::value_type         value_type;
    typedef ublas::vector<value_type>               vector_type;
    typedef ublas::matrix<value_type>               matrix_type;
    typedef ublas::scalar_traits<value_type>        scalar_traits_type;
    typedef ublas::basic_range<size_t, long>        range_type;
    typedef std::vector<size_t>                     range_container_type;
    typedef linear_independent_vectors<vector_type> li_vectors_type;
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    // TODO: Assert that value types in all input is compatible.
  
    range_type const N(0, A.size2()), M(0, A.size1());
    
    // TODO
    BOOST_ASSERT(N.size() > 0);
    BOOST_ASSERT(M.size() > 0);
    
    BOOST_ASSERT(M.size() < N.size());
    BOOST_ASSERT(is_linear_independent(matrix_rows_begin(A), matrix_rows_end(A)));
    BOOST_ASSERT(basicV.size()     == N.size());
    BOOST_ASSERT(nextBasicV.size() == N.size());
    BOOST_ASSERT(c.size()          == N.size());
    BOOST_ASSERT(b.size()          == M.size());
    
    BOOST_ASSERT(assert_basic_vector(A, b, basicV));
        
    range_container_type Nkp, Nkz, Nk, Lk;
    
    // Filling 'Nkp'.
    copy_if(N.begin(), N.end(), std::back_inserter(Nkp), 
        boost::bind<bool>(std::logical_not<bool>(), boost::bind<bool>(&scalar_traits_type::equals, 0.0, boost::bind<value_type>(basicV, _1))));
    BOOST_ASSERT(Nkp.size() > 0);
    BOOST_ASSERT(Nkp.size() <= M.size());
    
    // Filling 'Nkz' and 'Nk'.
    Nk.assign(Nkp.begin(), Nkp.end());
    
    li_vectors_type basicVectorLICols;
    BOOST_VERIFY(basicVectorLICols.insert(matrix_columns_begin(submatrix(A, M.begin(), M.end(), Nkp.begin(), Nkp.end())),
                                          matrix_columns_end  (submatrix(A, M.begin(), M.end(), Nkp.begin(), Nkp.end()))));
    
    // Filling 'Nk' with linear independent columns of 'A' to square size.
    for (size_t c = 0; c < N.size(); ++c)
    {
      if (Nk.size() < M.size())
      {
        // Trying to apped column 'c' to 'Nk'.
        if (std::find(Nk.begin(), Nk.end(), c) == Nk.end())
        {
          if (basicVectorLICols.is_independent(ublas::column(A, c)))
          {
            // Appending 'c' column to 'Nk'.
            Nk.push_back(c);
            Nkz.push_back(c);
            basicVectorLICols.insert(ublas::column(A, c));
          }
          else
          {
            // Column 'c' is not linear independent with columns in 'Nk'.
          }
        }
        else
        {
          // Column 'c' is already in 'Nk'.
        }
      }
      else
      {
        // Filled 'Nk'.
        BOOST_ASSERT(Nk.size() == M.size());
        break;
      }
    }
    
    std::sort(Nk.begin(), Nk.end());

    // Filling 'Lk'
    std::set_difference(N.begin(), N.end(), Nk.begin(), Nk.end(), std::back_inserter(Lk));
    
    BOOST_ASSERT(Nk.size() == M.size());
    BOOST_ASSERT(Nkz.size() + Nkp.size() == M.size());
    BOOST_ASSERT(Lk.size() == N.size() - M.size());
    
    // debug
    std::cout << "N: ";
    std::copy(N.begin(), N.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";
    
    std::cout << "M: ";
    std::copy(M.begin(), M.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";

    std::cout << "Nkp: ";
    std::copy(Nkp.begin(), Nkp.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";

    std::cout << "Nkz: ";
    std::copy(Nkz.begin(), Nkz.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";

    std::cout << "Nk: ";
    std::copy(Nk.begin(), Nk.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";

    std::cout << "Lk: ";
    std::copy(Lk.begin(), Lk.end(), std::ostream_iterator<size_t>(std::cout, " "));
    std::cout << "\n";
    
    //std::copy(Nkp.begin(), Nkp.end(), std::ostream_iterator<size_t>(std::cout, " "));
    //std::cout << r << "\n";
    // end of debug
    
    
    
    return nbrt_none; // debug
  }
  
  // Solves linear programming problem described in augment form:
  //   min (c^T * x), where x: x >= 0, A * x = b,
  // using provided first basic vector.
  template< class MatrixType, class VectorType >
  inline 
  simplex_result_type 
    solve_augment_with_basic_vector( MatrixType const &A, VectorType const &b, VectorType const &c, 
                                     VectorType const &basicV, VectorType &resultV )
  {
    typedef typename MatrixType::value_type value_type;
    typedef ublas::vector<value_type>       vector_type;
    
    vector_type curBasicV = basicV;
    
    while (true)
    {
      vector_type nextBasicV(basicV.size());
      next_basic_vector_result_type const result = find_next_basic_vector(A, b, c, curBasicV, nextBasicV);
      switch (result)
      {
      case nbrt_next_basic_vector_found:
        curBasicV = nextBasicV;
        break;
        
      case nbrt_min_found:
        BOOST_ASSERT(curBasicV == nextBasicV);
        resultV = curBasicV;
        return srt_min_found;
        break;
        
      case nbrt_not_limited:
        return srt_not_limited;
        break;
        
      case nbrt_none:
        return srt_none;
        break;
      }
    }
    
    // Impossible case.
    BOOST_ASSERT(0);
    return srt_none; 
  }
    
  // Solves linear programming problem described in augment form:
  //   min (c^T * x), where x: x >= 0, A * x = b,
  template< class MatrixType, class VectorType >
  inline 
  simplex_result_type solve_augment( MatrixType const &A, VectorType const &b, VectorType const &c, 
                                     VectorType &resultV )
  {
    typedef typename MatrixType::value_type         value_type;
    typedef ublas::vector<value_type>               vector_type;
    typedef ublas::matrix<value_type>               matrix_type;
    typedef ublas::scalar_traits<value_type>        scalar_traits_type;
    typedef ublas::basic_range<size_t, long>        range_type;
    typedef std::vector<size_t>                     range_container_type;
    typedef linear_independent_vectors<vector_type> li_vectors_type;
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    // TODO: Assert that value types in all input is compatible.
  
    range_type const N(0, A.size2()), M(0, A.size1());
    
    // TODO
    BOOST_ASSERT(N.size() > 0);
    BOOST_ASSERT(M.size() > 0);
    
    BOOST_ASSERT(M.size() < N.size());
    BOOST_ASSERT(is_linear_independent(matrix_rows_begin(A), matrix_rows_end(A)));
    BOOST_ASSERT(resultV.size()    == N.size());
    BOOST_ASSERT(c.size()          == N.size());
    BOOST_ASSERT(b.size()          == M.size());

    vector_type firstBasicV(N.size());
    first_basic_vector_result_type const result = find_first_basic_vector(A, b, c, firstBasicV);
    
    if (result == fbrt_found)
    {
      return solve_augment_with_basic_vector(A, b, c, firstBasicV, resultV);
    }
    else
    {
      BOOST_ASSERT(result == fbrt_none);
      // Set of admissible points is empty.
      return srt_none;
    }
  }
} // End of namespace 'simplex'.
} // End of namespace 'numeric'.

#endif // NUMERIC_SIMPLEX_ALG_HPP
