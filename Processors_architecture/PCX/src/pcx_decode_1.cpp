/* pcx_decode_1.cpp
 * PCX fast decoding routine.
 * Implementation #1.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_1( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    size_t const nPlanes = 3;
    
    size_t plane = 0, x = 0, y = 0, d = 0;
    while (d < size && y < height)
    {
      unsigned char byte = input[d];
      ++d;
      size_t count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        count = (byte & 0x3F); // 0x3F = 2#00111111
        if (d >= size)
          break;

        byte = input[d];
        ++d;
      }
      
      for (size_t i = 0; i < count; ++i)
      {
        if (x >= width)
        {
          x = 0;
          ++plane;
        }

        if (plane >= nPlanes)
        {
          plane = 0;
          ++y;
        }
        
        if (y >= height)
          break;
        
        image[y * nPlanes * width + plane * width + x] = byte;
        ++x;
      }
    }
  }
} // End of namespace 'pcx'.
