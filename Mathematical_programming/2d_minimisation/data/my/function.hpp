/*
 * function.hpp
 * Function definition.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

// TODO: Replace all this hack with Octave/GNU R integration.

#include <cstdlib>

#include <boost/assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/detail/concepts.hpp>

namespace function
{
  namespace ublas = boost::numeric::ublas;

  double const preferredPrecision = 1e-4;
  double const precisions[] = { 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8 };
  double const step      = 0.5;
  
  // Gradient descent algorithm specific.
  double const startX    = 2.5;
  double const startY    = 2.5;
  
  // Lipschitz specific.
  double const loLipschitzX       = -0.9;
  double const loLipschitzY       = -3;
  double const hiLipschitzX       = 2;
  double const hiLipschitzY       = 1;
  double const lipschitzStepX     = 0.01;
  double const lipschitzStepY     = 0.01;

  // Genetic algorithm specific.
  double const loGenX       = -0.9;
  double const loGenY       = -3;
  double const hiGenX       = 2;
  double const hiGenY       = 1;
  size_t const nIndividuals = 1000;
  size_t const nPrecisionSelect = 20;
  double const liveRate     = 0.8;
  double const mutationX    = 0.2;
  double const mutationY    = 0.2;

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
    
    return x1 * x1 * x1 + 2 * x2 + 4 * sqrt(2 + x1 * x1 + x2 * x2);
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
    grad(0) = 3. * x1 * x1 + 2. / sqrt(2 + x1 * x1 + x2 * x2) * 2 * x1;
    grad(1) = 2.           + 2. / sqrt(2 + x1 * x1 + x2 * x2) * 2 * x2;
    return grad;
  }
  
  // Function inverse Hessian matrix.
  template< class V >
  ublas::matrix<typename V::value_type> 
    inverseHessian( V const &x )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef typename V::value_type     scalar_type;
    typedef ublas::vector<scalar_type> vector_type;
    typedef ublas::matrix<scalar_type> matrix_type;
  
    BOOST_ASSERT(x.size() == 2);
    
    scalar_type const x1 = x(0);
    scalar_type const x2 = x(1);
    
    scalar_type const val = 2 + x1 * x1 + x2 * x2;
    scalar_type const sqrtval = sqrt(val);
    
    // TODO!
    scalar_type const d2fdx1dx1 = 2. + (4. * sqrtval - (4 * x1 * x1) / sqrtval) / val;
    scalar_type const d2fdx2dx2 = (4. * sqrtval - (4 * x2 * x2) / sqrtval) / val;
    scalar_type const d2fdx1dx2 = -4. * x1 * 2 * x2 / val;
    
    scalar_type const a = d2fdx1dx1;
    scalar_type const b = d2fdx1dx2;
    scalar_type const c = d2fdx1dx2;
    scalar_type const d = d2fdx2dx2;
    
    matrix_type invH(2, 2);
    invH(0, 0) =  d; invH(0, 1) = -b;
    invH(1, 0) = -c; invH(1, 1) =  a;
    
    scalar_type const denominator = a * d - b * c;
    BOOST_ASSERT(abs(denominator) > 1e-8);
    invH *= (1 / denominator);

    return invH;
  }
} // End of namespace 'function'
