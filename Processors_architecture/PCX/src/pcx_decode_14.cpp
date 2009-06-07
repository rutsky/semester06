/* pcx_decode_14.cpp
 * PCX fast decoding routine.
 * Implementation #14.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.06.2009
 */

#include "pcx.h"

//
// #14. I/O by blindly paired 2 DWORDs using MMX + output using SSE.
//

#define DO_4_TIMES(expr) expr expr expr expr

namespace pcx
{
  void decode_14( unsigned char const *input, size_t size,
                  size_t width, size_t height,
                  unsigned char *image )
  {
    unsigned char const *imageEnd = image + height * 3 * width;
    unsigned char const *inputEnd = input + size;
    
    static const dword_type mask[8] = 
      { 
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
        
        // Copying packed result into variable.
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
        
          if (input >= inputEnd)
            goto exit; // Impossible on correct images.
        
          byte = *input++;
        
          if (count == 1)
            *image++ = byte;
          else
          {
            if (image + count > imageEnd)
            {
              count = imageEnd - image;
            
              switch (count & (8 - 1))
              {
                case 7:
                  *(image + 6) = byte;
                case 6:
                  *(image + 5) = byte;
                case 5:
                  *(image + 4) = byte;
                case 4:
                  *(image + 3) = byte;
                case 3:
                  *(image + 2) = byte;
                case 2:
                  *(image + 1) = byte;
                case 1:
                  *(image + 0) = byte;
              }
              
              image += count & (8 - 1);
              count &= ~(8 - 1);
                
              if (count != 0)
              {
                dword_type dword = byte;
                dword <<= 8;
                dword |= byte;
                
                // Loading word with 2 bytes to repeat into mm0 lower word.
                asm("movd    mm0, %[var]": : [var]"r"(dword));
                // Filling mm1 with 8 repeat bytes.
                asm("pshufw   mm1, mm0, 0": : );
                
                switch (count >> 3)
                {
                case 7:
                  asm("movq    QWORD PTR[%[outaddr] + 48], mm1": : [outaddr]"r"(image));
                case 6:
                  asm("movq    QWORD PTR[%[outaddr] + 40], mm1": : [outaddr]"r"(image));
                case 5:
                  asm("movq    QWORD PTR[%[outaddr] + 32], mm1": : [outaddr]"r"(image));
                case 4:
                  asm("movq    QWORD PTR[%[outaddr] + 24], mm1": : [outaddr]"r"(image));
                case 3:
                  asm("movq    QWORD PTR[%[outaddr] + 16], mm1": : [outaddr]"r"(image));
                case 2:
                  asm("movq    QWORD PTR[%[outaddr] +  8], mm1": : [outaddr]"r"(image));
                case 1:
                  asm("movq    QWORD PTR[%[outaddr] +  0], mm1": : [outaddr]"r"(image));
                }
                
                image += count;
              }
            
              goto exit;
            }
            else
            {
              switch (count & (8 - 1))
              {
                case 7:
                  *(image + 6) = byte;
                case 6:
                  *(image + 5) = byte;
                case 5:
                  *(image + 4) = byte;
                case 4:
                  *(image + 3) = byte;
                case 3:
                  *(image + 2) = byte;
                case 2:
                  *(image + 1) = byte;
                case 1:
                  *(image + 0) = byte;
              }
              
              image += count & (8 - 1);
              count &= ~(8 - 1);
                
              if (count != 0)
              {
                dword_type dword = byte;
                dword <<= 8;
                dword |= byte;
                
                // Loading word with 2 bytes to repeat into mm0 lower word.
                asm("movd    mm0, %[var]": : [var]"r"(dword));
                // Filling mm1 with 8 repeat bytes.
                asm("pshufw   mm1, mm0, 0": : );
                
                switch (count >> 3)
                {
                case 7:
                  asm("movq    QWORD PTR[%[outaddr] + 48], mm1": : [outaddr]"r"(image));
                case 6:
                  asm("movq    QWORD PTR[%[outaddr] + 40], mm1": : [outaddr]"r"(image));
                case 5:
                  asm("movq    QWORD PTR[%[outaddr] + 32], mm1": : [outaddr]"r"(image));
                case 4:
                  asm("movq    QWORD PTR[%[outaddr] + 24], mm1": : [outaddr]"r"(image));
                case 3:
                  asm("movq    QWORD PTR[%[outaddr] + 16], mm1": : [outaddr]"r"(image));
                case 2:
                  asm("movq    QWORD PTR[%[outaddr] +  8], mm1": : [outaddr]"r"(image));
                case 1:
                  asm("movq    QWORD PTR[%[outaddr] +  0], mm1": : [outaddr]"r"(image));
                }
                
                image += count;
              }
            }
          }
        }
        
        if (input >= inputEnd || image >= imageEnd)
          goto exit;
      }
      ); // End of 'DO_4_TIMES'.
    } while (true);
    
  exit:
    // Freeing FPU registers.
    asm("emms": : );
  }
} // End of namespace 'pcx'.
