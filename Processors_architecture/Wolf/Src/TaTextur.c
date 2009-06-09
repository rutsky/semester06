//  **********************************************************
//  FILE NAME   TaTextur.c
//  PURPOSE     Texture management
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <stdio.h>

#include "TaTypes.h"
#include "TaWorld.h"
#include "TaTextur.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  TaUInt8*    _npaTextures[taTEXTURE_MAX_TEXTURES];

//  **********************************************************
//  Functions
//  **********************************************************

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaBool  _taTextureRead(
                              TaInt32   nT, 
                              TaChar    *cpFileName
                            )
{
  BITMAPFILEHEADER    hHead;
  BITMAPINFOHEADER    hInfo;
  HFILE               hFile;
  TaUInt8             *cpBitmap;
  TaInt32             nI;
  TaInt32             nJ;
  TaUInt8             *npSrc;
  TaUInt16            *npDest;
  TaUInt16            nColor;
  TaUInt16            nValue;
  TaUInt16            *npTex1;
  TaUInt16            *npTex2;

  hFile = _lopen(cpFileName, OF_READ);
  if (hFile == HFILE_ERROR)
  {
    return (taSUCCESS);
  }
  cpBitmap = (TaUInt8*)TaMemAllocate(taTEXTURE_SIDE*taTEXTURE_SIDE*3);
  if (!cpBitmap)
  {
    return (taFAIL);
  }
  _lread(hFile , &hHead , sizeof(hHead));
  if ( 
        hHead.bfType != 19778 ||
        hHead.bfOffBits != 54
     )
  {
    _lclose(hFile);
    return (taFAIL);
  }

  _lread(hFile , &hInfo, sizeof(hInfo));
  if (hInfo.biWidth != taTEXTURE_SIDE)
  {
    _lclose(hFile);
    return (taFAIL);
  }
  if (hInfo.biHeight != taTEXTURE_SIDE)
  {
    _lclose(hFile);
    return (taFAIL);
  }
  if (hInfo.biBitCount != 24)
  {
    _lclose(hFile);
    return (taFAIL);
  }

  _llseek(hFile , hHead.bfOffBits , FILE_BEGIN);
  _lread(hFile  , cpBitmap , taTEXTURE_SIDE*taTEXTURE_SIDE*3);
  _lclose(hFile);


  // Convert to 16 bpp format
  _npaTextures[nT] = (TaUInt8*)TaMemAllocate(taTEXTURE_SIDE*taTEXTURE_SIDE*2);
  if (!_npaTextures[nT])
  {
    TaMemFree(cpBitmap);
    return(taFAIL);
  }

  npSrc   = (TaUInt8*)cpBitmap;
  npDest  = (TaUInt16*)_npaTextures[nT];
  for (nI = 0; nI < taTEXTURE_SIDE*taTEXTURE_SIDE; nI++)
  {
    nColor = 0;

    // Blue
    nValue = (TaUInt16)(*npSrc);
    nValue >>= 3;
    nColor |= nValue;
    npSrc++;

    // Green
    nValue = (TaUInt16)(*npSrc);
    nValue >>= 3;
    nValue <<= 5;
    nColor |= nValue;
    npSrc++;

    // Red
    nValue = (TaUInt16)(*npSrc);
    nValue >>= 3;
    nValue <<= 10;
    nColor |= nValue;
    npSrc++;

    // Write 16 bit color
    *npDest = nColor;
    npDest++;
  }
  TaMemFree(cpBitmap);

  // Flip texture vertical due to bitmap wrong orientation
  for (nJ = 0; nJ < taTEXTURE_SIDE/2; nJ++)
  {
    npTex1 = (TaUInt16*)(_npaTextures[nT]) + nJ * taTEXTURE_SIDE;
    npTex2 = (TaUInt16*)(_npaTextures[nT]) + (taTEXTURE_SIDE - 1 - nJ) * taTEXTURE_SIDE;
    for (nI = 0; nI < taTEXTURE_SIDE; nI++)
    {
      nColor = *npTex1;
      *npTex1 = *npTex2;
      *npTex2 = nColor;
      npTex1++;
      npTex2++;
    }
  }

  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaTextureOpen(TaVoid)
{
  TaInt32     nT;
  TaChar      caFileName[32];

  for (nT = 0; nT < taTEXTURE_MAX_TEXTURES; nT++)
  {
    _npaTextures[nT] = NULL;
    sprintf(caFileName, "Data\\Tex%02ld.bmp", nT + 1);
    if (_taTextureRead(nT, caFileName) != taSUCCESS)
    {
      return (taFAIL);
    }
  }
  
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaTextureClose(TaVoid)
{
  TaInt32     nT;

  for (nT = 0; nT < taTEXTURE_MAX_TEXTURES; nT++)
  {
    if (_npaTextures[nT])
    {
      TaMemFree(_npaTextures[nT]);
      _npaTextures[nT] = NULL;
    }
  }
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaUInt8 *TaTextureGet(TaInt32 nTextureIndex)
{
  if (nTextureIndex < 0 || nTextureIndex >= taTEXTURE_MAX_TEXTURES)
    return(NULL);
  return (_npaTextures[nTextureIndex]);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaTextureDrawVertLine(
                            TaUInt8 *cpScreen,
                            TaInt32 nScreenYUp,
                            TaInt32 nScreenYDown,
                            TaInt32 nTextureIndex,
                            TaInt32 nTextureX,
                            TaInt32 nScreenX
                           )
{
  TaUInt16    *npTexture;
  TaInt32     nTexStart, nTexEnd;
  TaInt32     nTexStep;
  TaInt32     nTexAccurate;
  TaUInt16    *npSrc;
  TaUInt16    *npDest;
  TaInt32     nY;
  TaInt32     nTexY;
  TaUInt16    nColor;

  // Cull fully invisible vert line
  if (nScreenYUp >= taWINDOW_HEIGHT)
  {
    return (taSUCCESS);
  }
  if (nScreenYDown <= 0)
  {
    return (taSUCCESS);
  }

  nTexStart = 0;
  nTexEnd   = taTEXTURE_SIDE - 1;

  // Clip partially visible vert line
  if (nScreenYUp < 0)
  {
    return (taSUCCESS);
  }
  if (nScreenYDown >= taWINDOW_HEIGHT)
  {
    return (taSUCCESS);
  }

  npTexture = (TaUInt16*)TaTextureGet(nTextureIndex);
  if (!npTexture)
  {
    return (taFAIL);
  }

  nTexStep = 
              ((nTexEnd - nTexStart) << 16) / 
              (nScreenYDown - nScreenYUp + 1);

  nTexAccurate = nTexStart << 16;


  npDest = 
            (TaUInt16*)cpScreen + nScreenX + 
            nScreenYUp * taWINDOW_WIDTH;
  npSrc = npTexture + nTextureX;
  for (nY = nScreenYUp; nY <= nScreenYDown; nY++)
  {
    // Get int texture coord Y
    nTexY = nTexAccurate >> 16;
    nColor = npSrc[nTexY << taTEXTURE_SIDE_DEGREE];
    *npDest = nColor;
    npDest += taWINDOW_WIDTH;
    nTexAccurate += nTexStep;
  }

  return (taSUCCESS);
}

// NAME        TextureDrawScreen
// PURPOSE     For test output to screen only
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaTextureDrawScreen(
                            TaUInt8 *cpScreen,
                            TaInt32 nTextureIndex
                         )
{
  TaUInt16    *npSrc;
  TaUInt16    *npDest;
  TaInt32     nI;


  npSrc = (TaUInt16*)TaTextureGet(nTextureIndex);
  if (!npSrc)
  {
    return (taFAIL);
  }
  npDest = (TaUInt16*)cpScreen;

  for (nI = 0; nI < taTEXTURE_SIDE; nI++)
  {
    memcpy(npDest, npSrc, taTEXTURE_SIDE* 2);
    npSrc   += taTEXTURE_SIDE;
    npDest  += taWINDOW_WIDTH;
  }

  return (taSUCCESS);
}



// END
