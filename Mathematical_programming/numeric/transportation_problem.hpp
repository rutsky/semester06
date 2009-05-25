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
#include "linear_problem.hpp"
#include "lp_simplex.hpp"

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
    return lp_potentials::is_tp_closed(tp.a(), tp.b(), tp.C());
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
    typedef transportation_problem<Scalar, Traits> self_type;
    
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
    
    template< class S, template<class> class TPTraits >
    self_type & operator = ( ITransportationProblem<TPTraits<S> > const &other )
    {
      this->a() = other.a();
      this->b() = other.b();
      this->C() = other.C();
      
      return *this;
    }

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
  
  // TODO: Return converter.
  template< class S, template< class > class TPTraits >
  inline
  void to_closed( ITransportationProblem<TPTraits<S> > const &tp, 
                  transportation_problem<S, TPTraits<S> >    &closedTP )
  {
    typedef S                        scalar_type;
    typedef zero_vector<scalar_type> zero_vector_type;
    
    ASSERT(assert_valid(tp));
    
    closedTP = tp;
    size_t const m = supplies_count(tp), n = demands_count(tp);
    
    scalar_type const asum = std::accumulate(tp.a().begin(), tp.a().end(), scalar_type(0.));
    scalar_type const bsum = std::accumulate(tp.b().begin(), tp.b().end(), scalar_type(0.));
    
    if (asum < bsum)
    {
      // Supplies is not satisfies demand. Adding fake supply row.
      closedTP.a().resize(m + 1, true);
      closedTP.a()(m) = bsum - asum;
      
      closedTP.C().resize(m + 1, n, true);
      row(closedTP.C(), m) = zero_vector_type(n);
    }
    else if (asum > bsum)
    {
      // Too many supplies. Adding fake demand.
      closedTP.b().resize(n + 1, true);
      closedTP.b()(n) = asum - bsum;
      
      closedTP.C().resize(m, n + 1, true);
      column(closedTP.C(), n) = zero_vector_type(n);
    }
    else
    {
      // Transportation problem is closed.
    }
    
    ASSERT(is_closed(closedTP));
  }
  
  template< class M, class TPTraits >
  void solve_by_potentials( ITransportationProblem<TPTraits> const &closedTP,
                            matrix_expression<M> &result )
  {
    typedef typename TPTraits::scalar_type scalar_type;
    typedef transportation_problem<scalar_type, TPTraits> transportation_problem_type;
    
    ASSERT(is_closed(closedTP)); // TODO
    
    //transportation_problem_type closedTP;
    //to_closed(tp, closedTP);
    
    lp_potentials::solve(closedTP.a(), closedTP.b(), closedTP.C(), result);
  }
  
  // TODO: Return convertor of solutions.
  template< class TPTraits, class CLPTraits >
  void to_linear_problem( ITransportationProblem<TPTraits> const &tp,
                          linear_problem::canonical_linear_problem<typename CLPTraits::scalar_type, CLPTraits> &clp )
  {
    typedef typename CLPTraits::scalar_type scalar_type;
    typedef vector<scalar_type>             vector_type;
    typedef zero_vector<scalar_type>        zero_vector_type;
    typedef matrix<scalar_type>             matrix_type;
    typedef zero_matrix<scalar_type>        zero_matrix_type;
    
    ASSERT(is_closed(tp)); // TODO
    
    size_t const m = supplies_count(tp), n = demands_count(tp);
    
    clp.c() = zero_vector_type(m * n);
    // TODO: Use algorithms.
    for (size_t r = 0; r < m; ++r)
      for (size_t c = 0; c < n; ++c)
        clp.c()(r * n + c) = tp.C()(r, c);
    
    clp.A() = zero_matrix_type(m + n, m * n);
    clp.b() = zero_vector_type(m + n);
    
    for (size_t r = 0; r < m; ++r)
    {
      for (size_t c = 0; c < n; ++c)
        clp.A()(r, r * n + c) = 1;
      clp.b()(r) = tp.a()(r);
    }
    
    for (size_t c = 0; c < n; ++c)
    {
      for (size_t r = 0; r < m; ++r)
        clp.A()(m + c, r * n + c) = 1;
      clp.b()(m + c) = tp.b()(c);
    }
    
    clp.update();
    
    BOOST_ASSERT(linear_problem::is_valid(clp));
  }
  
  template< class TPTraits, class M >
  typename TPTraits::scalar_type transportation_cost( ITransportationProblem<TPTraits> const &tp,
                                                      matrix_expression<M> const &X )
  {
    return lp_potentials::transportationCost(tp.C(), X());
  }
  
  template< class M, class TPTraits >
  void solve_by_simplex( ITransportationProblem<TPTraits> const &closedTP,
                         matrix_expression<M> &result )
  {
    typedef typename TPTraits::scalar_type scalar_type;
    typedef vector<scalar_type>            vector_type;
    typedef matrix<scalar_type>            matrix_type;
    typedef transportation_problem<scalar_type, TPTraits> transportation_problem_type;
    typedef linear_problem::canonical_linear_problem<scalar_type> canonical_linear_problem_type;
    
    ASSERT(is_closed(closedTP)); // TODO
    
    size_t const m = supplies_count(closedTP), n = demands_count(closedTP);

    canonical_linear_problem_type clp;
    to_linear_problem(closedTP, clp);

    vector_type resultV;
    simplex::simplex_result_type const resultState = solve_by_simplex(clp, resultV);
    ASSERT_EQ(resultState, simplex::srt_min_found);
    ASSERT_EQ(resultV.size(), m * n);
    
    result().resize(m, n);
    for (size_t r = 0; r < m; ++r)
      for (size_t c = 0; c < n; ++c)
        result()(r, c) = resultV(r * n + c);
  }
} // End of namespace 'transportation_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_TRANSPORTATION_PROBLEM_HPP
