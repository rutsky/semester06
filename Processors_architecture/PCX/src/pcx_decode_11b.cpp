/* pcx_decode_11b.cpp
 * PCX fast decoding routine.
 * Implementation #11a.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #11a. I/O by blindly paired 4 DWORDs.
//

#define DO_3_TIMES(expr) expr expr expr
#define DO_8_TIMES(expr) expr expr expr expr expr expr expr expr

namespace pcx
{
  void decode_11b( unsigned char const *input, size_t size,
                   size_t width, size_t height,
                   unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    do
    {
      while (image < imageEnd - 16) // We know, that input size must be enough for full image decoding.
      {
        unsigned int dword1 = *((unsigned int const *)(input +  0));
        unsigned int dword2 = *((unsigned int const *)(input +  4));
        *((unsigned int *)(image +  0)) = dword1;
        
        dword1 |= dword2;
        *((unsigned int *)(image +  4)) = dword2;
        
        dword2 = *((unsigned int const *)(input +  8));
        dword1 |= dword2;
        *((unsigned int *)(image +  8)) = dword2;

        dword2 = *((unsigned int const *)(input + 12));
        dword1 |= dword2;
        *((unsigned int *)(image + 12)) = dword2;

        if (dword1 & 0xC0C0C0C0)
          break;

        input += 16;
        image += 16;
      }
    
      DO_8_TIMES(
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
      ); // End of 'DO_8_TIMES'.
    } while (true);
  }
} // End of namespace 'pcx'.
