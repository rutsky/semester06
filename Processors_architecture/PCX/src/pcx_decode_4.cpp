/* pcx_decode_3.cpp
 * PCX fast decoding routine.
 * Implementation #3.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_4( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *planeEnd = image + width;
    unsigned char const *imageEnd = image + height * 3 * width;
  
    // Decoding scan line.
    int plane = 0, d = 0;
    while (d < size && image != imageEnd)
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
        if (image == planeEnd)
        {
          // Completely filled current plane.
          planeEnd += width;
          ++plane;
        }

        if (plane >= 3)
        {
          plane = 0;
        }
        
        if (image == imageEnd)
        {
          // Readed complete image. Interrupting.
          break;
        }
        
        *image++ = byte;
      }
    }
  }
} // End of namespace 'pcx'.
