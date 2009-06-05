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
      if (x < y)
        return 0;
      
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
      if (x < y)
        return 0;
      
      dword_t quotient;
      asm volatile ("xor     eax, eax\n" // `eax' is `quotient'
                  "loop:"
                  "\tcmp     %[x], %[y]\n"
                  "\tjl      endloop\n"
                  
                  "\tsub     %[x], %[y]\n"
                  "\tinc     eax\n"
                  "\ttest    eax, eax\n"
                  "\tjz      endloop\n"
                  "jmp       loop\n"
                    
                  "endloop:\n"
                  "\tmov     %[quotient], eax"
                    : [quotient]"=r"(quotient) : [x]"r"(x), [y]"r"(y) ); 
      
      return quotient;
    }
  }
} // End of namespace 'subtraction'.
} // End of namespace 'division'.
