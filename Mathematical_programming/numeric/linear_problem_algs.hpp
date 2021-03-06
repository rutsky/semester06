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
#include <boost/lambda/bind.hpp>

#include "linear_problem.hpp"
#include "vector_ops.hpp"
#include "simplex_alg.hpp"

namespace numeric
{
namespace linear_problem
{
  // Checks is provided solution of direct linear problem `x' and
  // solution of dual linear problem `y' satifies linear problem solution criteria.
  template< class E1, class E2, class CLPTraits >
  bool is_solution( ICommonLinearProblem<CLPTraits> const &commonLP, 
                    vector_expression<E1> const &x, vector_expression<E2> const &y )
  {
    // TODO: Not sure about correctness of function formulation.
    
    typedef CLPTraits                             clp_traits_type;
    typedef typename clp_traits_type::scalar_type scalar_type;
    typedef vector<scalar_type>                   vector_type;
    typedef scalar_vector<scalar_type>            scalar_vector_type;
    typedef basic_range<size_t, long>             basic_range_type;
    
    // TODO: Get default epsilon value from some traits.
    scalar_type const eps = epsilon<scalar_type>();
    
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
    if (!M1.empty() && !by_component_greater_equal(subvector(y(), M1.begin(), M1.end()), scalar_vector_type(M1.size(), 0.), eps))
      return false;
    if (!M3.empty() && !by_component_less_equal(subvector(y(), M3.begin(), M3.end()), scalar_vector_type(M3.size(), 0.), eps))
      return false;
    
    if (!N1.empty() && !by_component_greater_equal(
          subvector(commonLP.c(), N1.begin(), N1.end()) - prod(y(), submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())),
          scalar_vector_type(N1.size(), 0.), eps))
      return false;
    if (!N3.empty() && !by_component_less_equal(
          subvector(commonLP.c(), N3.begin(), N3.end()) - prod(y(), submatrix(commonLP.A(), M.begin(), M.end(), N3.begin(), N3.end())),
          scalar_vector_type(N3.size(), 0.), eps))
      return false;
    
    if (!N2.empty() && !by_component_equal(
          subvector(commonLP.c(), N2.begin(), N2.end()) - prod(y(), submatrix(commonLP.A(), M.begin(), M.end(), N2.begin(), N2.end())),
          scalar_vector_type(N2.size(), 0.), eps))
      return false;
    
    if (!M1.empty() && !equal(inner_prod(vector_type(subvector(y(), M1.begin(), M1.end())), 
                                         vector_type(prod(submatrix(commonLP.A(), M1.begin(), M1.end(), N.begin(), N.end()), x()) - 
                                                     subvector(commonLP.b(), M1.begin(), M1.end()))), 
                              0., eps))
      return false;
    if (!M3.empty() && !equal(inner_prod(vector_type(subvector(y(), M3.begin(), M3.end())), 
                                         vector_type(prod(submatrix(commonLP.A(), M3.begin(), M3.end(), N.begin(), N.end()), x()) - 
                                                     subvector(commonLP.b(), M3.begin(), M3.end()))), 
                              0., eps))
      return false;
    
    if (!N1.empty() && !equal(inner_prod(vector_type(subvector(commonLP.c(), N1.begin(), N1.end()) - 
                                                     vector_type(prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())))), 
                                         vector_type(subvector(x(), N1.begin(), N1.end()))), 
                              0., eps))
      return false;
    
    return true;
  }
  
  // Asserts step by step that provided values are correct as in `is_solution' function.
  // This function is more for debug purposes.
  template< class V, class CLPTraits >
  bool assert_is_solution( ICommonLinearProblem<CLPTraits> const &commonLP, 
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
    if (!M1.empty() && !by_component_greater_equal(subvector(y, M1.begin(), M1.end()), scalar_vector_type(M1.size(), 0.), eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    if (!M3.empty() && !by_component_less_equal(subvector(y, M3.begin(), M3.end()), scalar_vector_type(M3.size(), 0.), eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    
    if (!N1.empty() && !by_component_greater_equal(
          subvector(commonLP.c(), N1.begin(), N1.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())),
          scalar_vector_type(N1.size(), 0.), eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    if (!N3.empty() && !by_component_less_equal(
          subvector(commonLP.c(), N3.begin(), N3.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N3.begin(), N3.end())),
          scalar_vector_type(N3.size(), 0.), eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    
    if (!N2.empty() && !by_component_equal(
          subvector(commonLP.c(), N2.begin(), N2.end()) - prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N2.begin(), N2.end())),
          scalar_vector_type(N2.size(), 0.), eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    
    if (!M1.empty() && !equal(inner_prod(vector_type(subvector(y, M1.begin(), M1.end())), 
                                         vector_type(prod(submatrix(commonLP.A(), M1.begin(), M1.end(), N.begin(), N.end()), x) - 
                                                     subvector(commonLP.b(), M1.begin(), M1.end()))), 
                              0., eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    if (!M3.empty() && !equal(inner_prod(vector_type(subvector(y, M3.begin(), M3.end())), 
                                         vector_type(prod(submatrix(commonLP.A(), M3.begin(), M3.end(), N.begin(), N.end()), x) - 
                                                     subvector(commonLP.b(), M3.begin(), M3.end()))), 
                              0., eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    
    if (!N1.empty() && !equal(inner_prod(vector_type(subvector(commonLP.c(), N1.begin(), N1.end()) - 
                                                     vector_type(prod(y, submatrix(commonLP.A(), M.begin(), M.end(), N1.begin(), N1.end())))), 
                                         vector_type(subvector(x, N1.begin(), N1.end()))), 
                              0., eps))
    {
      BOOST_ASSERT(0);
      return false;
    }
    
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
    
    typedef variable_sign_type   (*inequality_to_variable_function_type)( bool, inequality_sign_type );
    typedef inequality_sign_type (*variable_to_inequality_function_type)( bool, variable_sign_type   );
    
    BOOST_ASSERT(is_valid(commonLP));
    
    dualLP.min() = !commonLP.min();
    
    dualLP.c() = commonLP.b();
    dualLP.cSign().resize(constraints_count(commonLP));
    std::transform(commonLP.ASign().begin(), commonLP.ASign().end(), 
                   dualLP.cSign().begin(), 
                   boost::bind((inequality_to_variable_function_type)&dual, commonLP.min(), _1));
    
    dualLP.A() = trans(commonLP.A());
    
    dualLP.ASign().resize(variables_count(commonLP));
    std::transform(commonLP.cSign().begin(), commonLP.cSign().end(), 
                   dualLP.ASign().begin(), 
                   boost::bind((variable_to_inequality_function_type)&dual, commonLP.min(), _1));
    
    dualLP.b() = commonLP.c();
    
    BOOST_ASSERT(is_valid(dualLP));
  }
  
  // TODO: Use placeholder from boost::mpl.
  template< class S >
  struct converter_template_type
  {
    typedef boost::function<vector<S>( vector<S> const & )> type;
  };
  
  template< class S >
  struct lambda_vector
  {
  public:
    typedef S                                                    scalar_type;
    typedef vector<scalar_type>                                  vector_type;
    
  public:
    lambda_vector( size_t inputSize, size_t outputSize )
      : inputSize_ (inputSize)
      , outputSize_(outputSize)
    {
      BOOST_ASSERT(inputSize > 0 && outputSize > 0);
      differencies_.resize(outputSize);
      scales_.resize(outputSize);
    }
    
    vector_type operator()( vector_type const &v )
    {
      BOOST_ASSERT(v.size() == inputSize_);
      
      vector_type result(outputSize_);
      for (size_t r = 0; r < outputSize_; ++r) // TODO: Use algorithm.
      {
        if (differencies_[r])
        {
          result[r] = v(differencies_[r]->first) - v(differencies_[r]->second);
        }
        else
        {
          BOOST_ASSERT(v.size() > r); // Fail!
          result[r] = v(r);
        }
        
        if (scales_[r])
          result[r] *= scales_[r].get();
      }
      
      return result;
    }
    
    // output(rOutput) = input(rMinuend) - input(rSubtrahend).
    void setCoordinateFunction( size_t rOutput, size_t rMinuend, size_t rSubtrahend )
    {
      BOOST_ASSERT(rOutput < differencies_.size());
      BOOST_ASSERT(rMinuend < inputSize_);
      BOOST_ASSERT(rSubtrahend < inputSize_);
      
      if (rMinuend != rSubtrahend)
        differencies_[rOutput] = std::make_pair(rMinuend, rSubtrahend);
      else
        differencies_[rOutput].reset();
    }
    
    // output(r) = -input*(r).
    // Applies to result of other convetion function.
    void setScaleCoordinateFunction( size_t rOutput, scalar_type scale )
    {
      BOOST_ASSERT(rOutput < scales_.size());
      scales_[rOutput] = scale;
    }
    
  private:
    // TODO: Use boost::lambda.
    std::vector<boost::optional<std::pair<size_t, size_t> > > differencies_;
    std::vector<boost::optional<scalar_type> >                scales_;

    size_t inputSize_;
    size_t outputSize_;
  };
  
  template< class S, template< class > class CLPTraits >
  inline
  typename converter_template_type<S>::type
    to_canonical( ICommonLinearProblem<CLPTraits<S> > const  &commonLP, 
                  canonical_linear_problem<S, CLPTraits<S> > &canonicalLP )
  {
    typedef S                                  scalar_type;
    typedef CLPTraits<S>                       clp_traits_type;
    typedef vector<scalar_type>                vector_type;
    typedef matrix<scalar_type>                matrix_type;
    typedef basic_range<size_t, long>          range_type;
    typedef zero_matrix<scalar_type>           zero_matrix_type;
    typedef zero_vector<scalar_type>           zero_vector_type;
    typedef common_linear_problem<scalar_type> common_linear_problem_type;
    typedef lambda_vector<scalar_type>         convertion_function_type;
    
    typedef canonical_linear_problem<scalar_type> canonical_linear_problem_type;
    
    BOOST_ASSERT(is_valid(commonLP));
    
    // For details on transformations here see 
    //   Л.В. Петухов, Г.А. Серегин 
    //   "Методы решения задач выпуклого программирования", 
    //   учебное пособие, Ленинград 1991,
    //   pages 45-48.
    
    // Calculating how much additional variables will be needed.
    range_type const M(0, commonLP.A().size1());
    range_type const N(0, commonLP.A().size2());
    size_t const anySignVars = std::count(commonLP.cSign().begin(), commonLP.cSign().end(), variable_any_sign);
    size_t const neAConstraints   = commonLP.ASign().size() - std::count(commonLP.ASign().begin(), commonLP.ASign().end(), inequality_eq);
    
    // Creating canonical linear problem for result dimensions and filling with zeroes.
    matrix_type canonicalLPA(zero_matrix_type(M.size(), N.size() + anySignVars + neAConstraints));
    vector_type canonicalLPb(zero_vector_type(M.size()));
    vector_type canonicalLPc(zero_vector_type(N.size() + anySignVars + neAConstraints));
    
    // Setting known part of result.
    submatrix(canonicalLPA, M.begin(), M.end(), N.begin(), N.end()) = commonLP.A();
    canonicalLPb                                                    = commonLP.b();
    subvector(canonicalLPc, N.begin(), N.end())                     = commonLP.c();
    
    // Converting maximum search problem to minimum search problem.
    if (!commonLP.min())
      canonicalLPc = -canonicalLPc;
    
    // Converting less or equal constraints to greater or equal constraints.
    for (size_t r = 0; r < M.size(); ++r)
      if (commonLP.ASign()(r) == inequality_leq)
      {
        row(canonicalLPA, r) = -row(canonicalLPA, r);
        canonicalLPb(r)      = -canonicalLPb(r);
      }

    // Starting filling convertion function.
    convertion_function_type conv(N.size() + anySignVars + neAConstraints, N.size());
    
    // Converting less or equal zero variables to greater or equal zero variables.
    for (size_t c = 0; c < N.size(); ++c)
      if (commonLP.cSign()(c) == variable_leq_zero)
      {
        column(canonicalLPA, c) = -column(canonicalLPA, c);
        canonicalLPc(c)         = -canonicalLPc(c);
        conv.setScaleCoordinateFunction(c, scalar_type(-1.));
      }
    
    // Converting any sign variables to pair of non negative variables
    //   and filling convertion function.
    size_t nextAdditionalVar = N.size();
    for (size_t c = 0; c < N.size(); ++c)
    {
      if (commonLP.cSign()(c) == variable_geq_zero || commonLP.cSign()(c) == variable_leq_zero)
      {
        // Note: don't forget about convertion above.
        // Identity convertion. Don't moduify anything.
        // conv.setCoordinateFunction(c, c, c);
      }
      else if (commonLP.cSign()(c) == variable_any_sign)
      {
        canonicalLPc(nextAdditionalVar) = -canonicalLPc(c);
        column(canonicalLPA, nextAdditionalVar) = -column(canonicalLPA, c);
        conv.setCoordinateFunction(c, c, nextAdditionalVar);
        
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
      if (commonLP.ASign()(r) != inequality_eq)
      {
        // Found greater or equal limit (less or equal was converted to greater or equal above).
        canonicalLPA(r, nextAdditionalVar++) = -1;
      }
    }
    BOOST_ASSERT(nextAdditionalVar == N.size() + anySignVars + neAConstraints);
    
    canonicalLP = canonical_linear_problem_type(canonicalLPc, canonicalLPA, canonicalLPb);
    
    BOOST_ASSERT(assert_valid(canonicalLP));
    
    return conv;
  }
  
  // Returns true if constraints consistent, false otherwise.
  template< class S, template< class > class CLPTraits >
  bool remove_dependent_constraints( canonical_linear_problem<S, CLPTraits<S> > const &canonicalLP,
                                     canonical_linear_problem<S, CLPTraits<S> >       &liCanonicalLP )
  {
    liCanonicalLP.c() = canonicalLP.c();
    bool const result = simplex::remove_dependent_constraints(canonicalLP.A(), canonicalLP.b(), liCanonicalLP.A(), liCanonicalLP.b());
    liCanonicalLP.update();
    return result;
  }
  
  bool is_result_status_correct( simplex::simplex_result_type direct, simplex::simplex_result_type dual )
  {
    switch (direct)
    {
    case simplex::srt_min_found:
      if (dual != simplex::srt_min_found)
        return false;
      return true;
    case simplex::srt_not_limited:
      if (dual != simplex::srt_none)
        return false;
      return true;
    case simplex::srt_none:
      if (dual != simplex::srt_not_limited)
        return false;
      return true;
    case simplex::srt_loop:
      // TODO: Not implemented behavior.
      BOOST_ASSERT(0); // debug
      return true;
    default:
      // Impossible case.
      BOOST_ASSERT(0);
      return false;
    }
  }
  
  template< class CLPTraits, class E1, class E2 >
  bool is_lp_solution_correct( ICommonLinearProblem<CLPTraits> const &commonLP,
                               simplex::simplex_result_type directResult, vector_expression<E1> const &x,
                               simplex::simplex_result_type dualResult,   vector_expression<E2> const &y )
  {
    typedef CLPTraits                             clp_traits_type;
    typedef typename CLPTraits::scalar_type       scalar_type;
    typedef vector<scalar_type>                   vector_type;
    
    typedef common_linear_problem<scalar_type, clp_traits_type> common_linear_problem_type;
    
    bool failed = false;
    
    // Checking corresponsibility of results.
    if (!is_result_status_correct(directResult, dualResult))
      failed = true;
    
    // Checking result values corresponsibility.
    if (!failed && !is_solution(commonLP, x, y))
      failed = true;
    
    if (failed)
    {
      // debug
      std::cout << ">>> Solution is incorrect! <<<\n";
      std::cout << "      problem:\n";
      output_common_linear_problem(std::cout, commonLP);
      std::cout << "      direct solution: " << x << "\n";
      std::cout << "      dual   solution: " << y << "\n";
      
      {
        std::ofstream directDump("__problem.m");
        output_as_octave_source(directDump, commonLP);
      }
      // end of debug
      
      return false;
    }

    return true;
  }
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_PROBLEM_ALGS_HPP
