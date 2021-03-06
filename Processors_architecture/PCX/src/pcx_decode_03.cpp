/* pcx_decode_03.cpp
 * PCX fast decoding routine.
 * Implementation #3.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #03. Using `int' instead of `size_t'.
//

namespace pcx
{
  void decode_03( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    int plane = 0, x = 0, y = 0, d = 0;
    while (d < static_cast<int>(size) && y < static_cast<int>(height))
    {
      unsigned char byte = input[d];
      ++d;
      int count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        count = (byte & 0x3F); // 0x3F = 2#00111111
        if (d >= static_cast<int>(size))
        {
          break;
        }
        
        byte = input[d];
        ++d;
      }
      
      for (int i = 0; i < count; ++i)
      {
        if (x >= static_cast<int>(width))
        {
          x = 0;
          ++plane;
        }

        if (plane >= 3)
        {
          plane = 0;
          ++y;
        }
        
        if (y >= static_cast<int>(height))
          break;
        
        image[y * 3 * width + plane * width + x] = byte;
        ++x;
      }
    }
  }
} // End of namespace 'pcx'.
