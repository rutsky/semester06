/* pcx_decode_3.cpp
 * PCX fast decoding routine.
 * Implementation #3.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_3( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    // Decoding scan line.
    int plane = 0, x = 0, y = 0, d = 0;
    while (d < size && y < height)
    {
      unsigned char byte = input[d];
      ++d;
      int count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        // RLE encoded data.
        
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
        if (x >= width)
        {
          // Completely filled current plane.
          x = 0;
          ++plane;
        }

        if (plane >= 3)
        {
          plane = 0;
          ++y;
        }
        
        if (y >= height)
        {
          // Readed complete image. Interrupting.
          break;
        }
        
        image[y * 3 * width + plane * width + x] = byte;
        ++x;
      }
    }
  }
} // End of namespace 'pcx'.
