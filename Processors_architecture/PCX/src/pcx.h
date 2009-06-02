/* pcx.h
 * PCX file format stuff.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <cstdlib>

namespace pcx
{
#pragma pack(push, 1)
  struct PCXFileHeader
  {
    typedef unsigned char  byte_type;
    typedef unsigned short word_type;
  
    // Offset 0, size 1.
    // Constant Flag, 10 for ZSoft .PCX files.
    byte_type manufacturer;
    
    // Offset 1, size 1.
    // Version information.
    //   0 for Version 2.5 of PC Paintbrush 
    //   2 for Version 2.8 w/palette information 
    //   3 for Version 2.8 w/o palette information 
    //   4 for PC Paintbrush for Windows(Plus for Windows uses Ver 5)
    //   5 for Version 3.0 and grater of PC Paintbrush and PC Paintbrush . Includes 24-bit .PCX files.
    byte_type version;
    
    // Offset 2, size 1.
    // 1 for .PCX run length encoding.
    byte_type encoding;
    
    // Offset 3, size 1.
    // Number of bits to represent a pixel (per Plane) -- 1, 2, 4, or 8.
    byte_type bpp;
    
    // Offset 4, size 8.
    // Window -- image dimensions.
    word_type xMin, yMin, xMax, yMax;
 
    // Offset 12, size 4.
    // Resolution of image in DPI.
    word_type hdpi, vdpi;
    
    // Offset 16, size 48.
    //Color palette setting.
    byte_type colormap[48];
    
    // Offset 64, size 1.
    // Reserved. Should be set to 0.
    byte_type reserved_64_1;
    
    // Offset 65, size 1.
    // Number of color planes.
    byte_type nPlanes;

    // Offset 66, size 2.
    // Number of bytes to allocate for a scanline plane. 
    // Must be an even number. Not equal to xMax - xMin.
    word_type bytesPerLine;
    
    // Offset 68, size 2.
    // How to interpret palette: 1 = Color/BW,
    //                           2 = Grayscale.
    word_type palleteType;
    
    // Offset 70, size 4.
    // Screen size in pixels. New field found only in PB IV/IV Plus.
    word_type screenWidth, screenHeight;

    // Offset 74, size 54.
    // Blank to fill out 128 byte header. Set all bytes to 0.
    byte_type fill_74_54[54];
  };
#pragma pack(pop)

  // See implementation for details.
  void decode( unsigned char const *input, size_t size,
               size_t width, size_t height,
               unsigned char *image );
  
  void decode_1(
      unsigned char const *input, size_t size,
      size_t width, size_t height,
      unsigned char *image );
  
  void decode_2(
      unsigned char const *input, size_t size,
      size_t width, size_t height,
      unsigned char *image );

  void decode_3(
      unsigned char const *input, size_t size,
      size_t width, size_t height,
      unsigned char *image );

  void decode_4(
      unsigned char const *input, size_t size,
      size_t width, size_t height,
      unsigned char *image );

  void decode_5(
      unsigned char const *input, size_t size,
      size_t width, size_t height,
      unsigned char *image );
} // End of namespace 'pcx'.
