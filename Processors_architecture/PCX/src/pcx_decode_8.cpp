/* pcx_decode_8.cpp
 * PCX fast decoding routine.
 * Implementation #8.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_8( unsigned char const *input, size_t size,
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
          break;
        
        byte = *input++;
        
        if (count != 1)
        {
          if (image + count > imageEnd)
          {
            while (image != imageEnd)
              *image++ = byte;
            break;
          }
          else
          {
            for (; count > 0; --count)
              *image++ = byte;
            continue;
          }
        }
      }

      *image++ = byte;
      if (image == imageEnd)
        break;
    }
  }
} // End of namespace 'pcx'.
