/* dummy_output.cpp
 * Motion blur effect.
 * Dummy output.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 08.06.2009
 */

#include "motion_blur.h"

/*****
 * Dummy background output.
 *****/

namespace motion_blur
{
  void apply_dummy( 
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers )
  {
    memcpy(reinterpret_cast<void *>(image), reinterpret_cast<void const *>(background), scanlineLen * h);
  }
} // End of namespace 'motion_blur'.
