//  **********************************************************
//  FILE NAME   TaScene.c
//  PURPOSE     Scene definition common module
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************


//  **********************************************************
//  Includes
//  **********************************************************

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TaTypes.h"
#include "TaWorld.h"
#include "TaScene.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  HBITMAP   hBmp;
static  TaUInt32  nGlobalTime = 0;
static  TaUInt32  nOldGlobalTime = 0;


// Render buffer with pixels (dimension WINDOW_WIDTH * WINDOW_HEIGHT)
static  LPSTR     npPixels;
// WIndows handle to display render buffer
static  TaUInt8   *cpBitmap;

// Data to account frame rate
static  TaUInt32  nLastUpdateFrameTime;
static  TaUInt32  nLastRenderNumber;
static  TaUInt32  nRNumber;

//  **********************************************************
//  Functions
//  **********************************************************

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaSceneCreate(HWND hWnd)
{
  HDC            hdc;
  BYTE           bmiBuf[56];
  BITMAPINFO     *bmi;
  TaUInt32       *bmiCol;

  hdc   = GetDC( hWnd ) ;
  if (!hdc)
    return(taFAIL);
  bmi   = ( BITMAPINFO * ) bmiBuf ;
  ZeroMemory( bmi , sizeof(BITMAPINFO) );
  bmi->bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biWidth        = taWINDOW_WIDTH;
  bmi->bmiHeader.biHeight       = -taWINDOW_HEIGHT;
  bmi->bmiHeader.biPlanes       = 1;
  bmi->bmiHeader.biBitCount     = 16;
  bmi->bmiHeader.biCompression  = BI_RGB;
  bmi->bmiHeader.biSizeImage    = taWINDOW_WIDTH * taWINDOW_HEIGHT * 2;
  bmiCol                        = ( TaUInt32 * )( bmiBuf + sizeof(BITMAPINFOHEADER) );
  bmiCol[0]                     = 0;
  bmiCol[1]                     = 0;
  bmiCol[2]                     = 0;

  hBmp  = CreateDIBSection( 
                            hdc ,
                            bmi ,
                            DIB_RGB_COLORS ,
                            &npPixels ,
                            NULL ,
                            0
                         ) ;
  ReleaseDC(hWnd, hdc);
  if (!hBmp)
    return (taFAIL);

  if (!TaWorldCreate())
    return(taFAIL);

  return(taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaSceneDestroy()
{
  if (!TaWorldDestroy())
    return (taFAIL);

  if (hBmp)
  {
    DeleteObject(hBmp);
    hBmp  = 0;
  }
  return(taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid  _taSceneShowFrameRate(
                              HWND  hWnd,
                              long  nTicks
                             )
{
  char      S[64] ;
  long      Time ;

   Time = GetTickCount() ;
   if ( Time - nLastUpdateFrameTime > 3000 )
   {
      sprintf( 
                S , 
                "FPS=%f, Ticks=%ld" , 
                (float)( nRNumber - nLastRenderNumber ) * 1000.0 / 
                (float)( Time - nLastUpdateFrameTime ),
                nTicks
             ) ;
      SetWindowText(hWnd , S);
      nLastUpdateFrameTime = Time;
      nLastRenderNumber    = nRNumber;
   }
   nRNumber ++ ;
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaScenePaint(HWND hWnd) 
{
   HDC        hdc;
   HDC        memDC;
   HGDIOBJ    hOldBitmap;
   long       nTicks;

   nOldGlobalTime  = nGlobalTime;
   nGlobalTime     = GetTickCount();
   if (!nOldGlobalTime)
      nOldGlobalTime  = nGlobalTime;

   hdc  = GetDC(hWnd);
   if (!hdc)
    return(taFAIL);

   // empty body costs 31 ticks
   nTicks = TaWorldUpdate( (char*)(npPixels) ) - 31;

   memDC    = CreateCompatibleDC(hdc);
   if (!memDC)
     return(taFAIL);
   hOldBitmap  = SelectObject(memDC , hBmp);

   BitBlt( 
            hdc,
            0,
            0,
            taWINDOW_WIDTH,
            taWINDOW_HEIGHT,
            memDC,
            0,
            0,
            SRCCOPY
         );

   SelectObject(  memDC , hOldBitmap );
   DeleteDC(      memDC );
   ReleaseDC(     hWnd , hdc );

   _taSceneShowFrameRate(hWnd, nTicks);
   return(taSUCCESS);
}

// END
