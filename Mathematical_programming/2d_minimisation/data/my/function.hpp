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
    grad(1) = 2. + 2. / sqrt(2 + x1 * x1 + x2 * x2) * 2 * x2;
    return grad;
  }
} // End of namespace 'function'
