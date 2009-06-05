/* div_subtraction.cpp
 * Division using subtraction.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2009
 */

#include "div_algs.h"

namespace division
{
namespace subtraction
{
  // Searching: t = x/y
  // Subtracting `y' from `x' while (y > 0).
  
  namespace impl_cpp
  {
    dword_t div( dword_t x, dword_t y )
    {
      dword_t quotient = 0;
      while (x >= y)
      {
        x -= y;
        ++quotient;
        if (x == 0)
          break;
      }
      
      return quotient;
    }
  }
  
  namespace impl_asm
  {
    dword_t div( dword_t x, dword_t y )
    {
      return x;
    }
  }
} // End of namespace 'subtraction'.
} // End of namespace 'division'.
