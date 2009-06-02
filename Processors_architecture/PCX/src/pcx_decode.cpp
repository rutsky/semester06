/* pcx_decode.cpp
 * PCX fast decoding routines.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <iostream>

#include "pcx.h"

namespace pcx
{
  // Decodes RLE compressed 24 bit RGB image without pallete as in ZSoft PCX version 3 or greater file format.
  // Each line of image consists of 3 planes (`R', `G', `B').
  // All planes are encoded as single chunk in input (incorrect by documentation that I read).
  // Output is an array of `height' resulting image lines.
  // Each line of output is an array of the image's row colors (`R', `G', `B').
  // Note: memory for output image must be allocated!
  void decode( unsigned char const *input, size_t size,
               size_t width, size_t height,
               unsigned char *image )
  {
    size_t const nPlanes = 3;
    
    // Decoding scan line.
    size_t plane = 0, x = 0, y = 0, d = 0;
    while (d < size && y < height)
    {
      unsigned char byte = input[d];
      ++d;
      size_t count = 1;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        // RLE encoded data.
        
        count = (byte & 0x3F); // 0x3F = 2#00111111
        if (d >= size)
        {
          std::cerr << "Error: interrupting uncomplete decoding pixel (" << x << "," << y << 
              "): end of line input buffer." << std::endl;
          break;
        }
        
        byte = input[d];
        ++d;
      }
      
      for (size_t i = 0; i < count; ++i)
      {
        if (x >= width)
        {
          // Completely filled current plane.
          x = 0;
          ++plane;
        }

        if (plane >= nPlanes)
        {
          plane = 0;
          ++y;
        }
        
        if (y >= height)
        {
          // Readed complete image. Interrupting.
          break;
        }
        
        image[y * nPlanes * width + plane * width + x] = byte;
        ++x;
      }
    }
  }
} // End of namespace 'pcx'.
