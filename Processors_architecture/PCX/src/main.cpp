/* main.cpp
 * PCX fast decoding.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include <cstring>
#include <vector>

#include "SDL.h"

#include "pcx.h"

// TODO: Divivde `main()' on subroutines.

/* The main program function */
int main( int argc, char *argv[] )
{
  char const *inputFileName = "../data/baboon.pcx";
  char const *outputFileName = "../data/baboon.ppm";
  if (argc >= 2)
    inputFileName = argv[1];
  if (argc >= 3)
    outputFileName = argv[2];
  
  std::vector<unsigned char> inputData;
  {
    // Opening input PCX file.
    std::ifstream ifs;
    {
      ifs.open(inputFileName, std::ios::in | std::ios::binary);
      if (ifs.fail())
      {
        std::cerr << "Failed to open `" << inputFileName << "'." << std::endl;
        return 1;
      }
    }
    
    // Reading PCX file contents.
    {
      // Determining input file size.
      ifs.seekg(0, std::ios::end);
      std::streamsize const inputSize = ifs.tellg();
      ifs.seekg(0, std::ios::beg);
      
      // Resing data buffer.
      inputData.resize(inputSize, 0);
      
      // Reading.
      ifs.read(reinterpret_cast<char *>(&(inputData[0])), inputSize);
      assert(ifs.gcount() == inputSize);
    }
  }
  
  std::cout << "Loaded `" << inputFileName << "' file.\n";
  std::cout << inputData.size() << " bytes length." << std::endl;

  // Initializing PCX header.
  pcx::PCXFileHeader header;
  size_t const headerSize = sizeof(header);
  {
    assert(headerSize == 128); // debug
    if (inputData.size() >= headerSize)
      memcpy(reinterpret_cast<void *>(&header), reinterpret_cast<void *>(&(inputData[0])), headerSize);
    else
    {
      // Input file too small to be correct.
      std::cerr << "Error: input file too small to include PCX header!" << std::endl;
      return 2;
    }
  }
  size_t const dataSize = inputData.size() - headerSize;
  
  // Outputing information.
  {
    std::cout << "PCX image header info:\n";
    std::cout << "  Manufacturer:     " << (int)header.manufacturer << "\n";
    std::cout << "  Version:          " << (int)header.version << "\n";
    std::cout << "  Encoding:         " << (int)header.encoding << "\n";
    std::cout << "  Bits per pixel:   " << (int)header.bpp << "\n";
    std::cout << "  Window:           " << "from (" << (int)header.xMin << "," << (int)header.yMin << ") to (" << 
                                                       (int)header.xMax << "," << (int)header.yMax << ")" << "\n";
    std::cout << "  Resolution (dpi): " << "(" << (int)header.hdpi << "," << (int)header.vdpi << ")" << "\n";
    std::cout << "  Planes:           " << (int)header.nPlanes << "\n";
    std::cout << "  Bytes per line:   " << (int)header.bytesPerLine << "\n";
    std::cout << "  Pallete type:     " << (int)header.palleteType << "\n";
    std::cout << "  Screen size:      " << "(" << (int)header.screenWidth << "," << (int)header.screenHeight << ")" << "\n";
  }
  
  if (!(header.manufacturer == 10 &&
        header.version      == 5  &&
        header.encoding     == 1  &&
        header.bpp          == 8  &&
        header.nPlanes      == 3  &&
        header.palleteType  == 1))
  {
    std::cerr << "Unsupported file format. See source for details." << std::endl;
    return 3;
  }
  
  // Decoding image.
  std::vector<unsigned char> image;
  size_t width, height, nPlanes;
  {
    if (header.xMin > header.xMax || header.yMin > header.yMax)
    {
      std::cerr << "Error: incorrect image size!" << std::endl;
      return 4;
    }
    width  = header.xMax - header.xMin + 1;
    height = header.yMax - header.yMin + 1;
    
    if (dataSize / (height * header.nPlanes) < header.bytesPerLine)
    {
      std::cerr << "Error: incorrect bytes per line value!" << std::endl;
      return 5;
    }
    
    nPlanes = header.nPlanes;
    
    // Allocating result image.
    image.resize(height * width * nPlanes);
    
    // Decoding.
    {
      typedef void (*decode_func_ptr_type)( unsigned char const *, size_t, size_t, size_t, unsigned char *);
      
      decode_func_ptr_type decodeFuncs[] = 
        { 
          &pcx::decode,
          &pcx::decode_1,
          &pcx::decode_2,
          &pcx::decode_3,
          &pcx::decode_4,
          &pcx::decode_5,
        };
      size_t const nImplementations = sizeof(decodeFuncs) / sizeof(decodeFuncs[0]);
      
      char const *decodeFuncsNames    [nImplementations] = 
        {
          "general",
          "#1 w/o iostream",
          "#2 constants inlined",
          "#3 using `int' type",
          "#4 deduced output image addressing",
          "#5 deduced input fata addressing",
        };

      size_t const nTotalTries = 10;
      size_t const nTries = 10;
      Uint32 results[nImplementations][nTotalTries];
      
      for (size_t totalTry = 0; totalTry < nTotalTries; ++totalTry)
      {
        for (size_t funcIdx = 0; funcIdx < nImplementations; ++funcIdx)
        {
          // Locating each implementation running time.
          
          decode_func_ptr_type const decodeFunc = decodeFuncs[funcIdx];
          
          Uint32 const startTics = SDL_GetTicks();
          for (size_t i = 0; i < nTries; ++i)
          {
            decodeFunc(&(inputData[headerSize]), dataSize, width, height, &(image[0]));
          }
          Uint32 const endTics = SDL_GetTicks();
          
          results[funcIdx][totalTry] = endTics - startTics;
        }
      }
      
      // Final report.
      std::cout << "Average result based on " << nTotalTries << " runs by " << nTries << " calls." << std::endl;
      for (size_t funcIdx = 0; funcIdx < nImplementations; ++funcIdx)
      {
        // Locating each implementation running time.
        
        char const *decodeFuncName = decodeFuncsNames[funcIdx];
        
        std::cout << "Implementation: " << decodeFuncName << std::endl;
        
        Uint32 runTicks = 0;
        for (size_t i = 0; i < nTotalTries; ++i)
          runTicks += results[funcIdx][i];
        
        double const totalTime = (double)runTicks / ((double)nTotalTries * 1000.0);
        double const timePerTry = totalTime / nTries;
        
        std::cout << nTries << " calls in " << totalTime << " seconds (" << timePerTry << " seconds per try)." << std::endl;
      }
    }
  }
  
  // Saving decoded image as PPM.
  {
    // Opening output PPM file.
    std::ofstream ofs;
    {
      ofs.open(outputFileName, std::ios::out);
      if (ofs.fail())
      {
        std::cerr << "Failed to open `" << outputFileName << "'." << std::endl;
        return 1;
      }
    }
    
    // Writing header.
    ofs << "P3\n";  // P3 means that colors are in ASCII.
    ofs << "# Comment.\n";
    ofs << width << " " << height << "\n";
    ofs << "255\n"; // Colors are in range [0, 255].
    for (size_t y = 0; y < height; ++y)
      for (size_t x = 0; x < width; ++x)
      {
        ofs << (int)image[y * width * nPlanes + width * 0 + x];
        for (size_t p = 1; p < nPlanes; ++p)
          ofs << " " << (int)image[y * width * nPlanes + width * p + x];
        ofs << "\n";
      }
  }
}
