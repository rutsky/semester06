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

#include "pcx.h"

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
  {
    assert(sizeof(header) == 128); // debug
    if (inputData.size() >= sizeof(header))
      memcpy(reinterpret_cast<void *>(&header), reinterpret_cast<void *>(&(inputData[0])), sizeof(header));
    else
    {
      // Input file too small to be correct.
      std::cerr << "Error: input file too small to include PCX header!" << std::endl;
      return 2;
    }
  }
  
  // Outputing information.
  {
    std::cout << "PCX image header info:\n";
    std::cout << "  Manufacturer:     " << header.manufacturer << "\n";
    std::cout << "  Version:          " << header.version << "\n";
    std::cout << "  Encoding:         " << header.encoding << "\n";
    std::cout << "  Bits per pixel:   " << header.bpp << "\n";
    std::cout << "  Window:           " << "from (" << header.xMin << "," << header.yMin << ") to (" << header.xMax << "," << header.yMax << ")" << "\n";
    std::cout << "  Resolution (dpi): " << "(" << header.hdpi << "," << header.vdpi << ")" << "\n";
    std::cout << "  Colormap:         " << header.colormap << "\n";
    std::cout << "  Planes:           " << header.nPlanes << "\n";
    std::cout << "  Bytes per line:   " << header.bytesPerLine << "\n";
    std::cout << "  Pallete type:     " << header.palleteType << "\n";
    std::cout << "  Screen size:      " << "(" << header.screenWidth << "," << header.screenHeight << ")" << "\n";
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
  size_t width, height;
  {
    if (header.xMin >= header.xMax || header.yMin >= header.yMax)
    {
      std::cerr << "Error: incorrect image size!" << std::endl;
      return 4;
    }
    width  = header.xMax - header.xMin;
    height = header.yMax - header.yMin;
    
    if ((inputData.size() - sizeof(header)) / (height * header.nPlanes) < header.bytesPerLine)
    {
      std::cerr << "Error: incorrect bytes per line value!" << std::endl;
      return 5;
    }
    size_t const bytesPerLine = header.bytesPerLine;
    
    // Allocating result image.
    image.resize(height * width * header.nPlanes);
    
    // Decoding.
    pcx::decode(&(inputData[0]), width, height, bytesPerLine, &(image[0]));
  }
}
