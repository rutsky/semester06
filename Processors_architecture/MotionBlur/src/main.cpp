/* main.cpp
 * Motion blur effect.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.06.2009
 */

#include <iostream>
#include <istream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <cstring>
#include <memory>
#include <vector>

#include <SDL.h>

#include "effect_motion_blur.h"

namespace constants
{
  char   const *defaultBackgroundImagePath = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled.jpg";
  char   const *defaultMovingImagePath     = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled_car.png";
  size_t const  defaultNMovingFrames       = 10;
  int    const  defaultMovingFramesStep    = -2;
  
  int const windowWidth  = 1000;
  int const windowHeight = 500;
  int const windowBPP    = 32;
} // End of namespace `constants'.

namespace cmdline
{
  char const *getBackgroundImagePath( int argc, char *argv[] )
  {
    char const *path = constants::defaultBackgroundImagePath;
    if (argc >= 2)
      path = argv[1];
    return path;
  }
  
  char const *getMovingImagePath( int argc, char *argv[] )
  {
    char const *path = constants::defaultMovingImagePath;
    if (argc >= 3)
      path = argv[2];
    return path;
  }
  
  size_t getNMovingFrames( int argc, char *argv[] )
  {
    size_t result = constants::defaultNMovingFrames;
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
    int result = constants::defaultMovingFramesStep;
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

// Main program loop.
int mainLoop( SDL_Surface *screen )
{
  bool exit = false;
  while (!exit)
  {
    SDL_Event event;

    // Polling all events.
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
          std::cout << "The " << SDL_GetKeyName(event.key.keysym.sym) << " key was pressed!" << std::endl;
          break;
            
        case SDL_QUIT:
          // Exiting.
          exit = true;
          break;
      }
    }
    
    // Doing work.
  }
  
  return 0;
}
    
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
  
  {
    // Creating SDL window.
    SDL_Surface *screen;

    screen = SDL_SetVideoMode(constants::windowWidth, constants::windowHeight, constants::windowBPP, SDL_SWSURFACE);
    if (screen == NULL)
    {
      std::cerr << "Unable to set " 
          << constants::windowWidth << "x" << constants::windowHeight << "x" << constants::windowBPP 
          << " video mode window: %s\n" << SDL_GetError() << std::endl;
      SDL_Quit();
      return 2;
    }
    assert(screen->format->BytesPerPixel == 4);
    
    // Running main loop.
    mainLoop(screen);
  }

  // Deinitializing SDL.
  SDL_Quit();
  
  return 0;
}
