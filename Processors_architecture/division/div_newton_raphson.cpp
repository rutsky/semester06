/* div_newton_raphson.cpp
 * Division using Newton-Rafson algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2009
 */

#include "div_algs.h"

namespace division
{
namespace newton_raphson
{
  // Searching:  t = 1 / y.
  //
  // y - 1 / t = 0
  //
  // f(t) = y - 1 / t
  //
  // Newton-Raphson iterative process:
  // t0 = y
  // t_k+1 = t_k - f(t_k) / f'(t_k) = t_k * (2 - x * t_k)
  //
  // For integral process:
  // f(t) = y - K / t, where K = (1 << 16)
  
  namespace impl_cpp
  {
    dword_t div( dword_t x, dword_t y )
    {
      if (x < y)
        return 0;
      if (x == y)
        return 1;
      if (y == 1)
        return x;
      
      dword_t yCur, yNext = y;
      do
      {
        yCur = yNext;
        yNext = yCur * 2 - ((yCur * yCur * y) >> 16); // TODO: `yCur * yCur * y' can overflow 32 bit register.
      } while (yCur - yNext != 0);
      
      return (x * yCur) >> 16;
    }
  }
  
  namespace impl_asm
  {
    dword_t div( dword_t x, dword_t y )
    {
      if (x < y)
        return 0;
      if (x == y)
        return 1;
      if (y == 1)
        return x;
      
      dword_t result;
      asm volatile (
                  "mov     ecx, %[y]\n"   // `ecx' is `yNext'
                  "loop:\n"
                  "\tmov     eax, ecx\n"    // `eax' is `yCur'
                  
                  "\tmul     eax\n"
                  "\tmul     %[y]\n"
                  "\tshr     eax, 16\n"     // now `eax' == ((yCur * yCur * y) >> 16)
                  
                  "\tlea     edx, [ecx + ecx]\n"
                  "\tsub     edx, eax\n"    // now `edx' == yNext
                  
                  "\tcmp     edx, ecx\n"
                  "\tmov     ecx, edx\n"
                  "\tjne     loop\n"
                  "\tmov     %[result], ecx\n"
                  : [result]"=r"(result) : [x]"r"(x), [y]"r"(y));
      
      return (x * result) >> 16;
    }
  }
} // End of namespace 'newton_raphson'.
} // End of namespace 'division'.
