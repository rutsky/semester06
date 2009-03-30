/*
 * lerp.hpp
 * Interpolations of values.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 30.03.2009
 */

#ifndef NUMERIC_LERP_HPP
#define NUMERIC_LERP_HPP

#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

namespace numeric
{
  //
  // Linear interpolation.
  //
  
  // TODO: Not sure is type `V' should be used so...
  template< class S, class T >
  class Lerp
  {
  public:
    Lerp( S const &x0, S const &x1, T const &y0, T const &y1 )
      : y0_(y0)
    {
      BOOST_ASSERT(x0 != x1); // TODO: Do it with precision.
      dy_ = (y1 - y0) / (x1 - x0);
    }
    
    T operator()( S const &x )
    {
      return y0_ + x * dy_;
    }
    
  private:
    T y0_, dy_;
  };
  
  template< class S, class T >
  inline
  ublas::vector<T> lerp( S const &x0, S const &x1, ublas::vector<T> const &y0, ublas::vector<T> const &y1, S const &x )
  {
    return Lerp<S, T>(x0, x1, y0, y1)(x);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_GOLDEN_SECTION_SEARCH_HPP
