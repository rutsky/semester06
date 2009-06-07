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
        
        pixel_type const &backgroundPixel = *reinterpret_cast<pixel_type const *>(background + idx);
        float 
          totalR = backgroundPixel.r,
          totalG = backgroundPixel.g,
          totalB = backgroundPixel.b;
        
        for (size_t i = 0; i < nMovingLayers; ++i)
        {
          pixel_type const &movingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[i] + idx);
          
          if (movingPixel.a != 0)
          {
            float const
              r = backgroundPixel.r,
              g = backgroundPixel.g,
              b = backgroundPixel.b;
          
            totalR = (totalR + r) / 2.;
            totalG = (totalG + g) / 2.;
            totalB = (totalB + b) / 2.;
          }
        }
        
        pixel_type &imagePixel = *reinterpret_cast<pixel_type *>(image + idx);
        imagePixel.r = static_cast<byte_type>(totalR);
        imagePixel.g = static_cast<byte_type>(totalG);
        imagePixel.b = static_cast<byte_type>(totalB);
      }
    }
  }
} // End of namespace 'motion_blur'.
