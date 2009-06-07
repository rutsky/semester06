/* pcx_decode_13a.cpp
 * PCX fast decoding routine.
 * Implementation #13a.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #13a. I/O by blindly paired 2 DWORDs with MMX (only copying with MMX).
//

#define DO_4_TIMES(expr) expr expr expr expr

namespace pcx
{
  void decode_13a( unsigned char const *input, size_t size,
                   size_t width, size_t height,
                   unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    do
    {
      while (image < imageEnd - 8) // We know, that input size must be enough for full image decoding.
      {
        // Reading 8 bytes from input to mm0.
        asm("movq     mm0, QWORD PTR [%[inaddr]]" : : [inaddr]  "r"(input));
                        
        unsigned int       dword1 = *((unsigned int const *)(input + 0));
        unsigned int const dword2 = *((unsigned int const *)(input + 4));
        
        // Writing 8 bytes from mm0 to image.
        asm("movq     QWORD PTR [%[outaddr]], mm0": : [outaddr] "r"(image));
      
        *((unsigned int *)(image + 0)) = dword1;
        
        dword1 |= dword2;

        if (dword1 & 0xC0C0C0C0)
          break;

        *((unsigned int *)(image + 4)) = dword2;
        
        input += 8;
        image += 8;
      }
    
      // 4 times -- as average needed in guess error with 8 bytes.
      DO_4_TIMES(
      {
        unsigned char byte = *input++;

        if ((byte & 0xC0) != 0xC0) // 0xC0 = 2#11000000
          *image++ = byte;
        else
        {
          int count = (byte & 0x3F); // 0x3F = 2#00111111
        
          if (input == inputEnd)
            goto exit; // Impossible on correct images.
        
          byte = *input++;
        
          if (count == 1)
            *image++ = byte;
          else
          {
            if (image + count > imageEnd)
            {
              count = imageEnd - image;
            
              for (; count & 0x3; --count)
                *image++ = byte;
            
              for (; count > 0; count -= 4)
              {
                *(image + 0) = byte;
                *(image + 1) = byte;
                *(image + 2) = byte;
                *(image + 3) = byte;
                image += 4;
              }
            
              goto exit;
            }
            else
            {
              for (; count & 0x3; --count)
                *image++ = byte;
            
              for (; count > 0; count -= 4)
              {
                *(image + 0) = byte;
                *(image + 1) = byte;
                *(image + 2) = byte;
                *(image + 3) = byte;
                image += 4;
              }
            }
          }
        }
        
        if (input == inputEnd || image == imageEnd)
          goto exit;
      }
      ); // End of 'DO_4_TIMES'.
    } while (true);
    
  exit:
    // Freeing FPU registers.
    asm("emms": : );
  }
} // End of namespace 'pcx'.
