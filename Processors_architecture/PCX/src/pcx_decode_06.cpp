/* pcx_decode_6.cpp
 * PCX fast decoding routine.
 * Implementation #6.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_6( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    while (input != inputEnd && image != imageEnd)
    {
      unsigned char byte = *input++;
      int count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        count = (byte & 0x3F); // 0x3F = 2#00111111
        if (input == inputEnd)
          break;
        
        byte = *input++;
      }
      
      for (; count > 0; --count)
      {
        if (image == imageEnd)
          break;
        
        *image++ = byte;
      }
    }
  }
} // End of namespace 'pcx'.
