/* main.cpp
 * Motion blur effect.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.06.2009
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

#include <SDL.h>

#include "effect_motion_blur.h"

namespace cmdline
{
  char const *getBackgroundImagePath( int argc, char *argv[] )
  {
    char const *path = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled.jpg";
    if (argc >= 2)
      path = argv[1];
    return path;
  }
  
  char const *getMovingImagePath( int argc, char *argv[] )
  {
    char const *path = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled_car.png";
    if (argc >= 3)
      path = argv[2];
    return path;
  }
  
  size_t getNMovingFrames( int argc, char *argv[] )
  {
    size_t result = 10;
    if (argc >= 4)
    {
      std::string const str(argv[3]);
      std::istringstream istr(str);
      int val;
      istr >> val;
      result = val;
    }
    return result;
  }
  
  int getMovingFramesStep( int argc, char *argv[] )
  {
    int result = -2;
    if (argc >= 5)
    {
      std::string const str(argv[4]);
      std::istringstream istr(str);
      int val;
      istr >> val;
      result = val;
    }
    return result;
  }
} // End of namespace `cmdline'.

// Usefull function.
#ifdef WIN32
  #define array_size(x) \
    (sizeof(x) / sizeof((x)[0]))
#else // WIN32
  template< typename T, size_t N >
  size_t array_size( T (&)[N] ) 
  { 
    return N; 
  }
#endif // not WIN32

// The main program function.
int main( int argc, char *argv[] )
{
  char const *backgroundImagePath = cmdline::getBackgroundImagePath(argc, argv);
  char const *movingImagePath     = cmdline::getMovingImagePath(argc, argv);
  size_t nMovingFrames            = cmdline::getNMovingFrames(argc, argv);
  int movingFramesStep            = cmdline::getMovingFramesStep(argc, argv);
  
  // Initializing SDL.
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "Error: failed to initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Deinitializing SDL.
  SDL_Quit();
  
  return 0;
}
