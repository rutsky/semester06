/* motion_blur_06.cpp
 * Motion blur effect.
 * Implementation 6.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

/*****
 * 6. Using MMX and SSE.
 ****/

namespace motion_blur
{
  void apply_06( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
    // Calculating `1/nMovingLayers'.
    dword_type invNMovingLayers = (1 << 16) / nMovingLayers;
    
    // Storing `1/nMovingLayers' into mm5.
    asm("movd    mm5, %[var]": : [var]"r"(invNMovingLayers));
    // Cloning `1/nMovingLayers' into 4 mm5 words.
    asm("pshufw  mm5, mm5, 0": : );
  
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
          // mm0 = 0
          asm("pxor      mm0, mm0": : );
          // Loading background pixel into mm0 (packed as words).
          asm("punpcklbw mm0, DWORD PTR [%[addr]]": : [addr]"r"(background + idx));
          
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
              // mm2 = 0
              asm("pxor      mm2, mm2": : );
              // Unpacking bytes to words.
              asm("punpcklbw mm1, mm2": : );
              
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
          
          // mm4 = mm4 * (1/nMovingLayers)
          asm("pmullw   mm4, mm5": : );
          // Packing result with saturation.
          asm("packuswb mm4, mm7": : );
          
          // Saving result pixels.
          asm("movd     mm4, DWORD PTR [%[addr]]": : [addr]"r"(image + idx));
        }
      }
    }
    
    // Restoring FPU registers.
    asm("emms");
  }
} // End of namespace 'motion_blur'.
