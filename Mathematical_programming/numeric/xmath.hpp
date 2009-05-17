/* xmath.h
 * Useful math functions.
 * Bob Rutsky, 2057/2
 * 25.10.2007
 */

#ifndef XMATH_H
#define XMATH_H

#include <cmath>

template< class T >
  struct scalar_traits
{ 
  static T epsilon();
};

template<> struct scalar_traits<int>
{ 
  static int epsilon() { return 0; }
};
template<> struct scalar_traits<float>
{
  static float epsilon() { return 1e-8f; }
};
template<> struct scalar_traits<double>
{
  static double epsilon() { return 1e-10; }
};

// TODO!
#ifndef WIN32
template< class ScalarType > ScalarType epsilon() { return scalar_traits<ScalarType>::epsilon(); }
#else // WIN32
template< class ScalarType > double epsilon() { return 1e-10; }
#endif // WIN32

//template< class T >
//       bool ge( T a, T b, T eps = epsilon<T>() )                     { return a - b >= -eps; }
inline bool ge( double a, double b, double eps = epsilon<double>() ) { return a - b >= -eps; }
inline bool ge( float  a, float  b, float  eps = epsilon<float> () ) { return a - b >= -eps; }
inline bool ge( double a, int    b, double eps = epsilon<double>() ) { return a - b >= -eps; }
inline bool ge( int    a, double b, double eps = epsilon<double>() ) { return a - b >= -eps; }
inline bool ge( float  a, int    b, float  eps = epsilon<float> () ) { return a - b >= -eps; }
inline bool ge( int    a, float  b, float  eps = epsilon<float> () ) { return a - b >= -eps; }

//template< class T >
//       bool le( T a, T b, T eps = epsilon<T>() )               { return a - b <= eps; }
inline bool le( double a, double b, double eps = epsilon<double>() ) { return a - b <= eps; }
inline bool le( float  a, float  b, float  eps = epsilon<float> () ) { return a - b <= eps; }
inline bool le( double a, int    b, double eps = epsilon<double>() ) { return a - b <= eps; }
inline bool le( int    a, double b, double eps = epsilon<double>() ) { return a - b <= eps; }
inline bool le( float  a, int    b, float  eps = epsilon<float> () ) { return a - b <= eps; }
inline bool le( int    a, float  b, float  eps = epsilon<float> () ) { return a - b <= eps; }

//template< class T >
//       bool eq( T a, T b, T eps = epsilon<T>() )                     { return ge(a, b, eps) && le(a, b, eps); }
inline bool eq( double a, double b, double eps = epsilon<double>() ) { return ge(a, b, eps) && le(a, b, eps); }
inline bool eq( float  a, float  b, float  eps = epsilon<float> () ) { return ge(a, b, eps) && le(a, b, eps); }
//inline bool eq( int    a, int    b, double eps = epsilon<int>   () ) { return ge(a, b, eps) && le(a, b, eps); }
inline bool eq( double a, int    b, double eps = epsilon<double>() ) { return ge(a, double(b), eps) && le(a, double(b), eps); }
//inline bool eq( int    a, double b, double eps = epsilon<double>() ) { return ge(double(a), b, eps) && le(double(a), b, eps); }
inline bool eq( float  a, int    b, float  eps = epsilon<float> () ) { return ge(a, float(b), eps) && le(a, float(b), eps); }
//inline bool eq( int    a, float  b, float  eps = epsilon<float> () ) { return ge(float(a), b, eps) && le(float(a), b, eps); }

inline bool eq_zero( double a, double eps = epsilon<double>() ) { return eq(a, 0., eps); }
inline bool eq_zero( float  a, float  eps = epsilon<float> () ) { return eq(a, 0.f, eps); }
inline bool eq_zero( int    a, double eps = epsilon<int>   () ) { return eq(a, 0,   eps); }

inline double adjust( double val, double eps = epsilon<double>() ) { return eq_zero(val, eps) ? 0. : val; }
inline float  adjust( float  val, float  eps = epsilon<float> () ) { return eq_zero(val, eps) ? 0.f : val; }
inline int    adjust( int    val, double eps = epsilon<int>   () ) { return eq_zero(val, eps) ? 0  : val; }

template< class T > T abs( T a ) { return a > 0 ? a : -a; }
template< class T > T max( T a, T b ) { return a > b ? a : b; }
template< class T > T min( T a, T b ) { return a < b ? a : b; }
template< class T > T sqr( T a ) { return a * a; }
template< class T > T cube( T a ) { return a * a * a; }
template< class T > int sign( T val ) { return (val < 0) ? -1 : ((val > 0) ? 1 : 0); }
template< class T > T clamp( T val, T min, T max ) { return (val < min) ? min : ((val > max) ? max : val); }

#ifndef WIN32
template< typename T, int size >
int array_size( T (&)[size] ) { return size; };
#else
#define array_size(a) (sizeof(a) / sizeof((a)[0]))
#endif // WIN32

#ifdef WIN32
namespace std
{
  double pow( double x, double y )
  {
    double res = ::pow(x, y);
    return res;
  }
}
#endif // WIN32

template< class T > void make_min( T &to_be_min, T val )
{  
  if (to_be_min > val)
    to_be_min = val;
}

template< class T > void make_max( T &to_be_max, T val )
{  
  if (to_be_max < val)
    to_be_max = val;
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

#endif // XMATH_H
