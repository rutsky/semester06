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

      dword_t yy = y;
      do
      {
        yy <<= 1;
      } while (x >= yy);
      yy >>= 1;
      
      dword_t z = 0;
      while (yy >= y)
      {
        z = z << 1;
        if (x >= yy)
        {
          x = x - yy;
          z |= 1; // or `++z'
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
      if (x < y)
        return 0;

      dword_t volatile z, yy, result;
      asm volatile ("mov     %[yy], %[y]\n"
                  "loop0:\n"
                  "\tshl     %[yy], 1\n"
                  "\tcmp     %[x], %[yy]\n"
                  "\tjge     loop0\n"
                  "\tshr     %[yy], 1\n"
                  // Calculated `yy'
                  
                  "\txor     %[z], %[z]\n"
                  
                  "loop1:\n"
                  "\tcmp     %[yy], %[y]\n"
                  "\tjl      endloop\n"
                  
                  "\tshl     %[z], 1\n"
                  "\tcmp     %[x], %[yy]\n"
                  "\tjl      pass\n"
                  //{
                    "\tsub     %[x], %[yy]\n"
                    "\tinc     %[z]\n"
                  //}
                  "pass:\n"
                  "\tshr     %[yy]\n"
                  
                  "\tjmp     loop1\n"
                  
                  "endloop:\n"
                  "\tmov     %[result], %[z]\n"
                : [result]"=r"(result) : [y]"r"(y), [x]"r"(x), [yy]"r"(yy), [z]"r"(z));
      
      return result;
    }
  }
} // End of namespace 'long_division'.
} // End of namespace 'division'.
