/* motion_blur.cpp
 * Motion blur effect.
 * Base implementation.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

namespace motion_blur
{
  void apply( 
          byte_type *image, size_t w, size_t h, size_t scanlineLen,
          byte_type const *background,
          size_t nMovingLayers, byte_type const *const *movingLayers )
  {
    // Assume that pixel format is A[RGB], 
    // i.e. (pixel & 0xff000000) is always alpha, order of color components not matters.
  
    for (size_t y = 0; y < h; ++y)
    {
      for (size_t x = 0; x < w; ++x)
      {
        size_t const idx = y * scanlineLen + x * 4;
        
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
          
          float
            totalR = backgroundPixel.r,
            totalG = backgroundPixel.g,
            totalB = backgroundPixel.b;
          
          // Applying all layers.
          for (size_t i = 0; i < nMovingLayers - 1; ++i)
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
