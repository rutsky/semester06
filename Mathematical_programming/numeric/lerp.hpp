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
    // Linear interpolation:
    //   x = y0 + (x - x0) * (y1 - y0) / (x1 - x0) 
    
    // TODO: Optimize.
    
    Lerp( S const &x0, S const &x1, T const &y0, T const &y1 )
      : x0_(x0)
      , x1_(x1)
      , y0_(y0)
      , y1_(y1)
    {
      BOOST_ASSERT(x0 != x1); // TODO: Do it with precision.
    }
    
    T operator()( S const &x )
    {
      return y0_ + (x - x0_) * (y1_ - y0_) / (x1_ - x0_);
    }
    
  private:
    S const x0_, x1_;
    T const y0_, y1_;
  };
  
  template< class S, class T >
  inline
  ublas::vector<T> lerp( S const &x0, S const &x1, ublas::vector<T> const &y0, ublas::vector<T> const &y1, S const &x )
  {
    return Lerp<S, T>(x0, x1, y0, y1)(x);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_LERP_HPP
