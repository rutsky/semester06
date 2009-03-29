/*
 * gradient_descent.hpp
 * Gradient descent searching minimum.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

#ifndef NUMERIC_GRADIENT_DESCENT_HPP
#define NUMERIC_GRADIENT_DESCENT_HPP

#include "numeric_common.hpp"

namespace numeric
{
namespace gradient_descent
{
  template< class F, class FD, class V >
  V find_min( F function, FD, functionDerivative, V const &startPoint, V::value_type precision )
  {
    
  }
} // End of namespace 'gradient_descent'.
} // End of namespace 'numeric'.

#endif // NUMERIC_GRADIENT_DESCENT_HPP
