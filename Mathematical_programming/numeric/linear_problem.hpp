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
#include <functional>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace numeric
{
namespace linear_problem
{
  //
  // Base common linear problem type definition.
  //
  
  // Common linear problem constraint inequality sign type.
  enum inequality_sign_type
  {
    // TODO: Rename `inequality' to `constraint'.
    inequality_leq = -1, // less or equal
    inequality_eq  =  0, // equal
    inequality_geq = +1, // greater or equal
  };

  // Common linear problem goal function variable sign type.
  enum variable_sign_type
  {
    variable_leq_zero = -1, // less or equal zero
    variable_any_sign =  0, // any sign
    variable_geq_zero = +1, // greater or equal zero
  };
  
  inline
  variable_sign_type dual( bool directProblemMin, inequality_sign_type inequalitySign )
  {
    // TODO: Can be optimized using direct values of enumerations.
    switch(inequalitySign)
    {
    case inequality_geq:
      if (directProblemMin)
        return variable_geq_zero;
      else
        return variable_leq_zero;
    case inequality_eq:
      return variable_any_sign;
    case inequality_leq:
      if (directProblemMin)
        return variable_leq_zero;
      else
        return variable_geq_zero;
    default:
      // Impossible case.
      BOOST_ASSERT(0);
    }
  }
  
  inline
  inequality_sign_type dual( bool directProblemMin, variable_sign_type variableSign )
  {
    // TODO: Can be optimized using direct values of enumerations.
    switch(variableSign)
    {
    case variable_geq_zero:
      if (directProblemMin)
        return inequality_leq;
      else
        return inequality_geq;
    case variable_any_sign:
      return inequality_eq;
    case variable_leq_zero:
      if (directProblemMin)
        return inequality_geq;
      else
        return inequality_leq;
    default:
      // Impossible case.
      BOOST_ASSERT(0);
    }
  }

  template< class Scalar >
  struct common_linear_problem_traits
  {
    // TODO: Think about admissible inequation signs. 
    //   They should correspond to variable signs types through duality.
    
    // TODO: Separate types.
    typedef Scalar                       scalar_type;
    typedef vector<scalar_type>          vector_type;
    typedef matrix<scalar_type>          matrix_type;
    typedef vector<inequality_sign_type> inequality_signs_vector_type;
    typedef vector<variable_sign_type>   variables_signs_vector_type;
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
    
    // TODO: May be name members line `goalVector', `variablesSigns', `constraintsMatrix' etc?

    // Is problem of searching minimum.
    virtual bool                                 min  () const = 0;
    
    // Goal function coefficients.
    virtual vector_type                  const & c    () const = 0;
    // Signs of variables.
    virtual variables_signs_vector_type  const & cSign() const = 0;
    
    // Matrix of constaints coefficients.
    virtual matrix_type                  const & A    () const = 0;
    // Signs of constaints.
    // TODO: Must be in plural form.
    virtual inequality_signs_vector_type const & ASign() const = 0;
    
    // Constraints constant terms.
    virtual vector_type                  const & b    () const = 0;

    virtual ~ICommonLinearProblem() {}
  };
  
  //
  // Base common linear problem type algorithms.
  //
  
  template< class CLPTraits >
  inline
  bool is_valid( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    // TODO: Maybe smth. with rank?
    return 
        (commonLP.A().size1() > 0 && commonLP.A().size2() > 0) &&
        (commonLP.A().size1() == commonLP.b().size()) && (commonLP.ASign().size() == commonLP.b().size()) &&
        (commonLP.A().size2() == commonLP.c().size()) && (commonLP.cSign().size() == commonLP.c().size());
  }
  
  template< class CLPTraits >
  inline
  bool assert_valid( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    // TODO: Maybe smth. with rank?
    BOOST_ASSERT(commonLP.A().size1() > 0);
    BOOST_ASSERT(commonLP.A().size2() > 0);
    BOOST_ASSERT(commonLP.A().size1() == commonLP.b().size());
    BOOST_ASSERT(commonLP.ASign().size() == commonLP.b().size());
    BOOST_ASSERT(commonLP.A().size2() == commonLP.c().size());
    BOOST_ASSERT(commonLP.cSign().size() == commonLP.c().size());
    
    return is_valid(commonLP);
  }
  
  template< class CLPTraits >
  inline
  size_t variables_count( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    BOOST_ASSERT(is_valid(commonLP));
    return commonLP.A().size2();
  }
  
  template< class CLPTraits >
  inline
  size_t constraints_count( ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    BOOST_ASSERT(is_valid(commonLP));
    return commonLP.A().size1();
  }
  
  template< class CLPTraits, class IndexesOut >
  inline
  void select_constraints( ICommonLinearProblem<CLPTraits> const &commonLP, 
                           inequality_sign_type inequalitySign,
                           IndexesOut idxsOut )
  {
    BOOST_CONCEPT_ASSERT((boost::OutputIterator<IndexesOut, size_t>));
    BOOST_ASSERT(is_valid(commonLP));
    
    typedef CLPTraits                             clp_traits_type;
    typedef typename clp_traits_type::scalar_type scalar_type;
    typedef basic_range<size_t, long>             range_type;
    
    range_type M(0, constraints_count(commonLP));
    copy_if(M.begin(), M.end(), idxsOut, 
        boost::bind<bool>(std::equal_to<inequality_sign_type>(), inequalitySign, boost::bind<inequality_sign_type>(commonLP.ASign(), _1)));
  }
  
  template< class CLPTraits, class IndexesOut >
  inline
  void select_variables( ICommonLinearProblem<CLPTraits> const &commonLP, 
                         variable_sign_type variableSign,
                         IndexesOut idxsOut )
  {
    BOOST_CONCEPT_ASSERT((boost::OutputIterator<IndexesOut, size_t>));
    BOOST_ASSERT(is_valid(commonLP));
    
    typedef CLPTraits                             clp_traits_type;
    typedef typename clp_traits_type::scalar_type scalar_type;
    typedef basic_range<size_t, long>             range_type;
    
    range_type N(0, variables_count(commonLP));
    copy_if(N.begin(), N.end(), idxsOut, 
        boost::bind<bool>(std::equal_to<variable_sign_type>(), variableSign, boost::bind<variable_sign_type>(commonLP.cSign(), _1)));
  }
  
  //
  // Generic types definition.
  //

  template< class Scalar, class Traits = common_linear_problem_traits<Scalar> >
  struct canonical_linear_problem
    : public ICommonLinearProblem<Traits>
  {
    typedef ICommonLinearProblem<Traits> base_type;
    typedef Traits                       clp_traits_type;
    
    typedef typename base_type::scalar_type scalar_type;
    typedef typename base_type::vector_type vector_type;
    typedef typename base_type::matrix_type matrix_type;
    typedef typename base_type::variables_signs_vector_type  variables_signs_vector_type;
    typedef typename base_type::inequality_signs_vector_type inequality_signs_vector_type;
    
    canonical_linear_problem()
    {}
    
    
    // TODO!
    /*
    canonical_linear_problem( vector_expression<scalar_type> const &c, 
                              matrix_expression<scalar_type> const &A, 
                              vector_expression<scalar_type> const &b )
      : c_(c)
      , A_(A)
      , b_(b)
    {
      init();
    }
    */
    canonical_linear_problem( vector_type const &c, 
                              matrix_type const &A, 
                              vector_type const &b )
      : c_(c)
      , A_(A)
      , b_(b)
    {
      update();
    }
    
    explicit canonical_linear_problem( canonical_linear_problem const &other )
      : c_(other.c_)
      , A_(other.A_)
      , b_(other.b_)
    {
      update();
    }
    
    bool                                 min  () const { return true; }
    
    vector_type                  const & c    () const { return c_; }
    vector_type                        & c    ()       { return c_; }
    
    variables_signs_vector_type  const & cSign() const { return cSign_; }
    
    matrix_type                  const & A    () const { return A_; }
    matrix_type                        & A    ()       { return A_; }
    
    inequality_signs_vector_type const & ASign() const { return ASign_; }
    
    vector_type                  const & b    () const { return b_; }
    vector_type                        & b    ()       { return b_; }
    
    void update() const
    {
      cSign_ = scalar_vector<variable_sign_type>(c_.size(), variable_geq_zero);
      BOOST_ASSERT(cSign_.size() == c_.size());
      
      ASign_ = scalar_vector<inequality_sign_type>(A_.size1(), inequality_eq);
      BOOST_ASSERT(ASign_.size() == A_.size1());
      
      BOOST_ASSERT(assert_valid(*this));
    }
    
  protected:
    vector_type c_;
    matrix_type A_;
    vector_type b_;
    
    // TODO!
    mutable variables_signs_vector_type  cSign_;
    mutable inequality_signs_vector_type ASign_;
  };
  
  template< class Scalar, class Traits = common_linear_problem_traits<Scalar> >
  struct common_linear_problem 
    : public ICommonLinearProblem<Traits>
  {
    typedef ICommonLinearProblem<Traits> base_type;
    typedef Traits                       clp_traits_type;
    
    typedef typename base_type::scalar_type scalar_type;
    typedef typename base_type::vector_type vector_type;
    typedef typename base_type::matrix_type matrix_type;
    typedef typename base_type::variables_signs_vector_type  variables_signs_vector_type;
    typedef typename base_type::inequality_signs_vector_type inequality_signs_vector_type;
    
    bool                                 min  () const { return min_; }
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
} // End of namespace 'linear_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_LINEAR_PROBLEM_HPP
