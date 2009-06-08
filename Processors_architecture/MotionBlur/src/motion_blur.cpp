/* motion_blur.cpp
 * Motion blur effect.
 * Base implementation.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

#include <cmath>

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
          
          double
            totalR = backgroundPixel.r,
            totalG = backgroundPixel.g,
            totalB = backgroundPixel.b;
          
          // Applying all layers.
          for (size_t i = 0; i < nMovingLayers - 1; ++i)
          {
            pixel_type const &movingPixel = *reinterpret_cast<pixel_type const *>(movingLayers[i] + idx);
            
            double const coef = 0.5;
            
            if (movingPixel.a != 0)
            {
              double const
                r = movingPixel.r,
                g = movingPixel.g,
                b = movingPixel.b;
            
              //double const p = pow(2., -(nMovingLayers - 1. - i));
              //double const p = 0.5;
              
              /*
              totalR = (totalR + r) / 2.;
              totalG = (totalG + g) / 2.;
              totalB = (totalB + b) / 2.;
              */
              /*
              totalR = 3./4. * totalR + 1./4. * r;
              totalG = 3./4. * totalG + 1./4. * g;
              totalB = 3./4. * totalB + 1./4. * b;
              */
              /*
              totalR = (1. * totalR + p * r) / (1 + p);
              totalG = (1. * totalG + p * g) / (1 + p);
              totalB = (1. * totalB + p * b) / (1 + p);
              */
              /*
              totalR = (1. - p) * totalR + p * r;
              totalG = (1. - p) * totalG + p * g;
              totalB = (1. - p) * totalB + p * b;
              */
              
              totalR = coef * totalR + (1. - coef) * movingPixel.r;
              totalG = coef * totalG + (1. - coef) * movingPixel.g;
              totalB = coef * totalB + (1. - coef) * movingPixel.b;
            }
            else
            {
              totalR = coef * totalR + (1. - coef) * backgroundPixel.r;
              totalG = coef * totalG + (1. - coef) * backgroundPixel.g;
              totalB = coef * totalB + (1. - coef) * backgroundPixel.b;
            }
          }
          
          // Writing final color.
          imagePixel.r = static_cast<byte_type>(totalR);
          imagePixel.g = static_cast<byte_type>(totalG);
          imagePixel.b = static_cast<byte_type>(totalB);
        }
      }
    }
  }
} // End of namespace 'motion_blur'.
