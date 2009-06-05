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
      
      dword_t volatile result, quotient;
      asm volatile ("xor     %[quotient], %[quotient]\n"
                  "loop:"
                  "\tcmp     %[x], %[y]\n"
                  "\tjl      endloop\n"
                  
                  "\tsub     %[x], %[y]\n"
                  "\tinc     %[quotient]\n"
                  "\ttest    %[quotient], %[quotient]\n"
                  "\tjz      endloop\n"
                  "jmp       loop\n"
                    
                  "endloop:\n"
                  "\tmov     %[result], %[quotient]"
                    : [result]"=r"(result) : [x]"r"(x), [y]"r"(y), [quotient]"r"(quotient)); 
      
      return result;
    }
  }
} // End of namespace 'subtraction'.
} // End of namespace 'division'.
