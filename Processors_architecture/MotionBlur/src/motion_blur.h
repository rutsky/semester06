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
  
  void apply_dummy(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
  
  void apply(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );

  void apply_01(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
  
  void apply_02(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
  
  void apply_03(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
  
  void apply_04(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );
  
  void apply_06(
          byte_type *image, int w, int h, int scanlineLen,
          byte_type const *background,
          int nMovingLayers, byte_type const *const *movingLayers );

  extern "C"
  {
  #ifndef WIN32
    extern
    void _motion_blur_apply_dummy_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers );
  #else // WIN32
    static
    void cdecl motion_blur_apply_dummy_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers ) {};
  #endif // WIN32

  #ifndef WIN32
    static
    void _motion_blur_apply_dummy_32(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers ) {};
  #else // WIN32
    extern
    void cdecl motion_blur_apply_dummy_32(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers );
  #endif // WIN32

  #ifndef WIN32
    extern
    void _motion_blur_apply_05_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers );
  #else // WIN32
    static
    void cdecl motion_blur_apply_05_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers ) {};
  #endif // WIN32

  #ifndef WIN32
    static
    void _motion_blur_apply_05_32(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers ) {};
  #else // WIN32
    extern
    void cdecl motion_blur_apply_05_32(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers );
  #endif // WIN32

  #ifndef WIN32
    extern
    void _motion_blur_apply_07_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers );
  #else // WIN32
    static
    void cdecl motion_blur_apply_07_64(
            byte_type *image, int w, int h, int scanlineLen,
            byte_type const *background,
            int nMovingLayers, byte_type const *const *movingLayers ) {};
  #endif // WIN32
  }
} // End of namespace `motion_blur'.

#endif // MOTION_BLUR_H
