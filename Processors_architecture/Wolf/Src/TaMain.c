//  **********************************************************
//  FILE NAME   TaMain.c
//  PURPOSE     Main module asm template project
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************


//  **********************************************************
//  Includes
//  **********************************************************

#include <math.h>

#include "TaTypes.h"
#include "TaScene.h"
#include "TaWorld.h"
#include "TaCamera.h"
#include "TaRes.h"


//  **********************************************************
//  Defines
//  **********************************************************

//  class name
#define WINDOW_CLASSNAME      "WolfClass"
#define WINDOW_TITLE          "Wolfenstein3D task"


//  **********************************************************
//  Static data for this module
//  **********************************************************
static  HINSTANCE       hAppInstance;

static  TaBool          _bShiftPressed = FALSE;
static  TaChar          *_cpArgs = NULL;

//  **********************************************************
//  Public data for all modules
//  **********************************************************
TaBool                  bAppActive;
HWND            	      hFrame;

//  **********************************************************
//  Functions
//  **********************************************************

static  TaBool  RenderScene(HWND   hFrame)
{
  TaBool    bRet;
  if (bAppActive)
  {
    bRet  = TaScenePaint(hFrame);
    return (bRet);
  }
  return (TRUE);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
LRESULT CALLBACK  WndProc(
                            HWND    hFrame,
                            UINT    msg,
                            WPARAM  wParam,
                            LPARAM  lParam
                         )
{
    TaBool        hRes;

    switch (msg)
    {
        case WM_CREATE:
        {
          return 0L;
        }

        case WM_SYSCOMMAND:
        {
          if (wParam == SC_SCREENSAVE)
            return 0L;
          break ;
        }

        case WM_ACTIVATEAPP:
        {
          // Keep track of whether or not the app is in the foreground
          bAppActive = (TaBool)wParam;
          break;
        }
        case WM_ACTIVATE:
        {
          if(LOWORD(wParam) == WA_INACTIVE)
          {
            SendMessage(hFrame, WM_KILLFOCUS, 0, 0 );
          }
          else
          {
            SendMessage(hFrame, WM_SETFOCUS, 0, 0 );
          }
          return 0;
        }

        //  when Alt-Tab pressed
        case WM_CANCELMODE:
        {
          SendMessage(hFrame, WM_KILLFOCUS, 0, 0 );
          return 0;
        }

        case WM_KILLFOCUS:
        {
          //  return thread priority to normal
          SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
          SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
          return 0;
        }

        case WM_SETFOCUS:
        {
          //  go as high as we dare. Any higher than this and
          //  we start to interfere with task switching.
          //
          SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
          SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
          return 0;
        }
        case WM_KEYUP:
        {
          /*
           * We use the escape key as a quick way of getting out of the
           * application.
           */
          switch (wParam)
          {
            case VK_ESCAPE:
            {
              DestroyWindow(hFrame);
              return 0L;
            }
            case VK_SPACE:
            {
              break;
            }
            case VK_SHIFT:
            {
              _bShiftPressed = FALSE;
              break;
            }
          }
          break;
        }
        case WM_KEYDOWN:
        {
          switch (wParam)
          {
            case VK_SHIFT:
            {
              _bShiftPressed = TRUE;
              break;
            }
            case VK_UP:
            {
              TaCameraUp();
              break;
            }
            case VK_DOWN:
            {
              TaCameraDown();
              break;
            }
            case VK_LEFT:
            {
              if (_bShiftPressed)
                TaCameraLeft();
              else
                TaCameraRotateLeft();
              break;
            }
            case VK_RIGHT:
            {
              if (_bShiftPressed)
                TaCameraRight();
              else
                TaCameraRotateRight();
              break;
            }

          }
          break;
        }
        case WM_CLOSE:
        {
          DestroyWindow(hFrame);
          return 0L;
        }

        case WM_DESTROY:
        {
          // Close application
          {
            hRes  = TaSceneDestroy();
            if (!hRes)
              return -1 ;
          }
          PostQuitMessage(0);
          return 0L;
        }
    }
    return DefWindowProc(hFrame, msg, wParam, lParam);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid  _mainFindArgs(TaChar  *cpCommandLine)
{
  if (cpCommandLine[0] == 0)
    return;
  _cpArgs = cpCommandLine;
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
int PASCAL  WinMain(
                      HINSTANCE   hInstance,
                      HINSTANCE   hPrevInstance,
                      LPSTR       lpszCommandLine,
                      int         cmdShow
                   )
{
    WNDCLASS  wndClass;
    MSG       msg;
    TaBool    bRet;


    _mainFindArgs(lpszCommandLine);

    /*
     * Record the instance handle.
     */
    hAppInstance = hInstance;

    /*
     * Register the window class.
     */
    wndClass.style         = 0;
    wndClass.lpfnWndProc   = WndProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = hInstance;
    wndClass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(ICO_WOLF));
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = WINDOW_CLASSNAME;

    RegisterClass(&wndClass);

    // Check windows size
    if ( 
          GetSystemMetrics( SM_CXSCREEN ) < taWINDOW_WIDTH ||
          GetSystemMetrics( SM_CYSCREEN ) < taWINDOW_HEIGHT
       )
    {
      MessageBox( 
                  GetFocus() , 
                  "Windows screen too small" , 
                  "Error" , 
                  MB_OK 
                );
      return (-1);
    }

    // Check bits per pixel
    {
      HDC     hdc;
      TaInt32 nCaps;

      hdc = GetDC(NULL);
      nCaps = GetDeviceCaps(hdc , BITSPIXEL);
      ReleaseDC(NULL, hdc);
      if (nCaps != 16)
      {
        MessageBox( 
                    NULL , 
                    "Set 16 bpp windows mode" , 
                    "Error" , 
                    MB_OK 
                  );
        return (-1);
      }
    }



    /*
     * Create the main window of the instance.
     */
    hFrame = CreateWindow(
                        WINDOW_CLASSNAME,
                        WINDOW_TITLE,
                        WS_OVERLAPPED | WS_SYSMENU,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        taWINDOW_WIDTH+GetSystemMetrics(SM_CXSIZEFRAME)*2, 
                        taWINDOW_HEIGHT+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYCAPTION),
                        NULL,
                        NULL,
                        hInstance,
                        NULL);
    if (!hFrame)
      return (-1);

    bRet  = TaSceneCreate(hFrame);
    if (!bRet)
      return -1;

    ShowWindow(hFrame, cmdShow);
    UpdateWindow(hFrame);

    /*
     * The main message dispatch loop.
     *
     * NOTE: For simplicity we handle the message loop with a
     * simple PeekMessage scheme. This might not be the best
     * mechanism for a real application (a separate render worker
     * thread might be better). 
     */
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if ( GetFocus() == hFrame )
            {
              TaBool  bRet;
              HDC   hdc;

              hdc = GetDC( hFrame );
              bRet = RenderScene(hFrame);
              ReleaseDC(hFrame, hdc);
              if (!bRet)
              {
                DestroyWindow(hFrame);
                break;
              }
            }
        }
    }
    return msg.wParam;
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaChar  *TaMainGetArgs(TaVoid)
{
  return (_cpArgs);
}

// END
