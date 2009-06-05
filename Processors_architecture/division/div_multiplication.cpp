/* div_multiplication.cpp
 * Division using multiplication.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2009
 */

#include "div_algs.h"

namespace division
{
namespace multiplication
{
  // Using this approximation:
  //  x       (2^16 + y - 1)         1
  // ---   =  -------------- * x * ------
  //  y             y               2^16
  //
  // Storing table:
  //          (2^16 + y - 1)
  // K[y]  =  --------------
  //                y       
  //
  // Then division will look like:
  //  x                    1
  // ---   =  K[y] * x * ------   =  (K[y] * x) >> 16
  //  y                   2^16
  
  namespace details
  {
    static dword_t const yEffectiveBits = 16;
    static dword_t const tableSize = (1 << yEffectiveBits);
    
    static bool    KFilled = false;
    static dword_t K[tableSize];
    
    static void fillTable()
    {
      K[0] = 0;
  
      dword_t const nominator = (1 << 16) - 1;
      for (dword_t y = 1; y < details::tableSize; ++y)
        K[y] = nominator / y + 1;
    }
  }
  
  namespace impl_cpp
  {
    dword_t div( dword_t x, dword_t y )
    {
      if (y >= (1 << details::yEffectiveBits))
        return 0;
      
      if (!details::KFilled)
      {
        details::fillTable();
        details::KFilled = true;
      }
    
      return (details::K[y] * x) >> 16;
    }
  }
  
  namespace impl_asm
  {
    dword_t div( dword_t x, dword_t y )
    {
      if (y >= (1 << details::yEffectiveBits))
        return 0;
      
      if (!details::KFilled)
      {
        details::fillTable();
        details::KFilled = true;
      }
      
      y = details::K[y];
      asm volatile ("mov     eax, %[y]":           : [y]"r"(y));
      asm volatile ("mul     %[x]"     :           : [x]"r"(x));
      asm volatile ("shr     eax, 16"  :           : );
      asm volatile ("mov     %[x], eax": [x]"=r"(x): );
      return x;
    }
  }
} // End of namespace 'multiplication'.
} // End of namespace 'division'.
