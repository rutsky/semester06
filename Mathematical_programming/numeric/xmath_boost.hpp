/* 
 * xmath_boost.hpp
 * Useful boost dependent functions.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 18.05.2009
 */

#ifndef XMATH_BOOST_HPP
#define XMATH_BOOST_HPP

#include <boost/function.hpp>

#include "xmath.hpp"

namespace xmath
{
  template< class S >
  inline boost::function<bool( S, S )> ge_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S, S ))&ge, _1, _2, eps); 
  }
  
  template< class S >
  inline boost::function<bool( S, S )> sg_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S, S ))&sg, _1, _2, eps); 
  }
  
  template< class S >
  inline boost::function<bool( S, S )> le_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S, S ))&le, _1, _2, eps); 
  }

  template< class S >
  inline boost::function<bool( S, S )> sl_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S, S ))&sl, _1, _2, eps); 
  }
  
  template< class S >
  inline boost::function<bool( S, S )> eq_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S, S ))&eq, _1, _2, eps); 
  }

  template< class S >
  inline boost::function<bool( S )> eq_zero_functor( S eps = epsilon<S>() )
  { 
    return boost::bind<bool>((bool(*)( S, S ))&eq_zero, _1, eps); 
  }
} // End of namespace 'xmath'.

#endif // XMATH_BOOST_HPP
