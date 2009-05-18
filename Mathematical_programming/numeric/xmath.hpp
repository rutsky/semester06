/* 
 * xmath.hpp
 * Useful math functions.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 25.10.2007
 */

#ifndef XMATH_HPP
#define XMATH_HPP

#include <cmath>

namespace xmath
{
  template< class T > inline T   abs  ( T a )                 { return a > 0 ? a : -a; }
  template< class T > inline T   max  ( T a, T b )            { return a > b ? a : b; }
  template< class T > inline T   min  ( T a, T b )            { return a < b ? a : b; }
  template< class T > inline T   sqr  ( T a )                 { return a * a; }
  template< class T > inline T   cube ( T a )                 { return a * a * a; }
  template< class T > inline int sign ( T val )               { return (val < 0) ? -1 : ((val > 0) ? 1 : 0); }
  template< class T > inline T   clamp( T val, T min, T max ) { return (val < min) ? min : ((val > max) ? max : val); }
  
  template< class S >
  struct scalar_traits
  { 
    typedef S scalar_type;
    
    static S epsilon();
    static S infinity();
  };
  
  template<> struct scalar_traits<int>
  { 
    static int    epsilon() { return 0;     }
  };
  template<> struct scalar_traits<float>
  {
    static float  epsilon()  { return 1e-8f; }
    static float  infinity() { return 1e+8f; }
  };
  template<> struct scalar_traits<double>
  {
    static double epsilon()  { return 1e-10; }
    static double infinity() { return 1e+10; }
  };
  
  template< class S > S epsilon()  { return scalar_traits<S>::epsilon();  }
  template< class S > S infinity() { return scalar_traits<S>::infinity(); }
  
  // TODO: Use true C++ magic.
  // Fuzzy greater or equal.
  template< class S1, class S2, class S3 >
  inline bool ge_impl( S1 a, S2 b, S3 eps )                            { return a - b >= -eps;      }
  inline bool ge( double a, double b, double eps = epsilon<double>() ) { return ge_impl(a, b, eps); }
  inline bool ge( double a, float  b, double eps = epsilon<double>() ) { return ge_impl(a, b, eps); }
  inline bool ge( double a, int    b, double eps = epsilon<double>() ) { return ge_impl(a, b, eps); }
  inline bool ge( float  a, double b, double eps = epsilon<double>() ) { return ge_impl(a, b, eps); }
  inline bool ge( float  a, float  b, float  eps = epsilon<float> () ) { return ge_impl(a, b, eps); }
  inline bool ge( float  a, int    b, float  eps = epsilon<float> () ) { return ge_impl(a, b, eps); }
  inline bool ge( int    a, double b, double eps = epsilon<double>() ) { return ge_impl(a, b, eps); }
  inline bool ge( int    a, float  b, float  eps = epsilon<float> () ) { return ge_impl(a, b, eps); }
  inline bool ge( int    a, int    b, int    eps = epsilon<int>   () ) { return ge_impl(a, b, eps); }
  
  // Fuzzy strong greater.
  template< class S1, class S2, class S3 >
  inline bool sg_impl( S1 a, S2 b, S3 eps )                            { return a - b > eps;        }
  inline bool sg( double a, double b, double eps = epsilon<double>() ) { return sg_impl(a, b, eps); }
  inline bool sg( double a, float  b, double eps = epsilon<double>() ) { return sg_impl(a, b, eps); }
  inline bool sg( double a, int    b, double eps = epsilon<double>() ) { return sg_impl(a, b, eps); }
  inline bool sg( float  a, double b, double eps = epsilon<double>() ) { return sg_impl(a, b, eps); }
  inline bool sg( float  a, float  b, float  eps = epsilon<float> () ) { return sg_impl(a, b, eps); }
  inline bool sg( float  a, int    b, float  eps = epsilon<float> () ) { return sg_impl(a, b, eps); }
  inline bool sg( int    a, double b, double eps = epsilon<double>() ) { return sg_impl(a, b, eps); }
  inline bool sg( int    a, float  b, float  eps = epsilon<float> () ) { return sg_impl(a, b, eps); }
  inline bool sg( int    a, int    b, int    eps = epsilon<int>   () ) { return sg_impl(a, b, eps); }

  // Fuzzy less or equal.
  template< class S1, class S2, class S3 >
  inline bool le_impl( S1 a, S2 b, S3 eps )                            { return a - b <= eps;       }
  inline bool le( double a, double b, double eps = epsilon<double>() ) { return le_impl(a, b, eps); }
  inline bool le( double a, float  b, double eps = epsilon<double>() ) { return le_impl(a, b, eps); }
  inline bool le( double a, int    b, double eps = epsilon<double>() ) { return le_impl(a, b, eps); }
  inline bool le( float  a, double b, double eps = epsilon<double>() ) { return le_impl(a, b, eps); }
  inline bool le( float  a, float  b, float  eps = epsilon<float> () ) { return le_impl(a, b, eps); }
  inline bool le( float  a, int    b, float  eps = epsilon<float> () ) { return le_impl(a, b, eps); }
  inline bool le( int    a, double b, double eps = epsilon<double>() ) { return le_impl(a, b, eps); }
  inline bool le( int    a, float  b, float  eps = epsilon<float> () ) { return le_impl(a, b, eps); }
  inline bool le( int    a, int    b, int    eps = epsilon<int>   () ) { return le_impl(a, b, eps); }
  
  // Fuzzy strong less.
  template< class S1, class S2, class S3 >
  inline bool sl_impl( S1 a, S2 b, S3 eps )                            { return a - b < -eps;       }
  inline bool sl( double a, double b, double eps = epsilon<double>() ) { return sl_impl(a, b, eps); }
  inline bool sl( double a, float  b, double eps = epsilon<double>() ) { return sl_impl(a, b, eps); }
  inline bool sl( double a, int    b, double eps = epsilon<double>() ) { return sl_impl(a, b, eps); }
  inline bool sl( float  a, double b, double eps = epsilon<double>() ) { return sl_impl(a, b, eps); }
  inline bool sl( float  a, float  b, float  eps = epsilon<float> () ) { return sl_impl(a, b, eps); }
  inline bool sl( float  a, int    b, float  eps = epsilon<float> () ) { return sl_impl(a, b, eps); }
  inline bool sl( int    a, double b, double eps = epsilon<double>() ) { return sl_impl(a, b, eps); }
  inline bool sl( int    a, float  b, float  eps = epsilon<float> () ) { return sl_impl(a, b, eps); }
  inline bool sl( int    a, int    b, int    eps = epsilon<int>   () ) { return sl_impl(a, b, eps); }
  
  // Fuzzy equal.
  template< class S1, class S2, class S3 >
  inline bool eq_impl( S1 a, S2 b, S3 eps )                            { return abs(a - b) <= eps;  }
  inline bool eq( double a, double b, double eps = epsilon<double>() ) { return eq_impl(a, b, eps); }
  inline bool eq( double a, float  b, double eps = epsilon<double>() ) { return eq_impl(a, b, eps); }
  inline bool eq( double a, int    b, double eps = epsilon<double>() ) { return eq_impl(a, b, eps); }
  inline bool eq( float  a, double b, double eps = epsilon<double>() ) { return eq_impl(a, b, eps); }
  inline bool eq( float  a, float  b, float  eps = epsilon<float> () ) { return eq_impl(a, b, eps); }
  inline bool eq( float  a, int    b, float  eps = epsilon<float> () ) { return eq_impl(a, b, eps); }
  inline bool eq( int    a, double b, double eps = epsilon<double>() ) { return eq_impl(a, b, eps); }
  inline bool eq( int    a, float  b, float  eps = epsilon<float> () ) { return eq_impl(a, b, eps); }
  inline bool eq( int    a, int    b, int    eps = epsilon<int>   () ) { return eq_impl(a, b, eps); }
  
  // Fuzzy equal zero.
  inline bool eq_zero( double a, double eps = epsilon<double>() ) { return eq(a, 0.,  eps); }
  inline bool eq_zero( float  a, float  eps = epsilon<float> () ) { return eq(a, 0.f, eps); }
  inline bool eq_zero( int    a, int    eps = epsilon<int>   () ) { return eq(a, 0,   eps); }
  
  inline double adjust( double val, double eps = epsilon<double>() ) { return eq_zero(val, eps) ? 0.  : val; }
  inline float  adjust( float  val, float  eps = epsilon<float> () ) { return eq_zero(val, eps) ? 0.f : val; }
  inline int    adjust( int    val, int    eps = epsilon<int>   () ) { return eq_zero(val, eps) ? 0   : val; }
  
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
  
  template< typename T, int size >
  inline size_t array_size( T (&)[size] ) { return size; }
  
  template< class T >
  inline T & make_min( T &a, T const &b )
  {
    if (a > b) a = b;
    return a;
  }
  
  template< class T >
  inline T & make_max( T &a, T const &b )
  {
    if (a < b) a = b;
    return a;
  }
  
  double natural_pow( double x, int y )
  {
    if (x >= 0)  
      return std::pow(x, y);
    else if (y % 2 == 1)
      return -std::pow(-x, y);
    else
      return std::pow(-x, y);
  }
} // End of namespace 'xmath'.

#endif // XMATH_HPP
