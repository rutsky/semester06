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

namespace pcx
{
  void decode_10( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    while (input != inputEnd && image != imageEnd)
    {
      while (image < imageEnd - 4)
      {
        unsigned int dword = *((unsigned int const *)input);
      
        if (!(dword & 0xC0C0C0C0))
        {
          *((unsigned int *)image) = dword;
          image += 4;
          input += 4;
        }
        else
          break;
      }
    
      unsigned char byte = *input++;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        int count = (byte & 0x3F); // 0x3F = 2#00111111
        if (input == inputEnd)
          break;
        
        byte = *input++;
        
        if (count != 1)
        {
          if (image + count > imageEnd)
          {
            count = imageEnd - image;
            
            for (; count & 0x3; --count)
              *image++ = byte;
            
            for (; count > 0; count -= 4)
            {
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
            }
            break;
          }
          else
          {
            for (; count & 0x3; --count)
              *image++ = byte;
            
            for (; count > 0; count -= 4)
            {
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
            }
              
            continue;
          }
        }
      }

      *image++ = byte;
    }
  }
} // End of namespace 'pcx'.
