/*
 * golden_section_search.hpp
 * Searching single dimension function minimum with golden section search algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 30.03.2009
 */

#ifndef NUMERIC_GOLDEN_SECTION_SEARCH_HPP
#define NUMERIC_GOLDEN_SECTION_SEARCH_HPP

#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace numeric
{
namespace golden_section
{
  // TODO: Output passed points for rendering in Gnuplot.
  template< class Func, class Scalar, class OStream >
  inline
  Scalar find_min( Func function, 
                   Scalar const lo, Scalar const hi, Scalar const precision, 
                   size_t *nStepsPtr = 0 )
  {
    // TODO: Now we assume that coordinates and function value are same scalar types.
    // TODO: Assert on correctness of `ostr'.
    
    typedef Scalar scalar_type;
    
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<Func, scalar_type, scalar_type>));
    
    BOOST_ASSERT(precision > 0);
    BOOST_ASSERT(lo <= hi);
    
    scalar_type const alpha = 2. / (sqrt(5) + 1.);
    
    scalar_type a = lo, b = hi;
    scalar_type x1 = a + (1 - alpha) * (b - a);
    scalar_type x2 = a + (    alpha) * (b - a);
    scalar_type y1 = func(x1);
    scalar_type y2 = func(x2);

    // Searching minimum with golden section search.
    size_t counter = 0;
    while (std::abs(b - a) >= precision)
    {
      BOOST_ASSERT(a  < x1);
      BOOST_ASSERT(x1 < x2);
      BOOST_ASSERT(x2 < b );

      scalar_type const oldDist = b - a;

      if (y1 <= y2)
      {
        b  = x2;
        x2 = x1;
        x1 = a + (1 - alpha) * (b - a);
        y2 = y1;
        y1 = func(x1);
      }
      else
      {
        a  = x1;
        x1 = x2;
        x2 = a + alpha * (b - a);
        y1 = y2;
        y2 = func(x2);
      }
      
      // debug
      scalar_type const newDist = b - a;
      BOOST_ASSERT(oldDist > newDist); 
      // end of debug
    
      counter++;
    }
  
    scalar_type const result = (y1 <= y2 ? x1 : x2);
    
    if (nStepsPtr)
      *nStepsPtr = counter;
    
    return result;
  }
} // End of namespace 'golden_section'.
} // End of namespace 'numeric'.

#endif // NUMERIC_GOLDEN_SECTION_SEARCH_HPP
