/* motion_blur_06.cpp
 * Motion blur effect.
 * Implementation 6.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

/*****
 * 6. Using MMX and SSE. GCC specific version.
 ****/

namespace motion_blur
{
#ifndef WIN32
  void apply_06( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
    // Calculating `1/nMovingLayers'.
    dword_type invNMovingLayers = (1 << 8) / nMovingLayers;
    
    // Storing `1/nMovingLayers' into mm5.
    asm("movd    mm5, %[var]": : [var]"r"(invNMovingLayers));
    // Cloning `1/nMovingLayers' into 4 mm5 words.
    asm("pshufw  mm5, mm5, 0": : );

    // Storing zeroes at mm6.
    static char const zeroes[8] = {0};
    asm("movq    mm6, QWORD PTR [%[addr]]": : [addr]"r"(zeroes));
    
    // Storing mask.
    static char const mask[8] = {0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00};
    asm("movq    mm7, QWORD PTR [%[addr]]": : [addr]"r"(mask));
  
    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        int const idx = y * scanlineLen + x * 4;
        
        dword_type const lastMovingLayerDword = *reinterpret_cast<dword_type const *>(movingLayers[nMovingLayers - 1] + idx);
        if (lastMovingLayerDword & 0xff000000)
          *reinterpret_cast<dword_type *>(image + idx) = lastMovingLayerDword;
        else
        {
          // Loading background pixel into mm0 (packed as words).
          asm("movd      mm0, DWORD PTR [%[addr]]": : [addr]"r"(background + idx));
          asm("punpcklbw mm0, mm6": : );
          
          // mm4 will accumulate color sum.
          asm("movq      mm4, mm0": : );
          
          int i = nMovingLayers - 1;
          do
          {
            dword_type const movingLayerDword = *reinterpret_cast<dword_type const *>(movingLayers[i - 1] + idx);
            
            if (movingLayerDword & 0xff000000)
            {
              // Loading moving layer pixel into mm1.
              asm("movd      mm1, %[var]": : [var]"r"(movingLayerDword));
              // Unpacking bytes to words.
              asm("punpcklbw mm1, mm6": : );
              
              // mm4 += mm1
              asm("paddsw    mm4, mm1": : );
            }
            else
            {
              // mm4 += mm0
              asm("paddsw    mm4, mm0": : );
            }
            
            --i;
          } while (i != 0);
          
          // mm4 = mm4 * ((1 << 8)/nMovingLayers)
          asm("pmullw   mm4, mm5": : );
          // mm4 = mm4 >> 8
          asm("psrlw    mm4, 8": : );
          // mm4 = mm4 & 0xff
          asm("pand     mm4, mm7": : );
          // Packing result with saturation.
          asm("packuswb mm4, mm7": : );
          
          // Saving result pixels.
          asm("movd     DWORD PTR [%[addr]], mm4": : [addr]"r"(image + idx));
        }
      }
    }
    
    // Restoring FPU registers.
    asm("emms");
  }
#else // WIN32
  void apply_06( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
  }
#endif // !WIN32
} // End of namespace 'motion_blur'.
