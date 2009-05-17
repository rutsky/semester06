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
  //
  // Base types definition.
  //
  
  template< class Scalar >
  struct common_linear_problem_traits
  {
    enum inequality_sign_type
    {
      inequality_leq = -1, // less or equal
      inequality_eq  =  0, // equal
      inequality_geq = +1, // greater or equal
    };

    enum variable_sign_type
    {
      variable_geq_zero, // greater or equal zero
      variable_any_sign, // any sign
    };
    
    // TODO: Separate types.
    typedef Scalar                       scalar_type;
    typedef vector<scalar_type>          vector_type;
    typedef matrix<scalar_type>          matrix_type;
    typedef vector<inequality_sign_type> inequality_signs_vector_type;
    typedef vector<inequality_sign_type> variables_signs_vector_type;
  };
  
  template< class Traits >
  struct ICommonLinearProblem
  {
    typedef Traits clp_traits_type;
    
    typedef typename clp_traits_type::scalar_type scalar_type;
    typedef typename clp_traits_type::vector_type vector_type;
    typedef typename clp_traits_type::matrix_type matrix_type;
    typedef typename clp_traits_type::variables_signs_vector_type  variables_signs_vector_type;
    typedef typename clp_traits_type::inequality_signs_vector_type inequality_signs_vector_type;

    // Is problem of searching minimum.
    virtual bool                                 min  () const = 0;
    
    // Goal function coefficients.
    virtual vector_type                  const & c    () const = 0;
    // Signs of variables.
    virtual variables_signs_vector_type  const & cSign() const = 0;
    
    // Matrix of constaints.
    virtual matrix_type                  const & A    () const = 0;
    // Signs of constaints.
    virtual inequality_signs_vector_type const & ASign() const = 0;
    
    // Constraints absolute terms.
    virtual vector_type                  const & b    () const = 0;

    virtual ~ICommonLinearProblem() {}
  };
  
  template< class CLPTraits >
  bool is_valid( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    // TODO: Maybe smth. with rank?
    return 
        (commonLP.A().size1() > 0 && commonLP.A().size2() > 0) &&
        (commonLP.A().size1() == commonLP.b().size()) && (commonLP.ASign().size() == commonLP.b().size()) &&
        (commonLP.A().size2() == commonLP.c().size()) && (commonLP.ASign().size() == commonLP.c().size());
  }
  
  //
  // Generic types definition.
  //

  template< class Scalar >
  struct canonical_linear_problem
    : public ICommonLinearProblem<common_linear_problem_traits<Scalar> >
  {
    typedef ICommonLinearProblem<common_linear_problem_traits<Scalar> > base_type;
    typedef typename base_type::clp_traits_type                         clp_traits_type;
    
    typedef typename base_type::scalar_type scalar_type;
    typedef typename base_type::vector_type vector_type;
    typedef typename base_type::matrix_type matrix_type;
    typedef typename base_type::variables_signs_vector_type  variables_signs_vector_type;
    typedef typename base_type::inequality_signs_vector_type inequality_signs_vector_type;
    
    bool                                 min  () const { return true; }
    
    vector_type                  const & c    () const { return c_; }
    vector_type                        & c    ()       { return c_; }
    
    variables_signs_vector_type  const & cSign() const
    { 
      variables_signs_vector_type const result = 
        scalar_vector<typename clp_traits_type::variable_sign_type>(c_.size(), clp_traits_type::variable_geq_zero);
      BOOST_ASSERT(result.size() == c_.size());
      return result;
    }
    
    matrix_type                  const & A    () const { return A_; }
    matrix_type                        & A    ()       { return A_; }
    
    inequality_signs_vector_type const & ASign() const
    {
      inequality_signs_vector_type const result = 
        scalar_vector<typename clp_traits_type::inequality_sign_type>(A_.size2(), clp_traits_type::inequality_leq);
      BOOST_ASSERT(result.size() == A_.size2());
      return result;
    }
    
    vector_type                  const & b    () const { return b_; }
    vector_type                        & b    ()       { return b_; }
    
  protected:
    vector_type c_;
    matrix_type A_;
    vector_type b_;
  };
  
  template< class Scalar >
  struct common_linear_problem 
    : public ICommonLinearProblem<common_linear_problem_traits<Scalar> >
  {
    typedef ICommonLinearProblem<common_linear_problem_traits<Scalar> > base_type;
    typedef typename base_type::clp_traits_type                         clp_traits_type;
    
    typedef typename base_type::scalar_type scalar_type;
    typedef typename base_type::vector_type vector_type;
    typedef typename base_type::matrix_type matrix_type;
    typedef typename base_type::variables_signs_vector_type  variables_signs_vector_type;
    typedef typename base_type::inequality_signs_vector_type inequality_signs_vector_type;
    
    bool                         const & min  () const { return min_; }
    bool                               & min  ()       { return min_; }
    
    vector_type                  const & c    () const { return c_; }
    vector_type                        & c    ()       { return c_; }
    
    variables_signs_vector_type  const & cSign() const { return cSign_; }
    variables_signs_vector_type        & cSign()       { return cSign_; }
    
    matrix_type                  const & A    () const { return A_; }
    matrix_type                        & A    ()       { return A_; }
    
    inequality_signs_vector_type const & ASign() const { return ASign_; }
    inequality_signs_vector_type       & ASign()       { return ASign_; }
    
    vector_type                  const & b    () const { return b_; }
    vector_type                        & b    ()       { return b_; }
    
  protected:
    bool                         min_;
    vector_type                  c_;
    variables_signs_vector_type  cSign_;
    matrix_type                  A_;
    inequality_signs_vector_type ASign_;
    vector_type                  b_;
  };
  
  //
  // Utility functions.
  // 
  
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
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_PROBLEM_HPP
