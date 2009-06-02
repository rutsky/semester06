/* main.cpp
 * PCX fast decoding.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 02.06.2009
 */

#include <iostream>
#include <fstream>
#include <memory>
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
    ifs.open(fileName, std::ios::in | std::ios::binary);
    if (ifs.fail())
    {
      std::cerr << "Failed to open `" << fileName << "'." << std::endl;
      return 1;
    }
  }
  
  // Reading PCX file contents.
  std::vector<unsigned char> inputData(inputSize, 0);
  {
    // Determining input file size.
    ifs.seekg(0, std::ios::end);
    std::streamsize const inputSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    
    // Reading.
    ifs.read(reinterpret_cast<unsigned char *>(&buf[0]), inputSize);
    assert(ifs.gcount() == inputSize);
  }
  
  std::cout << "Loaded `" << inputFileName << "' file.\n";
  std::cout << inputData.size() << " bytes length." << std::endl;

  // Initializing PCX header.
  pcx::PCXFileHeader header;
  assert(sizeof() == 128); // debug
  if (inputData.size() >= sizeof(header))
    memcpy(reinterpret_cast<unsigned char>(&header), &(inputData[0]), sizeof(header));
  else
  {
    // Input file too small to be correct.
    std::cerr << "Error: input file too small to include PCX header!" << std::endl;
    return 2;
  }
  
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
