/*
 * function.hpp
 * Function definition.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

// TODO: Replace all this hack with Octave/GNU R integration.

#include <cassert>
#include <cstdlib>

namespace function
{
  template< class Vector >
  inline
  typename Vector::value_type function( Vector x )
  {
    // TODO: May be assert that we using Boost.uBLAS vector?
    typedef typename Vector::value_type scalar_type;
  
    assert(x.size() == 2);
    
    scalar_type const x1 = x(0);
    scalar_type const x2 = x(1);
    
    return x1 * x1 * x1 + 2 * x2 + 4 * sqrt(2 + x1 * x1 + x2 * x2);
  }
} // End of namespace 'function'
