/*
 * gradient_descent.hpp
 * Searching funciton minimum with gradient descent algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

#ifndef NUMERIC_GRADIENT_DESCENT_HPP
#define NUMERIC_GRADIENT_DESCENT_HPP

#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace numeric
{
namespace gradient_descent
{
  // Outputting passed points for rendering in Gnuplot.
  template< class Func, class FuncGrad, class V, class OStream >
  inline
  ublas::vector<typename V::value_type> 
    find_min( Func function, FuncGrad functionGrad, 
              V const &startPoint,
              typename V::value_type precision,
              typename V::value_type step,
              OStream &ostr /* debug for Gnuplot */ )
  {
    // TODO: Now we assume that vector's coordinates and function values are same scalal types.
    // TODO: Assert on correctness of `ostr'.
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef typename V::value_type     scalar_type;
    typedef ublas::vector<scalar_type> vector_type;
    
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<Func,     scalar_type, vector_type>));
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<FuncGrad, vector_type, vector_type>));
    
    BOOST_ASSERT(precision > 0);
    
    // Setting current point to start point.
    vector_type x = startPoint;
    
    output_vector_coordinates(ostr, x, " ", "\n"); // debug for Gnuplot
    
    while (true)
    {
      // Searching next point in direction opposite to gradient.
      vector_type const dir = -functionGrad(x);
      
      
    }
    
    return x;
  }
} // End of namespace 'gradient_descent'.
} // End of namespace 'numeric'.

#endif // NUMERIC_GRADIENT_DESCENT_HPP
