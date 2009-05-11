/*
 * kelley_cutting_plane.hpp
 * Kelley's convex cutting plane algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 27.04.2009
 */

#ifndef NUMERIC_KELLEY_CUTTING_PLANE_HPP
#define NUMERIC_KELLEY_CUTTING_PLANE_HPP

#include "numeric_common.hpp"

#include "linear_problem.hpp"
#include "simplex_alg.hpp"

#include <vector>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace numeric
{
namespace kelley_cutting_plane
{
  // For details see
  //   David G. Luenberger, Yinyu Ye
  //   Linear and Nonlinear Programming, Third Edition
  //   section 14.8 Kelley's convex cutting plane algorithm (p. 463).
  
  // Finds linear function minimum prefer to convex differentiable limits.
  //   min c^T * x,  g(x) <= 0
  // g(x) and grad g(x) are defined by coordinates through iterators.
  // This method requires initial linear limits which satisfies g(x) <= 0 condition,
  //   these limits are defined by two vectors `startLimitLo' and `startLimitHi' this like:
  //   x >= startLimitLo
  //   x <= startLimitHi
  //   (which is 2 * n inequations).
  // TODO: Handle more cases, return value should be enumeration of different exit statuses.
  template< class V, class FuncIterator, class GradFuncIterator >
  inline 
  vector<typename V::value_type> 
    find_min( V const &c, 
              FuncIterator     funcBegin,     FuncIterator     funcEnd,
              GradFuncIterator gradFuncBegin, GradFuncIterator gradFuncEnd,
              V const &startLimitLo, V const &startLimitHi )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef typename V::value_type                      scalar_type;
    typedef vector<scalar_type>                         vector_type;
    typedef matrix<scalar_type>                         matrix_type;
    typedef zero_matrix<scalar_type>                    zero_matrix;
    typedef scalar_traits<scalar_type>                  scalar_traits_type;
    typedef typename FuncIterator::value_type           function_type;
    typedef typename GradFuncIterator::value_type       gradient_function_type;
    typedef linear_problem::converter_type<scalar_type> converter_template_type;
    
    typedef linear_problem::common_linear_problem<scalar_type>    common_linear_problem_type;
    typedef linear_problem::canonical_linear_problem<scalar_type> canonical_linear_problem_type;
    typedef linear_problem::converter_type<scalar_type>           converter_type;

    // TODO: Using same type in much places now (like scalar_type).
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<function_type,          scalar_type, vector_type>));
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<gradient_function_type, vector_type, vector_type>));
    
    // TODO: Assert that input limits are valid (startLimitLo < startLimitHi and they rise correct LP).
    
    size_t const n = c.size();
    
    BOOST_ASSERT(n > 0);
    BOOST_ASSERT(startLimitLo.size() == n && startLimitHi.size() == n);

    // Storing limit function and its gradient.
    std::vector<function_type>          g    (funcBegin,     funcEnd);
    std::vector<gradient_function_type> gGrad(gradFuncBegin, gradFuncEnd);
    
    common_linear_problem_type commonLP;

    // Filling initial common linear problem.
    {
      commonLP.min = true;
      
      commonLP.c.assign(c);
      commonLP.cSign.resize(n);
      std::fill(commonLP.cSign.begin(), commonLP.cSign.end(), common_linear_problem_type::any_sign);
      
      matrix_type A = zero_matrix(2 * n, n);
      vector_type b(2 * n);
      for (size_t i = 0; i < n; ++i)
      {
        A(2 * i, i)     = -1;
        b(2 * i)        = -startLimitLo(i);
        A(2 * i + 1, i) = 1;
        b(2 * i + 1)    = startLimitHi(i);
      }
      
      commonLP.A.assign(A);
      commonLP.b.assign(A);
      commonLP.ASign.resize(2 * n);
      std::fill(commonLP.ASign.begin(), commonLP.ASign.end(), common_linear_problem_type::leq);
    }
    
    size_t nIterations(0);
    size_t const nMaxIterations(1000); // debug
    while (nIterations < nMaxIterations)
    {
      // Converting linear problem to canonical form.
      canonical_linear_problem_type canonicalLP;
      converter_template_type conv = linear_problem::to_canonical(commonLP, canonicalLP);
      
      // Solving linear problem.
      vector_type canonicalResultVec;
      simplex::simplex_result_type const result = 
          solve_augment(canonicalLP.A, canonicalLP.b, canonicalLP.c, canonicalResultVec);
      BOOST_ASSERT(result == simplex::srt_min_found); // FIXME: Handle other cases.
      
      vector_type const commonResult = conv(canonicalResultVec);
      
      // Adding new limits to common linear problem according to elements that satisfies g_i(x) > 0.
      bool isInside(true);
      for (size_t r = 0; r < n; ++r)
      {
        scalar_type const gr = g[r](commonResult);
        if (gr > 0)
        {
          // Adding new limit:
          // g[r](commonResult) + grad g[r](commonResult) * (x - commonResult) <= 0.
          // or 
          // grad g[r](commonResult) * x <= grad g[r](commonResult) * commonResult - g[r](commonResult).
          
          size_t const newRow = commonLP.b.size() + 1;
          BOOST_ASSERT(commonLP.ASign.size() == newRow - 1);
          BOOST_ASSERT(commonLP.A.size1() == newRow - 1);
          BOOST_ASSERT(commonLP.A.size2() == n);
          
          commonLP.b.resize(newRow);
          commonLP.A.resize(newRow, n);
          commonLP.ASign.resize(newRow, n);
          
          commonLP.ASign(newRow) = common_linear_problem_type::leq;
          
          vector_type const grGrad = gGrad[r](commonResult);
          BOOST_ASSERT(!scalar_traits_type::eq(norm_2(grGrad), 0)); // FIXME: I think this is possible case.
          row(commonLP.A, newRow) = grGrad;
          
          commonLP.b(newRow) = inner_prod(grGrad, commonResult) - gr;
          
          // Assert that builded limit cuts previosly founded minimum point.
          BOOST_ASSERT(inner_prod(row(commonLP.A, newRow), commonResult) > commonLP.b(newRow));
          
          BOOST_ASSERT(commonLP.valid());
        }
      }
      
      if (isInside)
      {
        // Found minimum of linear problem lies inside convex limits, so this is the answer.
        return commonResult;
      }
    }
    
    BOOST_ASSERT(0); // FIXME: Handle different cases.
    return vector_type(0);
  }
} // End of namespace 'kelley_cutting_plane'.
} // End of namespace 'numeric'.

#endif // NUMERIC_KELLEY_CUTTING_PLANE_HPP
