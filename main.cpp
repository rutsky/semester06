/*
 * main.cpp
 * Utility to pack Oracom's OB 890 firwmware.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.03.2009
 */

#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

extern "C" 
{
#include "telechips.h"
}

typedef std::vector<unsigned char> buffer_type;

bool correctFWHeader( char const *inFileName, char const *outFileName )
{
  std::fstream ifs(inFileName, std::ios::in | std::ios::binary);
  std::fstream ofs(outFileName, std::ios::out | std::ios::binary);
  
  std::cout << "'" << inFileName << "' -> '" << outFileName << "'\n"; // debug
  
  // TODO: Work with abstract streams.
 
  if (ifs && ofs)
  {
    // Determining input file size.
    ifs.seekg(0, std::ios::end);
    std::streamsize const inputSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
  
    // Reading firmware into buffer.
    buffer_type buf(inputSize, 0);

    std::cout << "Reading " << inputSize << " bytes" << std::endl; // debug
    ifs.read(reinterpret_cast<char *>(&buf[0]), inputSize);
    assert(ifs.gcount() == inputSize);
  
    // Correcting header.
    telechips_encode_sum(&buf[0], inputSize);
    telechips_encode_crc(&buf[0], inputSize);
  
    // Writing corrected firmware.
    ofs.write(reinterpret_cast<char *>(&buf[0]), inputSize);
  
    return true;
  }
  else
  {
    // TODO: Be more verbose.
    return false;
  }
}

/* The main program function */
int main( int argc, char *argv[] )
{
  if (argc > 2)
  {
    std::string const inFileName(argv[1]), outFileName(argv[2]);
    
    correctFWHeader(inFileName.c_str(), outFileName.c_str());
  }
  else
  {
    std::cout << "Look in sources for usage, please.\n";
  }
}
