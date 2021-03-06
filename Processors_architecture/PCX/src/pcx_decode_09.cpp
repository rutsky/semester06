/* pcx_decode_09.cpp
 * PCX fast decoding routine.
 * Implementation #9.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #9. Pairing in output.
//

namespace pcx
{
  void decode_09( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    while (input != inputEnd && image != imageEnd)
    {
      unsigned char byte = *input++;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        int count = (byte & 0x3F); // 0x3F = 2#00111111
        if (input == inputEnd)
          return; // Impossible on correct images.
        
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
              *(image + 0) = byte;
              *(image + 1) = byte;
              *(image + 2) = byte;
              *(image + 3) = byte;
              image += 4;
            }
            
            break;
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

            continue;
          }
        }
      }

      *image++ = byte;
    }
  }
} // End of namespace 'pcx'.
