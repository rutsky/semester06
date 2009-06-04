/* pcx_decode_13.cpp
 * PCX fast decoding routine.
 * Implementation #13.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include "pcx.h"

//
// #13. I/O by blindly paired 2 DWORDs using MMX.
//

#define DO_4_TIMES(expr) expr expr expr expr

namespace pcx
{
  void decode_13( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
    
    static const dword_type mask[16] = 
      { 
        0xC0, 0xC0, 0xC0, 0xC0,
        0xC0, 0xC0, 0xC0, 0xC0,
        0xC0, 0xC0, 0xC0, 0xC0,
        0xC0, 0xC0, 0xC0, 0xC0,
      };
    
    // Preloading mask into mm4.
    asm("movq    mm4, QWORD PTR [%[maskaddr]]": : [maskaddr] "r"(mask));

    do
    {
      while (image < imageEnd - 8) // We know, that input size must be enough for full image decoding.
      {
        // Reading 8 bytes from input to mm0.
        asm("movq     mm0, QWORD PTR [%[inaddr]]" : : [inaddr]  "r"(input));
        
        // Copying readed 8 bytes to mm1.
        asm("movq     mm1, mm0": : );
        // Bytewise `and' with mask.
        asm("pand     mm1, mm4": : );
        // Bytewise comparision.
        asm("pcmpeqb  mm1, mm4": : );
        // Packing 2 dwords of mm1 with signed saturation into mm1 lower dwords. Contents of mm5 not matters.
        asm("packsswb mm1, mm5": : );
        
        // Copying packing result into variable.
        dword_type check;
        asm volatile ("movd     %[var], mm1": [var] "=r"(check) : );
        
        if (check != 0)
          break;

        // Writing 8 bytes from mm0 to image.
        asm("movq     QWORD PTR [%[outaddr]], mm0": : [outaddr] "r"(image));
        
        input += 8;
        image += 8;
      }

      // 4 times -- as average needed in guess error with 8 bytes.
      DO_4_TIMES(
      {
        unsigned char byte = *input++;

        if ((byte & 0xC0) != 0xC0) // 0xC0 = 2#11000000
          *image++ = byte;
        else
        {
          int count = (byte & 0x3F); // 0x3F = 2#00111111
        
          if (input == inputEnd)
            return; // Impossible on correct images.
        
          byte = *input++;
        
          if (count == 1)
            *image++ = byte;
          else
          {
            if (image + count > imageEnd)
            {
              count = imageEnd - image;
            
              for (; count & 0x3; --count)
                *image++ = byte;
            
              for (; count > 0; count -= 4)
              {
                *(image + 0) = byte;
                *(image + 1) = byte;
                *(image + 2) = byte;
                *(image + 3) = byte;
                image += 4;
              }
            
              return;
            }
            else
            {
              for (; count & 0x3; --count)
                *image++ = byte;
            
              for (; count > 0; count -= 4)
              {
                *(image + 0) = byte;
                *(image + 1) = byte;
                *(image + 2) = byte;
                *(image + 3) = byte;
                image += 4;
              }
            }
          }
        }
        
        if (input == inputEnd || image == imageEnd)
          return;
      }
      ); // End of 'DO_4_TIMES'.
    } while (true);
  }
} // End of namespace 'pcx'.
