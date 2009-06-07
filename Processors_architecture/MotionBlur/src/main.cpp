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
#include <SDL_image.h>

#include "effect_motion_blur.h"

// Useful `array_size' function.
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

namespace constants
{
  char   const *defaultBackgroundImagePath = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled.jpg";
  char   const *defaultMovingImagePath     = "../data/Kimi_Raikkonen_won_2007_Brazil_GP_scaled_car.png";
  size_t const  defaultNMovingFrames       = 10;
  int    const  defaultMovingFramesStep    = -2;
  
  int const windowWidth  = 1000;
  int const windowHeight = 500;
  int const windowBPP    = 32;
  
  Uint32 const fpsUpdateInterval = 1000; // in ticks (1/1000 of second).
  
  char const *effectImplsNames[] = 
    {
      "Base version",
      "Paired copying",
    };
  size_t const nEffectImpls = array_size(effectImplsNames);
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
    assert(result > 0);
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

void updateWindowTitle( int effectCurImpl, double fps )
{
  std::ostringstream ostr;
  
  assert(effectCurImpl < static_cast<int>(array_size(constants::effectImplsNames)));
  ostr << constants::effectImplsNames[effectCurImpl] << " " 
      << std::fixed << std::setw(9) << std::setprecision(4) << fps << " FPS";
  
  SDL_WM_SetCaption(ostr.str().c_str(), NULL);
}

// Main program loop.
int mainLoop( SDL_Surface *screen,
              SDL_Surface *backgroundSurface,
              SDL_Surface *movingSurfaces[], size_t nMovingSurfaces )
{
  size_t frameCounter(0);
  Uint32 frameCounterLastUpdateTicks = SDL_GetTicks();

  int curEffectImpl = 0;
  double curEffectFPS = 0;
  
  updateWindowTitle(curEffectImpl, curEffectFPS);
    
  bool exit = false;
  while (!exit)
  {
    // Polling all events.
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
          case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_LEFT)
            {
              curEffectImpl = (curEffectImpl + 1) % constants::nEffectImpls;

              frameCounterLastUpdateTicks = SDL_GetTicks();
              curEffectFPS = 0;
              frameCounter = 0;
              updateWindowTitle(curEffectImpl, curEffectFPS);
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
              curEffectImpl = (curEffectImpl + constants::nEffectImpls - 1) % constants::nEffectImpls;

              frameCounterLastUpdateTicks = SDL_GetTicks();
              curEffectFPS = 0;
              frameCounter = 0;
              updateWindowTitle(curEffectImpl, curEffectFPS);
            }
            // std::cout << "The " << SDL_GetKeyName(event.key.keysym.sym) << " key was pressed!" << std::endl; // TODO: remove this line.
            break;
              
          case SDL_QUIT:
            // Exiting.
            exit = true;
            break;
        }
      }
    }
    
    // Doing work.
    {
      {
        SDL_Rect srcRect = {0};
        srcRect.w = screen->w;
        srcRect.h = screen->h;
        int const result = SDL_BlitSurface(backgroundSurface, &srcRect, screen, NULL);
        assert(result == 0);
      }
      
      SDL_UpdateRect(screen, 0, 0, 0, 0);
      
      //SDL_Delay(10);
    }
    
    // Timing.
    {
      ++frameCounter;
      
      Uint32 const curTicks = SDL_GetTicks();
      if (curTicks >= frameCounterLastUpdateTicks + constants::fpsUpdateInterval)
      {
        Uint32 const ticksDelta = curTicks - frameCounterLastUpdateTicks;
        
        frameCounterLastUpdateTicks = curTicks;
        curEffectFPS = static_cast<double>(frameCounter) * 1000.0 / ticksDelta;
        frameCounter = 0;
        updateWindowTitle(curEffectImpl, curEffectFPS);
      }
    }
  }
  
  return 0;
}

bool prepareData( SDL_Surface *screen,
                  char const *backgroundImagePath, char const *movingImagePath,
                  size_t nMovingFrames, int movingFramesStep,
                  SDL_Surface **backgroundSurface, 
                  std::vector<SDL_Surface *> &movingSurfaces )
{
  // Loading data images.
  SDL_Surface *backgroundLoaded = IMG_Load(backgroundImagePath);
  if (!backgroundLoaded)
  {
    std::cerr << "Failed to load `" << backgroundImagePath << "' with IMG_Load: " << IMG_GetError() << std::endl;
    return false;
  }
  
  SDL_Surface *movingLoaded = IMG_Load(movingImagePath);
  if (!movingLoaded)
  {
    std::cerr << "Failed to load `" << movingImagePath << "' with IMG_Load: " << IMG_GetError() << std::endl;
    return false;
  }
  
  // Converting to screen format.
  backgroundLoaded = SDL_ConvertSurface(backgroundLoaded, screen->format, SDL_SWSURFACE);
  if (backgroundLoaded == NULL)
  {
    std::cerr << "Error: failed to convert background image pixel format to screen pixel format." << std::endl;
    return false;
  }
  movingLoaded = SDL_ConvertSurface(movingLoaded, screen->format, SDL_SWSURFACE);
  if (movingLoaded == NULL)
  {
    std::cerr << "Error: failed to convert moving image pixel format to screen pixel format." << std::endl;
    return false;
  }

  // Resizing data images to screen size.
  SDL_Surface *background = 
      SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel,
                           screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
  assert(background);
  {
    SDL_Rect srcRect = {0};
    srcRect.w = std::min(screen->w, backgroundLoaded->w);
    srcRect.h = std::min(screen->h, backgroundLoaded->h);
    
    backgroundLoaded->flags &= ~SDL_SRCALPHA;
    int const result = SDL_BlitSurface(backgroundLoaded, &srcRect, background, NULL);
    assert(result == 0);
    background->flags |= SDL_SRCALPHA;
  }
  
  SDL_Surface *moving = 
      SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel,
                           screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
  assert(moving);
  {
    SDL_Rect srcRect = {0};
    srcRect.w = std::min(screen->w, movingLoaded->w);
    srcRect.h = std::min(screen->h, movingLoaded->h);
    
    movingLoaded->flags &= ~SDL_SRCALPHA;
    int const result = SDL_BlitSurface(backgroundLoaded, &srcRect, background, NULL);
    assert(result == 0);
  }
  
  assert(background->w == screen->w && background->h == screen->h);
  assert(moving->w     == screen->w && moving->h     == screen->h);
  
  // Creating blur steps images.
  moving->flags &= ~SDL_SRCALPHA;
  movingSurfaces.resize(nMovingFrames, (SDL_Surface *)NULL);
  for (size_t i = 0; i < nMovingFrames; ++i)
  {
    movingSurfaces[i] = 
        SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, 
                             screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

    int hOffset = movingFramesStep * (nMovingFrames - 1 - i);
    
    SDL_Rect srcRect = {0};
    srcRect.w = screen->w;
    srcRect.h = screen->h;
    
    SDL_Rect dstRect = {0};
    dstRect.x = hOffset;

    int const result = SDL_BlitSurface(moving, &srcRect, movingSurfaces[i], &dstRect);
    assert(result == 0);
    movingSurfaces[i]->flags |= SDL_SRCALPHA;
  }
  
  *backgroundSurface = background;

  return true;
}

// The main program function.
int main( int argc, char *argv[] )
{
  char   const *backgroundImagePath = cmdline::getBackgroundImagePath(argc, argv);
  char   const *movingImagePath     = cmdline::getMovingImagePath(argc, argv);
  size_t const nMovingFrames        = cmdline::getNMovingFrames(argc, argv);
  int    const movingFramesStep     = cmdline::getMovingFramesStep(argc, argv);
  
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
    }
    else
    {
      assert(screen->format->BytesPerPixel == 4);
      
      SDL_Surface *backgroundSurface;
      std::vector<SDL_Surface *> movingSurfaces;
      
      if (prepareData(screen, backgroundImagePath, movingImagePath, nMovingFrames, movingFramesStep,
                      &backgroundSurface, movingSurfaces))
      {
        // Running main loop.
        mainLoop(screen, backgroundSurface, &(movingSurfaces[0]), nMovingFrames);
      }
      else
      {
        // Exiting. // TODO: Do it with error.
      }
    }
  }

  // Deinitializing SDL.
  SDL_Quit();
  
  return 0;
}
