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

// TODO: Look how asserts implemented in Boost::Test.
// TODO: Must be more verbose.
#define ASSERT(e) \
    BOOST_ASSERT(e)

#define ASSERT_EQ(a, b) \
    { \
      if (!((a) == (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' == `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    { \
      if (!((a) != (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' != `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_LT(a, b) \
    { \
      if (!((a) < (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' < `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_LE(a, b) \
    { \
      if (!((a) <= (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' <= `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_GT(a, b) \
    { \
      if (!((a) > (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' > `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_GE(a, b) \
    { \
      if (!((a) >= (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' >= `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_FUZZY_EQ(a, b) \
    { \
      if (!eq((a), (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' fuzzy equal to `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_FUZZY_EQ_EPS(a, b, eps) \
    ASSERT(xmath::eq((a), (b), (eps)))

#define ASSERT_FUZZY_NEQ(a, b) \
    { \
      if (!!eq((a), (b))) \
      { \
        std::cerr << "Assertion failed: `" << (a) << "' fuzzy not equal to `" << (b) << "'" << std::endl; \
        BOOST_ASSERT(0); \
      } \
    } while(0)

#define ASSERT_FUZZY_NEQ_EPS(a, b, eps) \
    ASSERT(!xmath::eq((a), (b), (eps)))

#define ASSERT_FUZZY_EQ_ZERO(a) \
    ASSERT(xmath::eq_zero((a)))
   
#define ASSERT_FUZZY_EQ_ZERO_EPS(a, eps) \
    ASSERT(xmath::eq_zero((a), (eps)))

// TODO: Other macroses: le, ge, sl, sg.

// TODO: Write all functions' above `verify' version.
#define VERIFY(e) \
    BOOST_VERIFY(e)

#define VERIFY_EQ(a, b) \
    BOOST_VERIFY((a) == (b))

#endif // XMATH_BOOST_HPP
