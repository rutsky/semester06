/* div_algs.cpp
 * Division algorithms.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2009
 */

#include <cstddef>

namespace division
{
  typedef unsigned int dword_t;
  
  namespace native
  {
    inline 
    dword_t div( dword_t x, dword_t y )
    {
      return x / y;
    }
  }
  
  namespace multiplication
  {
    namespace impl_cpp
    {
      dword_t div( dword_t x, dword_t y );
    }
    namespace impl_asm
    {
      dword_t div( dword_t x, dword_t y );
    }
  }
  
  namespace newton_raphson
  {
    namespace impl_cpp
    {
      dword_t div( dword_t x, dword_t y );
    }
    namespace impl_asm
    {
      dword_t div( dword_t x, dword_t y );
    }
  }
} // End of namespace 'division'.
