/* main.cpp
 * Division algorithms.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <iostream>
#include <fstream>
#include <istream>
#include <memory>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "div_algs.h"

template< class T, size_t N >
size_t array_size( T (&)[N] ) { return N; }

// The main program function.
int main()
{
  using division::dword_t;
  
  typedef dword_t (*div_func_ptr_type)( dword_t x, dword_t y );
  
  div_func_ptr_type divAlgs[] =
    {
      &division::native::div,
      &division::multiplication::impl_cpp::div,
      &division::multiplication::impl_asm::div,
      &division::newton_raphson::impl_cpp::div,
      &division::newton_raphson::impl_asm::div,
      &division::subtraction::impl_cpp::div,
      &division::subtraction::impl_asm::div,
      &division::long_division::impl_cpp::div,
      &division::long_division::impl_asm::div,
    };
  
  char const * divAlgsNames[] =
    {
      "Native X/Y:         ",
      "Multiplication C++: ",
      "Multiplication ASM: ",
      "Newton-Raphson C++: ",
      "Newton-Raphson ASM: ",
      "Subtraction C++:    ",
      "Subtraction ASM:    ",
      "Long division C++:  ",
      "Long division ASM:  ",
    };
  
  // Tests must be unsigned, 16-bit and no division by zero.
  dword_t tests[][2] = 
    { 
      {1, 1}, {1, 5}, {1, (1 << 3)},
      {18243, 58},
      {2, 1}, {2, 2}, {2, 5}, {2, (1 << 14)},
      {(1 << 15), 2}, {(1 << 15), 37},
      {(1 << 14) + 1236, 32526},
      {25, 17}, {(1 << 7), (1 << 13)}, {(1 << 13), (1 << 18)},
      {(1 << 12) + 3195, (1 << 10) + 8598},
    };
  // TODO: {(1 << 14) + 5246, 5726}
  
  for (size_t alg = 0; alg < array_size(divAlgs); ++alg)
  {
    div_func_ptr_type div = divAlgs[alg];
    char const *name = divAlgsNames[alg];
    
    std::cout << name;
    
    for (size_t t = 0; t < array_size(tests); ++t)
    {
      dword_t const x = tests[t][0];
      dword_t const y = tests[t][1];
      
      std::cout << x << "/" << y << "=" << div(x, y) << "  ";
      std::cout.flush();
    }
    std::cout << std::endl;
  }
  
  return 0;
}
