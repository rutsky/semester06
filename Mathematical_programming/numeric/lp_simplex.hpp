/*
 * lp_simplex.hpp
 * Solving linear programming problem by simplex algorithm wrapper.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 22.05.2009
 */

#ifndef NUMERIC_LP_SIMPLEX_HPP
#define NUMERIC_LP_SIMPLEX_HPP

#include "numeric_common.hpp"

#include <algorithm>
#include <vector>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include "linear_problem.hpp"
#include "linear_problem_algs.hpp"
#include "simplex_alg.hpp"

namespace numeric
{
namespace linear_problem
{
  template< class E, class CLPTraits >
  simplex::simplex_result_type solve_by_simplex( ICommonLinearProblem<CLPTraits> const &commonLP, 
                                                 vector_expression<E> &result )
  {
    typedef CLPTraits                             clp_traits_type;
    typedef typename CLPTraits::scalar_type       scalar_type;
    typedef vector<scalar_type>                   vector_type;
    
    typedef common_linear_problem   <scalar_type, clp_traits_type> common_linear_problem_type;
    typedef canonical_linear_problem<scalar_type, clp_traits_type> canonical_linear_problem_type;
    typedef typename converter_template_type <scalar_type>::type   converter_type;

    // Converting linear problem to canonical form.
    canonical_linear_problem_type canonicalLP;
    converter_type conv = to_canonical(commonLP, canonicalLP);
    
    // debug
    //output_common_linear_problem(std::cout, commonLP);
    //output_common_linear_problem(std::cout, canonicalLP);
    // end of debug
    
    // Solving linear problem.
    vector_type canonicalResultVec;
    simplex::simplex_result_type const simplexResult = 
        simplex::solve_augment(canonicalLP.A(), canonicalLP.b(), canonicalLP.c(), canonicalResultVec);
    
    if (simplexResult == simplex::srt_min_found)
      result() = conv(canonicalResultVec);
    
    return simplexResult;
  }
  
  template< class CLPTraits >
  bool is_simplex_solving_correct( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    typedef CLPTraits                             clp_traits_type;
    typedef typename CLPTraits::scalar_type       scalar_type;
    typedef vector<scalar_type>                   vector_type;
    
    typedef common_linear_problem<scalar_type, clp_traits_type> common_linear_problem_type;
    
    // Solving direct linear problem.
    vector_type directResultVec;
    simplex::simplex_result_type const directSimplexResult = solve_by_simplex(commonLP, directResultVec);
    BOOST_ASSERT(directSimplexResult != simplex::srt_loop); // TODO: Not implemented bahavior.
    
    // Constructing dual linear problem.
    common_linear_problem_type dualLP;
    construct_dual(commonLP, dualLP);
    
    // Solving dual linear problem.
    vector_type dualResultVec;
    simplex::simplex_result_type const dualSimplexResult = solve_by_simplex(dualLP, dualResultVec);
    BOOST_ASSERT(dualSimplexResult != simplex::srt_loop); // TODO: Not implemented bahavior.
    
    if (!is_lp_solution_correct(commonLP, 
                                directSimplexResult, directResultVec, 
                                dualSimplexResult, dualResultVec))
    {
      return false;
    }

    return true;
  }
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LP_SIMPLEX_HPP
