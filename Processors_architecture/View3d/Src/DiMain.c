//  **********************************************************
//  FILE NAME   DiMain.h
//  PURPOSE     View3D main file
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiMain
//  NOTES
//  **********************************************************


//  **********************************************************
//  Includes
//  **********************************************************

// System specific includes
#include <windows.h>
#include <io.h>

#include <stdio.h>

// App specific includes
#include "DiTypes.h"
#include "DiErr.h"
#include "DiMem.h"
#include "DiScene.h"
#include "DiRender.h"
#include "DiMove.h"
#include "ViewRes.h"

#include "DiMain.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diMAIN_TITLE            "View3D application"
#define diMAIN_CLASS_NAME       "View3DClass"

#define diMAIN_DEF_SCENE_NAME   "Cube.ase"

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data for this module
//  **********************************************************

static  HWND                  _hWnd;
static  HINSTANCE				      _hInstance;          // App instance
static  DiBool                _bMousePressed= FALSE;
static  DiBool                _bActive      = FALSE;
static  DiBool                _bReady       = FALSE;
static  BOOL                  _bInUpdate    = FALSE;
static  DiInt32               _nTicks;
static  RECT                  _rcWindow;           // Saves the window size & pos.
static  RECT                  _rcViewport;         // Pos. & size to blt from
static  RECT                  _rcScreen;           // Screen pos. for blt

// Scene defaults
static  DiChar                *_caDefSceneName = diMAIN_DEF_SCENE_NAME;

// Windows rendering buffer
static  HBITMAP               _hbmp;
static  DiUInt8               *_npPixels;

// Fps values
static  DiInt32               _nLastUpdateFrameTime;
static  DiInt32               _nLastRenderNumber;
static  DiInt32               _nRNumber;

// For select dialog box
static  DiChar                _caFileName[128];

//  **********************************************************
//  Fordird refs
//  **********************************************************

//  **********************************************************
//  Functions 
//  **********************************************************

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hWnd
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diMainCreate(HWND hWnd)
{
  HDC            hdc;
  BYTE           bmiBuf[56];
  BITMAPINFO     *bmi;
  DWORD          *bmiCol;

  hdc   = GetDC(hWnd);
  if (!hdc)
    return FALSE;
  bmi   = ( BITMAPINFO * ) bmiBuf ;
  ZeroMemory( bmi , sizeof(BITMAPINFO) ) ;
  bmi->bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biWidth        = diMAIN_WINDOW_WIDTH;
  bmi->bmiHeader.biHeight       = -diMAIN_WINDOW_HEIGHT;
  bmi->bmiHeader.biPlanes       = 1;
  bmi->bmiHeader.biBitCount     = diMAIN_WINDOW_BPP;
  bmi->bmiHeader.biCompression  = BI_RGB;
  bmi->bmiHeader.biSizeImage    = diMAIN_WINDOW_WIDTH * diMAIN_WINDOW_HEIGHT * 3;
  bmiCol                        = ( DWORD * )( bmiBuf + sizeof(BITMAPINFOHEADER) ) ;
  bmiCol[0]                     = 0 ;
  bmiCol[1]                     = 0 ;
  bmiCol[2]                     = 0 ;

  _hbmp  = CreateDIBSection( 
                            hdc,
                            bmi,
                            DIB_RGB_COLORS,
                            (DiVoid**)&_npPixels,
                            NULL,
                            0
                         );
  ReleaseDC(hWnd, hdc);
  if (!_hbmp)
    return FALSE;

  return TRUE;
}

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diMainDestroy()
{
  if (_hbmp)
  {
    DeleteObject(_hbmp);
    _hbmp = 0;
  }
  return TRUE;

}

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hWnd
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diMainUpdateFrame(HWND hWnd)
{
  HDC       hdc;
  HDC       memDC;
  HBITMAP   hOldBitmap;

  // Update screen pixel buffer
  _nTicks = DiSceneRender(_npPixels);

  // Put to screen
  hdc      = GetDC(hWnd);
  if (!hdc)
    return FALSE;

  memDC    = CreateCompatibleDC(hdc);
  if (!memDC)
    return FALSE;
  hOldBitmap  = SelectObject(memDC, _hbmp);

  BitBlt( 
          hdc,
          0,                    // XDest
          0,                    // YDest
          diMAIN_WINDOW_WIDTH,  // Width
          diMAIN_WINDOW_HEIGHT, // Height
          memDC,                // DC
          0,                    // XSrc
          0,                    // YSrc
          SRCCOPY               // OpCode
       );

  SelectObject(  memDC , hOldBitmap);
  DeleteDC(      memDC);
  ReleaseDC(     hWnd, hdc);

  return TRUE;
}


/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hWnd
 * @param   nTicks
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diMainShowFrameRate( HWND hWnd, DiInt32 nTicks )
{
  char      S[64] ;
  long      Time ;

   Time = GetTickCount() ;
   if ( Time - _nLastUpdateFrameTime > 3000 )
   {
      sprintf( 
                S , 
                "FPS=%f, Ticks=%ld" , 
                (float)( _nRNumber - _nLastRenderNumber ) * 1000.0 / 
                (float)( Time - _nLastUpdateFrameTime ),
                nTicks
             ) ;
      SetWindowText( hWnd , S ) ;
      _nLastUpdateFrameTime = Time ;
      _nLastRenderNumber    = _nRNumber ;
   }
   _nRNumber ++ ;

  return(TRUE);
}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hWnd
 * @param   msg
 * @param   wParam
 * @param   lParam
 * @author  Vlad
 * @see     <see>
 */
LRESULT CALLBACK  MainWndProc(
                                HWND    hWnd, 
                                UINT    msg, 
                                WPARAM  wParam, 
                                LPARAM  lParam
                             )
{
    MINMAXINFO      *pMinMax;

    switch (msg)
    {
        case WM_ACTIVATEAPP:
        {
          // Pause if minimized or not the top window
        	_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
          return 0L;
        }

        case WM_KEYDOWN:
        {
          if (wParam == VK_ESCAPE)
          {
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            return 0L;
          }
          if (wParam == VK_SPACE)
          {
            DiRenderShapeMode mMode;
              
            mMode = DiRenderGetShapeMode();
            if (mMode == diRENDER_MODE_SOLID)
              mMode = diRENDER_MODE_WIREFRAME;
            else
              mMode = diRENDER_MODE_SOLID;
            DiRenderSetShapeMode(mMode);
            return 0L;
          }
          if (wParam == VK_LEFT)
          {
            DiMoveCamera(diMOVE_LEFT);
            return 0L;
          }
          if (wParam == VK_RIGHT)
          {
            DiMoveCamera(diMOVE_RIGHT);
            return 0L;
          }
          if (wParam == VK_UP)
          {
            DiMoveCamera(diMOVE_UP);
            return 0L;
          }
          if (wParam == VK_DOWN)
          {
            DiMoveCamera(diMOVE_DOWN);
            return 0L;
          }
          if (wParam == VK_PRIOR)
          {
            DiMoveCamera(diMOVE_FORWARD);
            return 0L;
          }
          if (wParam == VK_NEXT)
          {
            DiMoveCamera(diMOVE_BACKWARD);
            return 0L;
          }
          break;
        }

        case WM_DESTROY:
        {
          // Clean up and close the app
          PostQuitMessage(0);
          return 0L;
        }

        case WM_GETMINMAXINFO:
            // Fix the size of the window to client size
            pMinMax = (MINMAXINFO *)lParam;
            pMinMax->ptMinTrackSize.x = diMAIN_WINDOW_WIDTH+GetSystemMetrics(SM_CXSIZEFRAME)*2;
            pMinMax->ptMinTrackSize.y = diMAIN_WINDOW_HEIGHT+GetSystemMetrics(SM_CYSIZEFRAME)*2
                                           +GetSystemMetrics(SM_CYMENU);
            pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
            pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
            break;

        case WM_MOVE:
            // Retrieve the window position after a move
            if (_bActive && _bReady)
            {
              GetWindowRect(hWnd, &_rcWindow);
            	GetClientRect(hWnd, &_rcViewport);
            	GetClientRect(hWnd, &_rcScreen);
            	ClientToScreen(hWnd, (POINT*)&_rcScreen.left);
            	ClientToScreen(hWnd, (POINT*)&_rcScreen.right);
            }
            break;

        case WM_SETCURSOR:
            // Display the cursor in the window if windowed
            if (_bActive && _bReady)
            {
                SetCursor(NULL);
                return TRUE;
            }
            break;

        case WM_SIZE:
        {
          // Check to see if we are losing our window...
          if (SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam)
            _bActive = FALSE;
          else
            _bActive = TRUE;
          break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hDlg
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diMainInitDialog(HWND hDlg)
{
  struct _finddata_t  fFind;
  DiInt32             nFile;

  DIFUNCTION("_diMainInitDialog");
  if ( (nFile = _findfirst("Data\\*.ase", &fFind)) == -1)
  {
    DIRETURN(FALSE);
  }
  else
  {
    SendDlgItemMessage(hDlg, IDC_MODEL_LIST, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)(fFind.name));

    while (_findnext(nFile, &fFind) == 0)
    {
      SendDlgItemMessage(hDlg, IDC_MODEL_LIST, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)(fFind.name));
    }
    _findclose(nFile);
  }
  SendDlgItemMessage(hDlg, IDC_MODEL_LIST, LB_SETCURSEL, (WPARAM)0, 0);

  DIRETURN(TRUE);
} // end of _diMainInitDialog


/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hDlg
 * @param   uMsg
 * @param   wParam
 * @param   lParam
 * @author  Vlad
 * @see     <see>
 */
static  BOOL CALLBACK _diMainDialogProc(
                                        HWND    hDlg,
                                        UINT    nMsg, 
                                        WPARAM  wParam, 
                                        LPARAM  lParam
                                       )
{
  DIFUNCTION("_diMainDialogProc");
  switch (nMsg)
  {
    case WM_INITDIALOG:
    {
      _diMainInitDialog(hDlg);
      DIRETURN(TRUE);
    }
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case IDOK:
        {
          DiInt32   nIndex;

          nIndex = SendDlgItemMessage(hDlg, IDC_MODEL_LIST, LB_GETCURSEL, 0, 0);
          SendDlgItemMessage(
                              hDlg, 
                              IDC_MODEL_LIST, 
                              LB_GETTEXT, 
                              (WPARAM)nIndex, (LPARAM)(LPCTSTR)(_caFileName));
          EndDialog(hDlg, TRUE);
          DIRETURN(TRUE);
        }
        case IDCANCEL:
        {
          EndDialog(hDlg, -1);
          DIRETURN(TRUE);
        }
      } // switch
    }
  } // switch
  DIRETURN(FALSE);
} // end of _diMainDialogProc

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCmdLine
 * @author  Vlad
 * @see     <see>
 */
DiChar  *_diMainGetFileName(DiChar  *cpCmdLine)
{
  DiInt32   nRet;
  DiChar    *cpName;

  DIFUNCTION("_diMainGetFileName");
  nRet = DialogBox(
                    _hInstance, 
                    MAKEINTRESOURCE(IDD_SELECT_MODEL), 
                    NULL,
                    _diMainDialogProc
                  );
  if (nRet == -1)
  {
    DIRETURN(NULL);
  }
  cpName = _caFileName;

  DIRETURN(cpName);
} // end of _diMainGetFileName

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   hInstance
 * @param   hPrevInstance
 * @param   cpCmdLine
 * @param   nCmdShow
 * @author  Vlad
 * @see     <see>
 */
int PASCAL  WinMain(
                      HINSTANCE   hInstance,
                      HINSTANCE   hPrevInstance,
                      LPSTR       cpCmdLine,
                      int         nCmdShow
                   )
{
  WNDCLASS	          wc;
  MSG			            msg;
  DiInt32             cx,cy;
  DiChar              *cpSceneFileName;
  DiRenderTargetDesc  rDesc;

  if (!hPrevInstance)
  {
    // Register the Window Class
    wc.lpszClassName = diMAIN_CLASS_NAME;
    wc.lpfnWndProc = MainWndProc;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance = hInstance;
    wc.hIcon = NULL; //LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL ; // MAKEINTRESOURCE(IDR_MENU);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    RegisterClass(&wc);
  }

  _hInstance = hInstance;

  // Get Scene name
  cpSceneFileName = _diMainGetFileName(cpCmdLine);
  if (!cpSceneFileName)
  {
    return (-1);
  }

    // Calculate the proper size for the window given a client 
  cx = diMAIN_WINDOW_WIDTH+GetSystemMetrics(SM_CXSIZEFRAME)*2;
  cy = diMAIN_WINDOW_HEIGHT+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
  // Create and Show the Main Window
  _hWnd = CreateWindowEx(0,
                          diMAIN_CLASS_NAME,
                          diMAIN_TITLE,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
  	                      cx,
                          cy,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
  if (_hWnd == NULL)
    return FALSE;
  ShowWindow(_hWnd, nCmdShow);
  UpdateWindow(_hWnd);

  if (!_diMainCreate(_hWnd))
  {
    return FALSE;
  }

  // Init systems
  DiMemorySystemOpen();

  // Create scene from file
  if (!DiSceneCreate(cpSceneFileName))
  {
    MessageBox( _hWnd, "Cant init scene: check files", "Error", MB_OK);
    return FALSE;
  }

  rDesc.npDestBuffer  = _npPixels;
  rDesc.nWidth        = diMAIN_WINDOW_WIDTH;
  rDesc.nHeight       = diMAIN_WINDOW_HEIGHT;
  rDesc.nBPP          = diMAIN_WINDOW_BPP;
  DiRenderSystemOpen(&rDesc);

  _bReady = TRUE;

  // *************************************************************
  // **                   The Message Loop                      **
  // *************************************************************

  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      // if QUIT message received quit app
      if (!GetMessage(&msg, NULL, 0, 0 ))
        break;
      // Translate and dispatch the message
      TranslateMessage(&msg); 
      DispatchMessage(&msg);
    }
    else
    if (_bActive && _bReady)
    {
      _bInUpdate = TRUE;

      // Update the background and flip every time the timer ticks
      _diMainUpdateFrame(_hWnd);
      _diMainShowFrameRate(_hWnd,_nTicks) ;

      _bInUpdate = FALSE;
    } // Update app
  } // while ( TRUE )

  _diMainDestroy();

  DiSceneDestroy();
  DiRenderSystemClose();
  DiMemorySystemClose();

  return msg.wParam;
}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   nMaxChars
 * @author  Vlad
 * @see     <see>
 */
DiBool    DiMainGetPath(DiChar *cpBuffer, DiInt32 nMaxChars)
{
  DiChar    *cpS;

  DIFUNCTION("DiMainGetPath");
  GetModuleFileName(NULL, cpBuffer, nMaxChars);
  cpS = strrchr(cpBuffer, '\\');
  DIASSERT(cpS);
  cpS[1] = 0;
  DIRETURN(TRUE);
} // end of DiMainGetPath

// END
