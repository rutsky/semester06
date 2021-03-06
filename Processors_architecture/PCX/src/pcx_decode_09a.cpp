/* pcx_decode_09a.cpp
 * PCX fast decoding routine.
 * Implementation #9a.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #9a. Using `memset()' for outputting.
//

#include <cstring>

namespace pcx
{
  void decode_09a( unsigned char const *input, size_t size,
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
        
        if (count != 1)
        {
          if (image + count > imageEnd)
          {
            count = imageEnd - image;
            
            memset(image, static_cast<int>(byte), count);
            
            break;
          }
          else
          {
            memset(image, static_cast<int>(byte), count);
            image += count;
              
            continue;
          }
        }
      }

      *image++ = byte;
    }
  }
} // End of namespace 'pcx'.
