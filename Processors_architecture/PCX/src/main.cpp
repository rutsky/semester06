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
  std::vector<unsigned char> inputData;
  {
    // Determining input file size.
    ifs.seekg(0, std::ios::end);
    std::streamsize const inputSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    inputData.resize(inputSize);
    
    // Reading.
    ifs.read(reinterpret_cast<char *>(&inputData[0]), inputSize);
    assert(ifs.gcount() == inputSize);
  }
  
  std::cout << "Loaded `" << inputFileName << "' file, "
    << inputData.size() << " bytes length." << std::endl;

  // Initializing PCX header.
  pcx::PCXFileHeader header;
  std::cout << sizeof(header) << std::endl; // debug
  assert(sizeof(header) == 128); // debug
  if (inputData.size() >= sizeof(header))
    memcpy(reinterpret_cast<void *>(&header), reinterpret_cast<void *>(&(inputData[0])), sizeof(header));
  else
  {
    // Input file too small to be correct.
    std::cerr << "Error: input file too small to include PCX header!" << std::endl;
    return 2;
  }
  
  std::cout << "PCX image header info:\n";
  std::cout << "  Manufacturer:     " << (int)header.manufacturer << "\n";
  std::cout << "  Version:          " << (int)header.version << "\n";
  std::cout << "  Encoding:         " << (int)header.encoding << "\n";
  std::cout << "  Bits per pixel:   " << (int)header.bpp << "\n";
  std::cout << "  Window:           " << "from (" << (int)header.xMin << "," << (int)header.yMin << ") to (" 
                                                  << (int)header.xMax << "," << (int)header.yMax << ")" << "\n";
  std::cout << "  Resolution (dpi): " << "(" << (int)header.hdpi << "," << (int)header.vdpi << ")" << "\n";
  std::cout << "  Planes:           " << (int)header.nPlanes << "\n";
  std::cout << "  Bytes per line:   " << (int)header.bytesPerLine << "\n";
  std::cout << "  Pallete type:     " << (int)header.palleteType << "\n";
  std::cout << "  Screen size:      " << "(" << (int)header.screenWidth << "," << (int)header.screenHeight << ")" << "\n";
}
