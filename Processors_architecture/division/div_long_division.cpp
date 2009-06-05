/* div_long_division.cpp
 * Long division.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2009
 */

#include "div_algs.h"

namespace division
{
namespace long_division
{
  // Classic one-bit long division.
  
  namespace impl_cpp
  {
    dword_t div( dword_t x, dword_t y )
    {
      if (x < y)
        return 0;

      dword_t yyNext = y, yy;
      do
      {
        yy = yyNext;
        yyNext = yyNext << 1;
      } while (x >= yyNext);
      
      dword_t z = 0;
      while (yy >= y)
      {
        z = z << 1;
        if (x >= yy)
        {
          x = x - yy;
          z |= 1;
        }
        yy = yy >> 1;
      }
      
      return z;
    }
  }
  
  namespace impl_asm
  {
    dword_t div( dword_t x, dword_t y )
    {
      return x;
    }
  }
} // End of namespace 'long_division'.
} // End of namespace 'division'.
