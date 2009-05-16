/*
 * gradient_descent.hpp
 * Searching multidimensional function minimum with gradient descent algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

#ifndef NUMERIC_GRADIENT_DESCENT_HPP
#define NUMERIC_GRADIENT_DESCENT_HPP

#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "golden_section_search.hpp"
#include "lerp.hpp"

namespace numeric
{
namespace gradient_descent
{
  // TODO: Implement all possible states handling.
  enum gradient_descent_result
  {
    gd_close_point = 0,     // Ok, next point founded by one dimension minimisation is almost old point.
    gd_zero_gradient,       // Ok, found point with almost zero gradient.
    gd_step_too_small,      // Ok, step decreased to value smaller than precision.
    gd_too_many_iterations, // Debug failure, iterations numer excided predefined number.
    gd_inf_gradient,        // Failure, in some point function gradient is infinite, algorithm interrupted.
    gd_inf_function,        // Failure, function value is infimum.
  };
  
  template< class Func, class FuncGrad, class V, class ConstrainPredicate, class PointsOut >
  inline
  gradient_descent_result
    find_min( Func function, FuncGrad functionGrad, 
              V const &startPoint,
              typename V::value_type precision,
              typename V::value_type step,
              V &result,
              ConstrainPredicate constrainPred,
              PointsOut          pointsOut )
  {
    // TODO: Now we assume that vector's coordinates and function values are same scalar types.
    // TODO: Assert on correctness of `pointsOut'.
    // TODO: On each iteration something is stored in some container.
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef typename V::value_type            scalar_type;
    typedef ublas::vector<scalar_type>        vector_type;
    typedef ublas::scalar_traits<scalar_type> scalar_traits_type;
    
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<Func,     scalar_type, vector_type>));
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<FuncGrad, vector_type, vector_type>));
    
    BOOST_ASSERT(precision > 0);
    
    // Setting current point to start point.
    vector_type x = startPoint;
    BOOST_ASSERT(constrainPred(x));
    
    *pointsOut++ = x;
    
    size_t iterations = 0;
    while (true)
    {
      // Searching next point in direction opposite to gradient.
      vector_type const grad = functionGrad(x);
      
      scalar_type const gradNorm = ublas::norm_2(grad);
      if (scalar_traits_type::equals(gradNorm, 0))
      {
        // Function gradient is almost zero, found minimum.
        result = x;
        return gd_zero_gradient;
      }
      
      // TODO: Use normal constants.
      scalar_type const inf = 1e8;
      // FIXME: Very tricky!
      scalar_type const gradInfNorm = ublas::norm_inf(grad);
      //std::cout << "grad=" << grad << std::endl;
      //std::cout << "infNorm(grad)=" << gradInfNorm << std::endl;
      if (gradInfNorm >= inf / 2)
      {
        // Infinite gradient. Thats bad. Really bad.
        result = x;
        std::cout << "GD exit by inf gradient: " << x << "; " << grad << std::endl; // debug
        return gd_inf_gradient;
      }
      
      vector_type const dir = -grad / gradNorm;
      BOOST_ASSERT(scalar_traits_type::equals(ublas::norm_2(dir), 1));
      
      vector_type nextX;
      do
      {
        vector_type const s0 = x;
        vector_type const s1 = s0 + dir * step;
        
        //std::cout << "golden start" << std::endl; // debug
        typedef boost::function<scalar_type ( scalar_type )> function_bind_type;
        function_bind_type functionBind = 
            boost::bind<scalar_type>(function, boost::bind<vector_type>(Lerp<scalar_type, vector_type>(0.0, 1.0, s0, s1), _1));
        scalar_type const section = 
            golden_section::find_min<function_bind_type, scalar_type>(functionBind, 0.0, 1.0, precision / step);
        BOOST_ASSERT(0 <= section && section <= 1);
        //std::cout << "golden end" << std::endl; // debug
        
        nextX = s0 + dir * step * section;
        if (ublas::norm_2(x - nextX) < precision)
        {
          // Next point is equal to current (with precision and constrain), seems found minimum.
          std::cout << "GD exit by constrain: " << s0 << "; " << (s1 - s0) << " ; " << grad << std::endl; // debug
          result = x;
          return gd_close_point;
        }
        
        //std::cout << "nextX=" << nextX << std::endl; // debug
        
        // TODO: Use normal constants.
        // FIXME: Tricky place!
        if (abs(function(nextX)) >= inf / 2)
        {
          result = x;
          return gd_inf_function;
        }
        
        // Decreasing search step if point is not admissible.
        if (!constrainPred(nextX)) // TODO: Do not rerund constrain predicate for same point.
          step /= 2.;
        
        if (step <= precision)
        {
          result = x;
          std::cout << "gd_step_too_small: " << x << ", step=" << step << ", prec=" << precision << std::endl; // debug
          return gd_step_too_small;
        }
      } while (!constrainPred(nextX));

      // Moving to next point.
      x = nextX;
      *pointsOut++ = x;
      
      ++iterations;
      
      // debug
      if (iterations >= 1000)
      {
        std::cerr << "gradient_descent::find_min(): Too many iterations!\n";
        break;
      }
      // end of debug
    }
    
    result = x;
    return gd_too_many_iterations;
  }
} // End of namespace 'gradient_descent'.
} // End of namespace 'numeric'.

#endif // NUMERIC_GRADIENT_DESCENT_HPP
