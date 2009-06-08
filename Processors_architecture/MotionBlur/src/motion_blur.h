/* motion_blur.h
 * Motion blur effects implementations.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */


#ifndef MOTION_BLUR_H
#define MOTION_BLUR_H

#include <cstddef>
#include <SDL.h>

namespace motion_blur
{
  typedef Uint8  byte_type;
  typedef Uint16 word_type;
  typedef Uint32 dword_type;
  
  struct pixel_type
  {
    byte_type r, g, b, a;
  };
  
  void apply(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );

  void apply_01(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
} // End of namespace `motion_blur'.

#endif // MOTION_BLUR_H
