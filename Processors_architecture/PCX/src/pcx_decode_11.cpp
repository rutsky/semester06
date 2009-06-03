/* pcx_decode_11.cpp
 * PCX fast decoding routine.
 * Implementation #11.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

namespace pcx
{
  void decode_11( unsigned char const *input, size_t size,
                 size_t width, size_t height,
                 unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
  
    while (input != inputEnd && image != imageEnd)
    {
      while (image < imageEnd - 8)
      {
        unsigned int word1 = *((unsigned int const *)(input + 0));
        unsigned int word2 = *((unsigned int const *)(input + 4));
      
        if (!((word1 | word2) & 0xC0C0C0C0))
        {
          *((unsigned int *)(image + 0)) = word1;
          *((unsigned int *)(image + 4)) = word2;
          input += 8;
          image += 8;
        }
        else
        {
          if (!(word1 & 0xC0C0C0C0))
          {
            *((unsigned int *)(image + 0)) = word1;
            input += 4;
            image += 4;
          }
          break;
        }
      }
    
      unsigned char byte = *input++;
      
      if ((byte & 0xC0) == 0xC0) // 0xC0 = 2#11000000
      {
        int count = (byte & 0x3F); // 0x3F = 2#00111111
        if (input == inputEnd)
          break;
        
        byte = *input++;
        
        if (count != 1)
        {
          if (image + count > imageEnd)
          {
            count = imageEnd - image;
            
            for (; count & 0x3; --count)
              *image++ = byte;
            
            for (; count > 0; count -= 4)
            {
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
            }
            break;
          }
          else
          {
            for (; count & 0x3; --count)
              *image++ = byte;
            
            for (; count > 0; count -= 4)
            {
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
              *image++ = byte;
            }
              
            continue;
          }
        }
      }

      *image++ = byte;
    }
  }
} // End of namespace 'pcx'.
