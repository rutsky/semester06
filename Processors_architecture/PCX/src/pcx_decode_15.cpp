/* pcx_decode_15.cpp
 * PCX fast decoding routine.
 * Implementation #15.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #15. Only using SSE for output.
//

#include <cstring>

namespace pcx
{
  void decode_15( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    while (input <= inputEnd && image <= imageEnd)
    {
      unsigned char byte = *input++;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        int count = (byte & 0x3F); // 0x3F = 2#00111111
        if (input == inputEnd)
          goto exit; // Impossible on correct images.
        
        byte = *input++;
        
        if (count != 1)
        {
          if (image + count > imageEnd)
          {
            count = imageEnd - image;
            
            for (; count & (8 - 1); --count)
              *image++ = byte;
              
            if (count != 0)
            {
              dword_type dword = byte;
              dword <<= 8;
              dword |= byte;
              
              // Loading word with 2 bytes to repeat into mm0 lower word.
              asm("movd    mm0, %[var]": : [var]"r"(dword));
              // Filling mm1 with 8 repeat bytes.
              asm("pshufw   mm1, mm0, 0": : );
              
              switch (count >> 3)
              {
              case 7:
                asm("movq    QWORD PTR[%[outaddr] + 48], mm1": : [outaddr]"r"(image));
              case 6:
                asm("movq    QWORD PTR[%[outaddr] + 40], mm1": : [outaddr]"r"(image));
              case 5:
                asm("movq    QWORD PTR[%[outaddr] + 32], mm1": : [outaddr]"r"(image));
              case 4:
                asm("movq    QWORD PTR[%[outaddr] + 24], mm1": : [outaddr]"r"(image));
              case 3:
                asm("movq    QWORD PTR[%[outaddr] + 16], mm1": : [outaddr]"r"(image));
              case 2:
                asm("movq    QWORD PTR[%[outaddr] +  8], mm1": : [outaddr]"r"(image));
              case 1:
                asm("movq    QWORD PTR[%[outaddr] +  0], mm1": : [outaddr]"r"(image));
              }
            }
            
            goto exit;
          }
          else
          {
            for (; count & (8 - 1); --count)
              *image++ = byte;
              
            if (count != 0)
            {
              dword_type dword = byte;
              dword <<= 8;
              dword |= byte;
              
              // Loading word with 2 bytes to repeat into mm0 lower word.
              asm("movd    mm0, %[var]": : [var]"r"(dword));
              // Filling mm1 with 8 repeat bytes.
              asm("pshufw   mm1, mm0, 0": : );
              
              switch (count >> 3)
              {
              case 7:
                asm("movq    QWORD PTR[%[outaddr] + 48], mm1": : [outaddr]"r"(image));
              case 6:
                asm("movq    QWORD PTR[%[outaddr] + 40], mm1": : [outaddr]"r"(image));
              case 5:
                asm("movq    QWORD PTR[%[outaddr] + 32], mm1": : [outaddr]"r"(image));
              case 4:
                asm("movq    QWORD PTR[%[outaddr] + 24], mm1": : [outaddr]"r"(image));
              case 3:
                asm("movq    QWORD PTR[%[outaddr] + 16], mm1": : [outaddr]"r"(image));
              case 2:
                asm("movq    QWORD PTR[%[outaddr] +  8], mm1": : [outaddr]"r"(image));
              case 1:
                asm("movq    QWORD PTR[%[outaddr] +  0], mm1": : [outaddr]"r"(image));
              }
              
              image += count;
            }
              
            continue;
          }
        }
      }

      *image++ = byte;
    }
    
  exit:
    // Freeing FPU registers.
    asm("emms": : );
  }
} // End of namespace 'pcx'.
