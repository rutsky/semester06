/*
 * linear_problem_algs.hpp
 * Algorithms on different forms of linear problems.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 17.05.2009
 */

#ifndef NUMERIC_LINEAR_PROBLEM_ALGS_HPP
#define NUMERIC_LINEAR_PROBLEM_ALGS_HPP

#include "numeric_common.hpp"

#include <algorithm>
#include <vector>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/lambda/lambda.hpp>

#include "linear_problem.hpp"
#include "subvector.hpp"
#include "simplex_alg.hpp"

namespace numeric
{
namespace linear_problem
{
  // Checks is provided solution of direct linear problem `x' and
  // solution of dual linear problem `y' satifies linear problem solution criteria.
  template< class V, class CLPTraits >
  bool is_solution( ICommonLinearProblem<CLPTraits> const &commonLP, 
                    V const &x, V const &y,
                    typename V::value_type eps )
  {
    // TODO: Not sure about correctness of function formulation.
    // TODO: Get default epsilon value from some traits.
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));
    
    typedef CLPTraits                             clp_traits_type;
    typedef typename clp_traits_type::scalar_type scalar_type;
    typedef vector<scalar_type>                   vector_type;
    typedef scalar_vector<scalar_type>            scalar_vector_type;
    typedef basic_range<size_t, long>             basic_range_type;
    
    basic_range_type M(0, constraints_count(commonLP)), N(0, variables_count(commonLP));
    
    std::vector<size_t> M1, M2, M3;
    select_constraints(commonLP, inequality_geq, std::back_inserter(M1));
    select_constraints(commonLP, inequality_eq,  std::back_inserter(M2));
    select_constraints(commonLP, inequality_leq, std::back_inserter(M3));
    BOOST_ASSERT(M1.size() + M2.size() + M3.size() == M.size());
    
    std::vector<size_t> N1, N2, N3;
    select_variables(commonLP, variable_geq_zero, std::back_inserter(N1));
    select_variables(commonLP, variable_any_sign, std::back_inserter(N2));
    select_variables(commonLP, variable_leq_zero, std::back_inserter(N3));
    BOOST_ASSERT(N1.size() + N2.size() + N3.size() == N.size());
    
    // TODO: Comment this.
    if (!by_component_greater_equal(subvector(y, M1.begin(), M1.end()), scalar_vector_type(M1.size(), 0), eps))
      return false;
    if (!by_component_less_equal(subvector(y, M3.begin(), M3.end()), scalar_vector_type(M3.size(), 0), eps))
      return false;
    
    if (!by_component_greater_equal(
          subvector(commonLP.c(), N1.begin(), N1.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())),
          scalar_vector_type(N1.size(), 0), eps))
      return false;
    if (!by_component_less_equal(
          subvector(commonLP.c(), N3.begin(), N3.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N3.begin(), N3.end())),
          scalar_vector_type(N3.size(), 0), eps))
      return false;
    
    if (!by_component_equal(
          subvector(commonLP.c(), N2.begin(), N2.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N2.begin(), N2.end())),
          scalar_vector_type(N2.size(), 0), eps))
      return false;
    
    if (!equal(prod(subvector(y, M1.begin(), M1.end()), 
                    prod(submatrix(commonLP.A(), M1.begin(), M1.end(), N.begin(), N.end()), x - 
                          subvector(commonLP.b(), M1.begin(), M1.end()))), 
               0, eps))
      return false;
    if (!equal(prod(subvector(y, M3.begin(), M3.end()), 
                    prod(submatrix(commonLP.A(), M3.begin(), M3.end(), N.begin(), N.end()), x - 
                          subvector(commonLP.b(), M3.begin(), M3.end()))), 
               0, eps))
      return false;
    
    if (!equal(prod(subvector(commonLP.c(), N1.begin(), N1.end()) - 
                    prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())), 
                    subvector(x, N1.begin(), N1.end())), 
               0, eps))
      return false;
    
    return true;
  }
  
  //
  // Utility functions.
  //
  
  template< class S, template< class > class CLPTraits >
  inline
  void construct_dual( ICommonLinearProblem<CLPTraits<S> > const  &commonLP, 
                       common_linear_problem<S, CLPTraits<S> >    &dualLP )
  {
    typedef S                                  scalar_type;
    typedef CLPTraits<S>                       clp_traits_type;
    
    typedef variable_sign_type   (*inequality_to_variable_function_type)( inequality_sign_type );
    typedef inequality_sign_type (*variable_to_inequality_function_type)( variable_sign_type   );
    
    BOOST_ASSERT(is_valid(commonLP));
    
    dualLP.min() = !commonLP.min();
    
    dualLP.c().assign(commonLP.b());
    dualLP.cSign().resize(constraints_count(commonLP));
    std::transform(commonLP.ASign().begin(), commonLP.ASign().end(), 
                   dualLP.cSign().begin(), (inequality_to_variable_function_type)&dual);
    
    dualLP.A().assign(trans(commonLP.A()));
    
    dualLP.ASign().resize(variables_count(commonLP));
    std::transform(commonLP.cSign().begin(), commonLP.cSign().end(), 
                   dualLP.ASign().begin(), (variable_to_inequality_function_type)&dual);
    
    dualLP.b().assign(commonLP.c());
    
    BOOST_ASSERT(is_valid(dualLP));
  }
  
  // TODO: Use placeholder from boost::mpl.
  template< class S >
  struct converter_type
  {
    typedef boost::function<vector<S>( vector<S> const & )> type;
  };
  
  template< class S >
  struct lambda_vector
  {
  public:
    typedef S                                                    scalar_type;
    typedef vector<scalar_type>                                  vector_type;
    typedef boost::function<scalar_type ( vector_type const & )> coordinate_function_type;
    typedef std::vector<coordinate_function_type>                coordinate_functions_type;
    
  public:
    lambda_vector( size_t inputSize, size_t outputSize )
      : inputSize_ (inputSize)
      , outputSize_(outputSize)
    {
      BOOST_ASSERT(inputSize > 0 && outputSize > 0);
      functions_.resize(outputSize);
      std::fill(functions_.begin(), functions_.end(), boost::lambda::_1 * 0);
    }
    
    vector_type operator()( vector_type const &v )
    {
      BOOST_ASSERT(v.size() == inputSize_);
      
      vector_type result(outputSize_);
      for (size_t r = 0; r < outputSize_; ++r) // TODO: Use algorithm.
        result[r] = functions_[r](v);
      
      return result;
    }
    
    void setCoordinateFunction( size_t r, coordinate_function_type function )
    {
      BOOST_ASSERT(r < functions_.size());
      functions_[r] = function;
    }
    
  private:    
    coordinate_functions_type functions_;

    size_t inputSize_;
    size_t outputSize_;
  };
  
  template< class S, template< class > class CLPTraits >
  inline
  typename converter_type<S>::type
    to_canonical( ICommonLinearProblem<CLPTraits<S> > const &commonLP, 
                  canonical_linear_problem<S>               &canonicalLP )
  {
    typedef S                                  scalar_type;
    typedef vector<scalar_type>                vector_type;
    typedef matrix<scalar_type>                matrix_type;
    typedef basic_range<size_t, long>          range_type;
    typedef zero_matrix<scalar_type>           zero_matrix_type;
    typedef zero_vector<scalar_type>           zero_vector_type;
    typedef common_linear_problem<scalar_type> common_linear_problem_type;
    typedef lambda_vector<scalar_type>         convertion_function_type;
    
    BOOST_ASSERT(is_valid(commonLP));
    
    // For details on transformations here see 
    //   Л.В. Петухов, Г.А. Серегин 
    //   "Методы решения задач выпуклого программирования", 
    //   учебное пособие, Ленинград 1991,
    //   pages 45-48.
    
    // Calculating how much additional variables will be needed.
    range_type const M(0, commonLP.A().size1());
    range_type const N(0, commonLP.A().size2());
    size_t const anySignVars = std::count(commonLP.cSign().begin(), commonLP.cSign().end(), common_linear_problem_type::variable_any_sign);
    size_t const neALimits   = commonLP.ASign().size() - std::count(commonLP.ASign().begin(), commonLP.ASign().end(), common_linear_problem_type::inequality_eq);
    
    // Resizing canonical linear problem for result dimensions and filling with zeroes.
    canonicalLP.A.assign(zero_matrix_type(M.size(), N.size() + anySignVars + neALimits));
    canonicalLP.b.assign(zero_vector_type(M.size()));
    canonicalLP.c.assign(zero_vector_type(N.size() + anySignVars + neALimits));
    
    // Setting known part of result.
    submatrix(canonicalLP.A(), M.begin(), M.end(), N.begin(), N.end()) = commonLP.A();
    canonicalLP.b()                                                    = commonLP.b();
    subvector(canonicalLP.c(), N.begin(), N.end())                     = commonLP.c();
    
    // Converting maximum search problem to minimum search problem.
    if (!commonLP.min())
      canonicalLP.c() *= -1;
    
    // Converting less or equal limits to greater or equal limits.
    for (size_t r = 0; r < M.size(); ++r)
      if (commonLP.ASign()(r) == common_linear_problem_type::inequality_leq)
      {
        row(commonLP.A(), r) = -matrix_row_type(commonLP.A(), r);
        commonLP.b()(r)      = -commonLP.b()(r);
      }
    
    // Converting any sign variables to pair of non negative variables
    //   and filling convertion function.
    convertion_function_type conv(N.size() + anySignVars + neALimits, N.size());
    size_t nextAdditionalVar = N.size();
    for (size_t c = 0; c < N.size(); ++c)
    {
      if (commonLP.cSign()(c) == common_linear_problem_type::variable_geq_zero)
      {
        conv.setCoordinateFunction(c, boost::lambda::_1(c));
      }
      else if (commonLP.cSign()(c) == common_linear_problem_type::variable_any_sign)
      {
        canonicalLP.c()(nextAdditionalVar) = -canonicalLP.c()(c);
        column(canonicalLP.A(), nextAdditionalVar) = -column(canonicalLP.A(), c);
        
        conv.setCoordinateFunction(c, boost::lambda::_1(c) - boost::lambda::_1(nextAdditionalVar));
        
        ++nextAdditionalVar;
      }
      else
      {
        // Impossible case.
        BOOST_ASSERT(0);
      }
    }
    BOOST_ASSERT(nextAdditionalVar == N.size() + anySignVars);
    
    // Converting greater or equal limits to equal limits.
    for (size_t r = 0; r < M.size(); ++r)
    {
      if (commonLP.ASign()(r) != common_linear_problem_type::inequality_eq)
      {
        // Found greater or equal limit (less or equal was converted to greater or equal before).
        canonicalLP.A()(r, ++nextAdditionalVar) = -1;
      }
    }
    BOOST_ASSERT(nextAdditionalVar == N.size() + anySignVars + neALimits);
    
    BOOST_ASSERT(is_valid(canonicalLP));
    
    return conv;
  }
  
  template< class V, class CLPTraits >
  simplex::simplex_result_type solve_by_simplex( ICommonLinearProblem<CLPTraits> const &commonLP, 
                                                 V &result )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));
    
    typedef CLPTraits                             clp_traits_type;
    typedef typename CLPTraits::scalar_type       scalar_type;
    typedef vector<scalar_type>                   vector_type;
    
    typedef common_linear_problem   <scalar_type, clp_traits_type> common_linear_problem_type;
    typedef canonical_linear_problem<scalar_type, clp_traits_type> canonical_linear_problem_type;
    typedef converter_type          <scalar_type>                  converter_type;

    // Converting linear problem to canonical form.
    canonical_linear_problem_type canonicalLP;
    converter_type conv = to_canonical(commonLP, canonicalLP);
    
    // Solving linear problem.
    vector_type canonicalResultVec;
    simplex::simplex_result_type const simplexResult = 
        simplex::solve_augment(canonicalLP.A(), canonicalLP.b(), canonicalLP.c(), canonicalResultVec);
    
    result.assign(conv(canonicalResultVec));
    
    return simplexResult;
  }
  
  template< class CLPTraits >
  bool check_linear_problem_solving_correctness( ICommonLinearProblem<CLPTraits> const &commonLP )
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
    
    // Checking corresponsibility of results.
    switch (directSimplexResult)
    {
    case simplex::srt_min_found:
      if (dualSimplexResult != simplex::srt_min_found)
      {
        BOOST_ASSERT(0); // debug
        return false;
      }
      break;
    case simplex::srt_not_limited:
      if (dualSimplexResult != simplex::srt_none)
      {
        BOOST_ASSERT(0); // debug
        return false;
      }
      break;
    case simplex::srt_none:
      if (dualSimplexResult != simplex::srt_not_limited)
      {
        BOOST_ASSERT(0); // debug
        return false;
      }
      break;
    case simplex::srt_loop:
      // TODO: Not implemented bahavior.
      BOOST_ASSERT(0); // debug
      return false;      
    default:
      // Impossible case.
      BOOST_ASSERT(0);
    }
    
    // Checking result values corresponsibility.
    scalar_type const eps = 1e-6; // TODO
    if (!is_solution(commonLP, directResultVec, dualResultVec, eps))
    {
      BOOST_ASSERT(0); // debug
      return false;
    }

    return true;
  }
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_PROBLEM_ALGS_HPP
