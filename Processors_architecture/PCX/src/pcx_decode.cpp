/* pcx_decode.cpp
 * PCX fast decoding routines.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  // Decodes RLE compressed 24 bit image without pallete as in ZSoft PCX version 3 or greater file format.
  // Each line of image consists of 3 planes (`R', `G', `B').
  // Each plane are encoded with RLE in `input' using exactly `bytesPerLine' bytes.
  // So input must be of `height' * `bytesPerLine' * 3 bytes.
  // Output is an array of `height' resulting image lines.
  // Each line of output is an array of the image's row colors (`R', `G', `B').
  // Note: memory for output image must be allocated!
  void decode( unsigned char *input,
               size_t width, size_t height,
               size_t bytesPerLine,
               unsigned char *image[] )
  {
    size_t const nPlanes = 3;
  
    for (size_t y = 0; y < height; ++y)
    {
      // Decoding each resulting image row.
      
      for (size_t plane = 0; plane < nPlanes; ++plane)
      {
        // Decoding each plane (`R', `G', `B') of current row.

        size_t x = 0;
        for (size_t d = 0; d < bytesPerLine && x < width; ++d)
        {
          // Decoding plane byte.
          unsigned char const byte = input[(y * nPlanes + plane) * bytesPerLine + d];
          
          if ((byte & 0xC0) = 0xC0) // 0xC0 = 2#11000000
          {
            // RLE encoded data.
            
            size_t const nRepeat = (byte & 0x3F); // 0x3F = 2#00111111
            ++d;
            
            for (d < bytesPerLine && x < width; ++x, ++d)
            {
              unsigned char const byte = input[(y * nPlanes + plane) * bytesPerLine + d];
              image[y * width * nPlanes + x * nPlanes + plane] = byte;
            }
          }
          else
          {
            // Raw data.
            image[y * width * nPlanes + x * nPlanes + plane] = byte;
            ++x;
          }
        }
      }
    }
  }
} // End of namespace 'pcx'.
