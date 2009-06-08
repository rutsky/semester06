/* motion_blur_03.cpp
 * Motion blur effect.
 * Implementation 3.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

/*****
 * 3. Optimized cycle.
 ****/

namespace motion_blur
{
  void apply_03( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        int const idx = y * scanlineLen + x * 4;
        
        pixel_type &imagePixel = *reinterpret_cast<pixel_type *>(image + idx);
        
        pixel_type const &lastLayerMovingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[nMovingLayers - 1] + idx);
        if (lastLayerMovingPixel.a != 0)
          imagePixel = lastLayerMovingPixel;
        else
        {
          pixel_type const &backgroundPixel = *reinterpret_cast<pixel_type const *>(background + idx);
          
          int
            totalR = backgroundPixel.r,
            totalG = backgroundPixel.g,
            totalB = backgroundPixel.b;
          
          int i = nMovingLayers - 1;
          do
          {
            pixel_type const &movingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[i - 1] + idx);
            
            if (movingPixel.a != 0)
            {
              totalR += movingPixel.r;
              totalG += movingPixel.g;
              totalB += movingPixel.b;
            }
            else
            {
              totalR += backgroundPixel.r;
              totalG += backgroundPixel.g;
              totalB += backgroundPixel.b;
            }
            --i;
          } while (i != 0);
          
          imagePixel.r = static_cast<byte_type>(totalR / nMovingLayers);
          imagePixel.g = static_cast<byte_type>(totalG / nMovingLayers);
          imagePixel.b = static_cast<byte_type>(totalB / nMovingLayers);
        }
      }
    }
  }
} // End of namespace 'motion_blur'.
