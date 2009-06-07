/* main.cpp
 * PCX fast decoding.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <iostream>
#include <fstream>
#include <istream>
#include <memory>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#ifdef WIN32
  #include "mtimer.hpp"
#else
  #include "SDL.h"
#endif

#include "pcx.h"

// TODO: Divivde `main()' on subroutines.

template< typename T, size_t N >
size_t array_size( T (&)[N] ) { return N; }

// The main program function.
int main( int argc, char *argv[] )
{
  using pcx::byte_type;

  //char const *inputFileName = "../data/baboon4.pcx";
  //char const *outputFileName = "../data/baboon4.ppm";
  char const *inputFileName = "../data/square_2x2.pcx";
  char const *outputFileName = "../data/square_2x2.ppm";
  size_t nTries = 10; // TODO: Set to 1000.
  size_t nTotalTries = 3;
  
  if (argc >= 2)
    inputFileName = argv[1];
  if (argc >= 3)
    outputFileName = argv[2];
  if (argc >= 4)
  {
    std::string const str(argv[3]);
    std::istringstream istr(str);
    int x;
    istr >> x;
    nTries = x;
  }
  if (argc >= 5)
  {
    std::string str(argv[4]);
    std::istringstream istr(str);
    int x;
    istr >> x;
    nTotalTries = x;
  }
  
  std::vector<byte_type> inputData;
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
  std::vector<byte_type> image;
  size_t width, height, nPlanes;
  {
    if (header.xMin > header.xMax || header.yMin > header.yMax)
    {
      std::cerr << "Error: incorrect image size!" << std::endl;
      return 4;
    }
    width  = header.xMax - header.xMin + 1;
    height = header.yMax - header.yMin + 1;
    
    nPlanes = header.nPlanes;
    
    // Allocating result image.
    image.resize(height * width * nPlanes);
    
    // Decoding.
    pcx::decode(&(inputData[headerSize]), dataSize, width, height, &(image[0]));

    // Optimized decoding timing.
    size_t const nAdditionalInput = 1024, nAdditionalOutput = 1024;
    std::vector<byte_type> testInput(inputData.size() + nAdditionalInput, byte_type(0));
    std::copy(inputData.begin(), inputData.end(), testInput.begin());
    std::vector<byte_type> testImage(height * width * nPlanes + nAdditionalOutput, byte_type(0));
    
    {
      typedef void (*decode_func_ptr_type)( byte_type const *, size_t, size_t, size_t, byte_type *);
      
      decode_func_ptr_type decodeFuncs[] = 
        { 
          &pcx::decode,
          &pcx::decode_01,
          &pcx::decode_02,
          &pcx::decode_03,
          &pcx::decode_04,
          &pcx::decode_05,
          &pcx::decode_06,
          &pcx::decode_07,
          &pcx::decode_08,
          &pcx::decode_09,
          &pcx::decode_09a,
          &pcx::decode_10,
          &pcx::decode_11,
          &pcx::decode_11a,
          &pcx::decode_11b,
          &pcx::decode_12,
          &pcx::decode_13,
          &pcx::decode_13a,
          &pcx::decode_14,
        };
      
      char const *decodeFuncsNames    [] = 
        {
          "general",
          "# 1  w/o iostream",
          "# 2  constants inlined",
          "# 3  using `int' type",
          "# 4  deduced output image addressing",
          "# 5  deduced input data addressing",
          "# 6  reimplemented cycle",
          "# 7  inlined outputting",
          "# 8  inlined outputting in all cases",
          "# 9  pairing in output",
          "# 9a using memset() for output",
          "#10  I/O by DWORD",
          "#11  I/O by paired 2 DWORDs",
          "#11a I/O by blindly paired 2 DWORDs",
          "#11b I/O by blindly paired 4 DWORDs",
          "#12  reading aligned by 4 bytes",
          "#13  I/O by blindly paired 2 DWORDs using MMX",
          "#13a I/O by blindly paired 2 DWORDs using MMX (only copying with MMX)",
          "14   I/O by blindly paired 2 DWORDs using MMX + output using SSE.",
        };
      assert(array_size(decodeFuncs) == array_size(decodeFuncsNames));

      size_t const nImplementations = array_size(decodeFuncs);

      double results[nImplementations][nTotalTries + 2]; // `... + 2' because we skipping first and last runs.
      
      std::cout << "Starting " << nTotalTries << " runs by " << nTries << " calls..." << std::endl;
      for (size_t totalTry = 0; totalTry < nTotalTries + 2; ++totalTry)
      {
        for (size_t funcIdx = 0; funcIdx < nImplementations; ++funcIdx)
        {
          // Locating each implementation running time.
          
          decode_func_ptr_type const decodeFunc = decodeFuncs[funcIdx];
          
        #ifdef WIN32
          Timer timer;
          double const startTime = timer.GetTime();
        #else
          Uint32 const startTics = SDL_GetTicks();
        #endif
          for (size_t i = 0; i < nTries; ++i)
          {
            decodeFunc(&(testInput[headerSize]), dataSize, width, height, &(testImage[0]));
          }
        #ifdef WIN32
          double const tryTime = timer.GetTime() - startTime;
        #else
          Uint32 const endTics = SDL_GetTicks();
          double const tryTime = (double)(endTics - startTics) / 1000.0;
        #endif
          results[funcIdx][totalTry] = tryTime;
          
          // Verifying generated image.
          if (!std::equal(image.begin(), image.end(), testImage.begin()))
          {
            std::cout << "Error! Implementation `" << decodeFuncsNames[funcIdx] << "' incorectly decoded image." << std::endl;
          }
        }
      }
      
      // Final report.
      std::cout << "Average result based on " << nTotalTries << " runs by " << nTries << " calls." << std::endl;
      for (size_t funcIdx = 0; funcIdx < nImplementations; ++funcIdx)
      {
        // Locating each implementation running time.
        
        char const *decodeFuncName = decodeFuncsNames[funcIdx];
        
        std::cout << "Implementation: " << decodeFuncName << std::endl;
        
        double minTime = results[funcIdx][1], maxTime = results[funcIdx][1];
        double runTime = 0;
        for (size_t i = 1; i <= nTotalTries; ++i)
        {
          runTime += results[funcIdx][i];
          if (minTime > results[funcIdx][i])
            minTime = results[funcIdx][i];
          if (maxTime < results[funcIdx][i])
            maxTime = results[funcIdx][i];
        }
        
        double const timePerTotalTry = runTime / nTotalTries;
        
        std::cout << nTotalTries << " * " << nTries << " calls in " << std::setw(9) << std::left << runTime << " seconds " 
            << "(min="  << std::setw(9) << std::left << minTime << " max=" << std::setw(9) << std::left << maxTime 
            << " avg=" << std::setw(9) << std::left << timePerTotalTry << " seconds per " << nTries << " calls)\n" << std::endl;
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

  return 0;
}
