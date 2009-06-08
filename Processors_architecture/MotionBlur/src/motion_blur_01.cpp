/* motion_blur_01.cpp
 * Motion blur effect.
 * Implementation 1.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

/*****
 * 1. Used `int' type instead `float'.
 ****/

namespace motion_blur
{
  void apply_01( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
    for (int y = 0; y < h; ++y)
    {
      for (int x = 0; x < w; ++x)
      {
        int const idx = y * scanlineLen + x * 4;
        
        // Preparing output pixel.
        pixel_type &imagePixel = *reinterpret_cast<pixel_type *>(image + idx);
        
        pixel_type const &lastLayerMovingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[nMovingLayers - 1] + idx);
        if (lastLayerMovingPixel.a != 0)
        {
          // Current pixel contains 100% of last moving layer pixel.
          
          imagePixel.r = lastLayerMovingPixel.r;
          imagePixel.g = lastLayerMovingPixel.g;
          imagePixel.b = lastLayerMovingPixel.b;
        }
        else
        {
          // Loading background pixel.
          pixel_type const &backgroundPixel = *reinterpret_cast<pixel_type const *>(background + idx);
          
          int
            totalR = backgroundPixel.r,
            totalG = backgroundPixel.g,
            totalB = backgroundPixel.b;
          
          // Applying all layers.
          for (int i = 0; i < nMovingLayers - 1; ++i)
          {
            pixel_type const &movingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[i] + idx);
            
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
          }
          
          // Writing final color.
          imagePixel.r = static_cast<byte_type>(totalR / nMovingLayers);
          imagePixel.g = static_cast<byte_type>(totalG / nMovingLayers);
          imagePixel.b = static_cast<byte_type>(totalB / nMovingLayers);
        }
      }
    }
  }
} // End of namespace 'motion_blur'.
