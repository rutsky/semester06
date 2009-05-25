/*
 * transportation_problem.hpp
 * Transportation problem definition.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 24.05.2009
 */

#ifndef NUMERIC_TRANSPORTATION_PROBLEM_HPP
#define NUMERIC_TRANSPORTATION_PROBLEM_HPP

#include "numeric_common.hpp"

#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include "potentials_alg.hpp"

namespace numeric
{
namespace transportation_problem
{
  //
  // Base transportation problem.
  //

  template< class Scalar >
  struct transportation_problem_traits
  {
    typedef Scalar                       scalar_type;
    typedef vector<scalar_type>          vector_type;
    typedef matrix<scalar_type>          matrix_type;
  };
  
  template< class Traits >
  struct ITransportationProblem
  {
    typedef Traits tp_traits_type;
    
    typedef typename tp_traits_type::scalar_type scalar_type;
    typedef typename tp_traits_type::vector_type vector_type;
    typedef typename tp_traits_type::matrix_type matrix_type;
    
    // TODO: May be rename members line `supplyVector', `demandVector' etc?
    
    // Supply.
    virtual vector_type const & a    () const = 0;

    // Demand.
    virtual vector_type const & b    () const = 0;
    
    // Costs matrix.
    virtual matrix_type const & C    () const = 0;
    
    virtual ~ITransportationProblem() {}
  };
  
  //
  // Base transportation problem type algorithms.
  //
  
  template< class TPTraits >
  inline
  bool is_valid( ITransportationProblem<TPTraits> const &tp )
  {
    return lp_potentials::is_tp_valid(tp.a(), tp.b(), tp.C());
  }
  
  template< class TPTraits >
  inline
  bool assert_valid( ITransportationProblem<TPTraits> const &tp )
  {
    return lp_potentials::assert_tp_valid(tp.a(), tp.b(), tp.C());
  }
  
  template< class TPTraits >
  inline
  bool is_closed( ITransportationProblem<TPTraits> const &tp )
  {
    return transportation_problem::is_closed(tp.a(), tp.b(), tp.C());
  }
  
  template< class TPTraits >
  inline
  size_t supplies_count( ITransportationProblem<TPTraits> const &tp )
  {
    BOOST_ASSERT(assert_valid(tp));
    return tp.a().size();
  }
  
  template< class TPTraits >
  inline
  size_t demands_count( ITransportationProblem<TPTraits> const &tp )
  {
    BOOST_ASSERT(assert_valid(tp));
    return tp.b().size();
  }
  
  //
  // Generic types definition.
  //

  template< class Scalar, class Traits = transportation_problem_traits<Scalar> >
  struct transportation_problem
    : public ITransportationProblem<Traits>
  {
    typedef ITransportationProblem<Traits> base_type;
    typedef Traits                         tp_traits_type;
    
    typedef typename base_type::scalar_type scalar_type;
    typedef typename base_type::vector_type vector_type;
    typedef typename base_type::matrix_type matrix_type;
    
    transportation_problem()
    {}
    
    
    template< class V1, class V2, class M >
    transportation_problem( vector_expression<V1> const &a, 
                            vector_expression<V2> const &b, 
                            matrix_expression<M>  const &C )
      : a_(a())
      , b_(b())
      , C_(C())
    {}
    
    template< class TPTraits >
    explicit transportation_problem( ITransportationProblem<TPTraits> const &other )
      : a_(other.a_)
      , b_(other.b_)
      , C_(other.C_)
    {}
    
    vector_type                  const & a    () const { return a_; }
    vector_type                        & a    ()       { return a_; }
    
    vector_type                  const & b    () const { return b_; }
    vector_type                        & b    ()       { return b_; }

    matrix_type                  const & C    () const { return C_; }
    matrix_type                        & C    ()       { return C_; }
    
  protected:
    vector_type a_, b_;
    matrix_type C_;
  };
  
  //
  // Solving algorithms.
  //
  
  template< class M, class TPTraits >
  void solve_by_potentials( ITransportationProblem<TPTraits> const &tp,
                            matrix_expression<M> &result )
  {
  }
} // End of namespace 'transportation_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_TRANSPORTATION_PROBLEM_HPP
