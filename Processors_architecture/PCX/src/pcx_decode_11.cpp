/* pcx_decode_11.cpp
 * PCX fast decoding routine.
 * Implementation #10.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #11. I/O by paired DWORDs.
//

#define DO_3_TIMES(expr) expr expr expr

namespace pcx
{
  void decode_11( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    do
    {
      while (image < imageEnd - 8) // We know, that input size must be enough for full image decoding.
      {
        unsigned int const dword1 = *((unsigned int const *)(input + 0));
      
        if (dword1 & 0xC0C0C0C0)
          break;

        unsigned int const dword2 = *((unsigned int const *)(input + 4));
        
        if (dword2 & 0xC0C0C0C0)
        {
          *((unsigned int *)(image + 0)) = dword1;
          input += 4;
          image += 4;
          break;
        }

        *((unsigned int *)(image + 0)) = dword1;
        *((unsigned int *)(image + 4)) = dword2;
        
        input += 8;
        image += 8;
      }
    
      DO_3_TIMES(
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
      ); // End of 'DO_3_TIMES'.
    } while (true);
  }
} // End of namespace 'pcx'.
