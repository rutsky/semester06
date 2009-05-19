/*
 * function.hpp
 * Function definition.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.04.2009
 */

// TODO: Replace all this hack with Octave/GNU R integration.

#include <cstdlib>

#include <boost/assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/detail/concepts.hpp>

namespace function
{
  namespace ublas = boost::numeric::ublas;

  double const preferredPrecision = 1e-3;
  double const precisions[]       = { 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6 };
  double const step               = 30; // TODO: what is this?
  
  // Function minimum lower bound (for Kelley's convex cutting plane algorithm).
  double const minimumLowerBound = -1e4;
  
  // Minimum search start point (admissible point).
  double const startX    = -20;
  double const startY    = -20;
  
  // Lipschitz constant calculation parameters.
  double const lipschitzStepX = 1e-3;
  double const lipschitzStepY = 1e-3;
  double const loLipschitzX = -30;
  double const loLipschitzY = -30;
  double const hiLipschitzX =  30;
  double const hiLipschitzY =  30;
  
  // Barrier method specific.
  double const startMu = 1e6;
  double const beta    = 0.1;

  // Warning! Functions are not inline! :(
  
  // Function value.
  template< class V >
  typename V::value_type 
    function( V const &x )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));
    
    typedef typename V::value_type     scalar_type;
    typedef ublas::vector<scalar_type> vector_type;
  
    BOOST_ASSERT(x.size() == 2);
    
    scalar_type const x1 = x(0);
    scalar_type const x2 = x(1);
    
    return x1 * x1 + x2 * x2 - 10 * x1 - 8 * x2;
  }
  
  // Function gradient value.
  template< class V >
  ublas::vector<typename V::value_type> 
    functionGrad( V const &x )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef typename V::value_type     scalar_type;
    typedef ublas::vector<scalar_type> vector_type;
  
    BOOST_ASSERT(x.size() == 2);
    
    scalar_type const x1 = x(0);
    scalar_type const x2 = x(1);
    
    vector_type grad(2);
    grad(0) = 2 * x1 - 10;
    grad(1) = 2 * x2 - 8;
    return grad;
  }
  
  // TODO: Use single class for all constraint functions.
  template< class S >
  S limitFunc1( ublas::vector<S> const &x ) // TODO: Rename to 'constraint...'
  {
    BOOST_ASSERT(x.size() == 2);

    return x(0) + 2 * x(1) - 2;
  }
  
  template< class S >
  ublas::vector<S>
    limitFuncGrad1( ublas::vector<S> const &x )
  {
    BOOST_ASSERT(x.size() == 2);
    
    ublas::vector<S> v(2);
    v(0) = 1;
    v(1) = 2;
    return v;
  }
  
  template< class S >
  S limitFunc2( ublas::vector<S> const &x )
  {
    BOOST_ASSERT(x.size() == 2);

    return 2 * x(0) + x(1) - 2;
  }
  
  template< class S >
  ublas::vector<S>
    limitFuncGrad2( ublas::vector<S> const &x )
  {
    BOOST_ASSERT(x.size() == 2);
    
    ublas::vector<S> v(2);
    v(0) = 2;
    v(1) = 1;
    return v;
  }
} // End of namespace 'function'.
