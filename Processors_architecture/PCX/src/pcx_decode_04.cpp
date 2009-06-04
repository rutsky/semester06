/* pcx_decode_04.cpp
 * PCX fast decoding routine.
 * Implementation #4.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #4. Deduced output image addressing.
//

namespace pcx
{
  void decode_04( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
  
    int d = 0;
    while (d < size && image != imageEnd)
    {
      unsigned char byte = input[d];
      ++d;
      int count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        count = (byte & 0x3F); // 0x3F = 2#00111111
        if (d >= size)
        {
          break;
        }
        
        byte = input[d];
        ++d;
      }
      
      for (int i = 0; i < count; ++i)
      {
        if (image == imageEnd)
          break;
        
        *image++ = byte;
      }
    }
  }
} // End of namespace 'pcx'.
