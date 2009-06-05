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

      dword_t z;
      asm volatile ("mov     eax, %[y]\n" // `eax' is `yy'
                  "loop0:\n"
                  "\tshl     eax, 1\n"
                  "\tcmp     %[x], eax\n"
                  "\tjge     loop0\n"
                  "\tshr     eax, 1\n"
                  // Calculated `yy' (`eax').
                  
                  "\txor     ecx, ecx\n"  // `ecx' is `z'
                  
                  "loop1:\n"
                  "\tcmp     eax, %[y]\n"
                  "\tjl      endloop\n"
                  
                  "\tshl     ecx, 1\n"
                  "\tcmp     %[x], eax\n"
                  "\tjl      pass\n"
                  //{
                    "\tsub     %[x], eax\n"
                    "\tinc     ecx\n"
                  //}
                  "pass:\n"
                  "\tshr     eax\n"
                  
                  "\tjmp     loop1\n"
                  
                  "endloop:\n"
                  
                  "\tmov     %[z], ecx\n"
                : [z]"=r"(z) : [y]"r"(y), [x]"r"(x));
      
      return z;
    }
  }
} // End of namespace 'long_division'.
} // End of namespace 'division'.
