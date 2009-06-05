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
      
      dword_t volatile result, yCur, yNext, tmp;
      asm volatile (
                  "mov     %[yNext], %[y]\n"
                  "loop:\n"
                  "\tmov     %[yCur], %[yNext]\n"
                  
                  "\timul    %[yCur], %[yCur]\n"
                  "\timul    %[yCur], %[y]\n"
                  "\tshr     %[yCur], 16\n"     // now `yCur' == ((yCur * yCur * y) >> 16)
                  
                  "\tlea     %[tmp], [%[yNext] + %[yNext]]\n"
                  "\tsub     %[tmp], %[yCur]\n"    // now `tmp' == yNext
                  
                  "\tcmp     %[tmp], %[yNext]\n"
                  "\tmov     %[yNext], %[tmp]\n"
                  "\tjne     loop\n"
                  "\tmov     %[result], %[yNext]\n"
                  : [result]"=r"(result) : [x]"r"(x), [y]"r"(y), [yNext]"r"(yNext), [yCur]"r"(yCur), [tmp]"r"(tmp));
      
      return (x * result) >> 16;
    }
  }
} // End of namespace 'newton_raphson'.
} // End of namespace 'division'.
