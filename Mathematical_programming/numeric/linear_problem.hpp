/*
 * linear_problem.hpp
 * Different forms of linear problems.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 13.04.2009
 */

#ifndef NUMERIC_LINEAR_PROBLEM_HPP
#define NUMERIC_LINEAR_PROBLEM_HPP

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

namespace numeric
{
namespace linear_problem
{
  // TODO: This is not so generic now :)
  
  template< class Scalar >
  struct canonical_linear_problem
  {
    typedef Scalar              scalar_type;
    typedef vector<scalar_type> vector_type;
    typedef matrix<scalar_type> matrix_type;
    
    vector_type c;
    matrix_type A;
    vector_type b;
    
    bool valid() const
    {
      // TODO: M.b. smth. with rank?
      return 
          (A.size1() > 0 && A.size2() > 0) &&
          (A.size1() == b.size()) &&
          (A.size2() == c.size());
    }
  };
  
  template< class Scalar >
  struct common_linear_problem
  {
    // TODO: Rename `eq' to `ineqality_eq' and so on.
    enum inequality_sign_type
    {
      leq = -1, // less or equal
      eq  =  0, // equal
      geq = +1, // greater or equal
    };

    enum variable_sign_type
    {
      geq_zero, // greater or equal zero
      any_sign, // any sign
    };
    
    typedef Scalar                       scalar_type;
    typedef vector<scalar_type>          vector_type;
    typedef matrix<scalar_type>          matrix_type;
    typedef vector<inequality_sign_type> inequality_signs_vector_type;
    typedef vector<inequality_sign_type> variables_signs_vector_type;
    
    bool                         min;   // is problem of searching minimum
    
    vector_type                  c;
    variables_signs_vector_type  cSign; // signs of variables
    
    matrix_type                  A;
    inequality_signs_vector_type ASign; // signs of limits
    
    vector_type                  b;
    
    bool valid() const
    {
      return 
          (A.size1() > 0 && A.size2() > 0) &&
          (A.size1() == b.size()) && (ASign.size() == b.size()) &&
          (A.size2() == c.size()) && (ASign.size() == c.size());
    }
  };
  
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
      for (size_t r = 0; r < outputSize_; ++r) // TODO: Use algorithm
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
  
  template< class S >
  inline
  typename converter_type<S>::type
    to_canonical( common_linear_problem   <S> const &commonLP, 
                  canonical_linear_problem<S>       &canonicalLP )
  {
    typedef S                                  scalar_type;
    typedef vector<scalar_type>                vector_type;
    typedef matrix<scalar_type>                matrix_type;
    typedef basic_range<size_t, long>          range_type;
    typedef zero_matrix<scalar_type>           zero_matrix_type;
    typedef zero_vector<scalar_type>           zero_vector_type;
    typedef common_linear_problem<scalar_type> common_linear_problem_type;
    typedef lambda_vector<scalar_type>         convertion_function_type;
    
    BOOST_ASSERT(commonLP.valid());
    
    // For details on transformations here see 
    //   Л.В. Петухов, Г.А. Серегин 
    //   "Методы решения задач выпуклого программирования", 
    //   учебное пособие, Ленинград 1991,
    //   pages 45-48.
    
    // Calculating how much additional variables will be needed.
    range_type const M(0, commonLP.A.size1());
    range_type const N(0, commonLP.A.size2());
    size_t const anySignVars = std::count(commonLP.cSign.begin(), commonLP.cSign.end(), common_linear_problem_type::any_sign);
    size_t const neALimits   = commonLP.ASign.size() - std::count(commonLP.ASign.begin(), commonLP.ASign.end(), common_linear_problem_type::eq);
    
    // Resizing canonical linear problem for result dimensions and filling with zeroes.
    canonicalLP.A.assign(zero_matrix_type(M.size(), N.size() + anySignVars + neALimits));
    canonicalLP.b.assign(zero_vector_type(M.size()));
    canonicalLP.c.assign(zero_vector_type(N.size() + anySignVars + neALimits));
    
    // Setting known part of result.
    submatrix(canonicalLP.A, M.begin(), M.end(), N.begin(), N.end()) = commonLP.A;
    canonicalLP.b                                                    = commonLP.b;
    subvector(canonicalLP.c, N.begin(), N.end())                     = commonLP.c;
    
    // Converting maximum search problem to minimum search problem.
    if (!commonLP.min)
      canonicalLP.c *= -1;
    
    // Converting less or equal limits to greater or equal limits.
    for (size_t r = 0; r < M.size(); ++r)
      if (commonLP.ASign(r) == common_linear_problem_type::leq)
      {
        row(commonLP.A, r) = -matrix_row_type(commonLP.A, r);
        commonLP.b(r)                  = -commonLP.b(r);
      }
    
    // Converting any sign variables to pair of non negative variables
    //   and filling convertion function.
    convertion_function_type conv(N.size() + anySignVars + neALimits, N.size());
    size_t nextAdditionalVar = N.size();
    for (size_t c = 0; c < N.size(); ++c)
    {
      if (commonLP.cSign(c) == common_linear_problem_type::geq_zero)
      {
        conv.setCoordinateFunction(c, boost::lambda::_1(c));
      }
      else if (commonLP.cSign(c) == common_linear_problem_type::any_sign)
      {
        canonicalLP.c(nextAdditionalVar) = -canonicalLP.c(c);
        column(canonicalLP.A, nextAdditionalVar) = -column(canonicalLP.A, c);
        
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
      if (commonLP.ASign(r) != common_linear_problem_type::eq)
      {
        // Found greater or equal limit (less or equal was converted to greater or equal before).
        canonicalLP.A(r, ++nextAdditionalVar) = -1;
      }
    }
    BOOST_ASSERT(nextAdditionalVar == N.size() + anySignVars + neALimits);
    
    BOOST_ASSERT(canonicalLP.valid());
    
    return conv;
  }
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_PROBLEM_HPP
