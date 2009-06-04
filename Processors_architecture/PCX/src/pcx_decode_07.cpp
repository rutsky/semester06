/* pcx_decode_07.cpp
 * PCX fast decoding routine.
 * Implementation #7.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #7. Inlined outputting.
//

namespace pcx
{
  void decode_07( unsigned char const *input, size_t size,
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
        
        if (image + count > imageEnd)
          count = (imageEnd - image);
        
        for (; count > 0; --count)
          *image++ = byte;
      }
      else
        *image++ = byte;
    }
  }
} // End of namespace 'pcx'.
