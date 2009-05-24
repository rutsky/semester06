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
    typedef typename TPTraits::scalar_type scalar_type;

    if (!(tp.C().size1() > 0 && tp.C().size2() > 0) &&
        (tp.C().size1() == tp.a().size()) &&
        (tp.C().size2() == tp.b().size()))
    {
      // Data sizes is not correspond.
      return false;
    }
    
    if (!(
        std::find_if(tp.a().begin(), tp.a().end(), 
                     boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.a().end() &&
        std::find_if(tp.b().begin(), tp.b().end(), 
                     boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.b().end() &&
        std::find_if(tp.C().data().begin(), tp.C().data().end(), 
                     boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.C().data().end()))
    {
      // One of data values is less than zero, which is incorrect.
      return false;
    }
    
    return true;
  }
  
  template< class TPTraits >
  inline
  bool assert_valid( ITransportationProblem<TPTraits> const &tp )
  {
    typedef typename TPTraits::scalar_type scalar_type;

    BOOST_ASSERT(tp.C().size1() > 0 && tp.C().size2() > 0);
    BOOST_ASSERT(tp.C().size1() == tp.a().size());
    BOOST_ASSERT(tp.C().size2() == tp.b().size());
    
    BOOST_ASSERT(std::find_if(tp.a().begin(), tp.a().end(), 
                              boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.a().end());
    BOOST_ASSERT(std::find_if(tp.b().begin(), tp.b().end(), 
                              boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.b().end());
    BOOST_ASSERT(std::find_if(tp.C().data().begin(), tp.C().data().end(), 
                              boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == tp.C().data().end());
    
    return is_valid(tp);
  }
  
  template< class TPTraits >
  inline
  bool is_closed( ITransportationProblem<TPTraits> const &tp )
  {
    typedef typename TPTraits::scalar_type scalar_type;
    typedef vector<scalar_type> vector_type;
    
    BOOST_ASSERT(assert_valid(tp));
    
    if (std::accumulate(tp.a().begin(), tp.a().end(), scalar_type(0.)) == 
        std::accumulate(tp.b().begin(), tp.b().end(), scalar_type(0.)))
    {
      // Sum of supplies equal to sum of demand, problem is closed.
      return true;
    }
    else
    {
      // Problem is unclosed.
      return false;
    }
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
} // End of namespace 'transportation_problem'.
} // End of namespace 'numeric'.

#endif // NUMERIC_TRANSPORTATION_PROBLEM_HPP
