/* pcx_decode_10.cpp
 * PCX fast decoding routine.
 * Implementation #10.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #10. I/O by DWORD.
//

#define DO_4_TIMES(expr) expr expr expr expr

namespace pcx
{
  void decode_10( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    do
    {
      while (image < imageEnd - 4) // We know, that input size must be enough for full image decoding.
      {
        unsigned int const dword = *((unsigned int const *)input);
      
        if (dword & 0xC0C0C0C0)
          break;

        *((unsigned int *)image) = dword;
        image += 4;
        input += 4;
      }
    
      DO_4_TIMES(
      {
        unsigned char byte = *input++;

        if ((byte & 0xC0) != 0xC0) // 0xC0 = 2#11000000
          *image++ = byte;
        else
        {
          int count = (byte & 0x3F); // 0x3F = 2#00111111
        
          if (input == inputEnd)
            return; // Impossible on correct images.
        
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
            
              return;
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
          return;
      }
      ); // End of 'DO_4_TIMES'
    } while (true);
  }
} // End of namespace 'pcx'.
