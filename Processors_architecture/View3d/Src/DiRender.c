//  **********************************************************
//  FILE NAME   DiRender.c
//  PURPOSE     Render primitives
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiRender
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <memory.h>

#include "DiErr.h"
#include "DiMem.h"

#include "DiRender.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diRENDER_ACC_BITS     16

#define diRENDER_ACC_HALF     (1L<<(diRENDER_ACC_BITS-1))

#define diRENDER_SWAP(a, b, c)    {(c) = (a); (a) = (b); (b) = (c);}


//  **********************************************************
//  Types
//  **********************************************************

typedef struct tagDiRenderScanLine
{
  DiInt32 nXStart;
  DiInt32 nXEnd;
  DiInt32 nUStart;
  DiInt32 nUEnd;
  DiInt32 nVStart;
  DiInt32 nVEnd;
  DiInt32 nZStart;
  DiInt32 nZEnd;
} DiRenderScanLine;


//  **********************************************************
//  Static data
//  **********************************************************

static  DiRenderTargetDesc  _dTargetDesc;

static  DiRenderScanLine    *_spScanLines = NULL;
static  DiUInt16            *_pZBuffer = NULL;

static  DiRenderShapeMode   _mShapeMode = diRENDER_MODE_SOLID;

// Check correct Begin/End
static  DiBool              _bRenderPrimitiveValid = FALSE;

//  **********************************************************
//  Functions
//  **********************************************************

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   rpDesc
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderSystemOpen(DiRenderTargetDesc *rpDesc)
{
  DiInt32     nJ;

  DIFUNCTION("DiRenderSystemOpen");

  _dTargetDesc = *rpDesc;

  _spScanLines = (DiRenderScanLine*)DiMemAllocate(
                          sizeof(DiRenderScanLine) * 
                          (_dTargetDesc.nHeight + 2)
                                                 );
  // Fill scan line buffer with default values (full screen)
  for (nJ = 0; nJ < _dTargetDesc.nHeight; nJ++)
  {
    _spScanLines[nJ].nXStart  = 0;
    _spScanLines[nJ].nXEnd    = _dTargetDesc.nWidth - 1;
  }

  // Allocate Z buffer
  _pZBuffer = (DiUInt16*)DiMemAllocate(
                      sizeof(DiUInt16) *
                      _dTargetDesc.nWidth *
                      _dTargetDesc.nHeight
                                      );

  // Invalide to draw something
  _bRenderPrimitiveValid = FALSE;

  DIRETURN(TRUE);
} // end of DiRenderSystemOpen

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderSystemClose(DiVoid)
{
  DIFUNCTION("DiRenderSystemClose");
  DiMemFree(_spScanLines);
  _spScanLines = NULL;
  DiMemFree(_pZBuffer);
  _pZBuffer = NULL;
  DIRETURN(TRUE);
} // end of DiRenderSystemClose

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mMode
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderSetShapeMode(DiRenderShapeMode mMode)
{
  DIFUNCTION("DiRenderSetShapeMode");
  _mShapeMode = mMode;
  DIRETURN(TRUE);
} // end of DiRenderSetShapeMode

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiRenderShapeMode DiRenderGetShapeMode(DiVoid)
{
  DIFUNCTION("DiRenderGetShapeMode");
  DIRETURN(_mShapeMode);
} // end of DiRenderGetShapeMode

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderBegin(DiVoid)
{
  DIFUNCTION("DiRenderBegin");

  DIASSERT(!_bRenderPrimitiveValid );
  _bRenderPrimitiveValid = TRUE;

  // Clear target buffer
  memset( 
          (DiVoid*)_dTargetDesc.npDestBuffer,
          0,
          _dTargetDesc.nWidth * _dTargetDesc.nHeight * (_dTargetDesc.nBPP >> 3)
        );
  // Clear Z buffer
  memset(
          (DiVoid*)_pZBuffer,
          0xffffffff,
          _dTargetDesc.nWidth * _dTargetDesc.nHeight * sizeof(DiUInt16)
        );

  DIRETURN(TRUE);
} // end of DiRenderBegin

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderEnd(DiVoid)
{
  DIFUNCTION("DiRenderEnd");

  DIASSERT(_bRenderPrimitiveValid);
  _bRenderPrimitiveValid = FALSE;

  DIRETURN(TRUE);
} // end of DiRenderEnd


/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   pParams
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderLine(DiRenderLineParams *pParams)
{
  DiInt32     nDeltaX, nDeltaY;
  DiInt32     nAbsDeltaX, nAbsDeltaY;
  DiInt32     nStartX, nStartY;
  DiInt32     nEndX, nEndY;
  DiInt32     nTemp;
  DiInt32     nStep;
  DiInt32     nAccX;
  DiInt32     nAccY;
  DiInt32     nX;
  DiInt32     nY;
  DiInt32     nOffX;
  DiInt32     nOffY;
  DiInt32     nBytesPerPixel;
  DiInt32     nStride;
  DiUInt8     *npDest;

  DIFUNCTION("DiRenderLine");

  DIASSERT(_bRenderPrimitiveValid);

  DIASSERT(pParams->nStartX >= 0);
  DIASSERT(pParams->nStartX < _dTargetDesc.nWidth);
  DIASSERT(pParams->nStartY >= 0);
  DIASSERT(pParams->nStartY < _dTargetDesc.nHeight);

  DIASSERT(pParams->nEndX >= 0);
  DIASSERT(pParams->nEndX < _dTargetDesc.nWidth);
  DIASSERT(pParams->nEndY >= 0);
  DIASSERT(pParams->nEndY < _dTargetDesc.nHeight);

  nStartX = pParams->nStartX;
  nStartY = pParams->nStartY;

  nEndX   = pParams->nEndX;
  nEndY   = pParams->nEndY;

  nDeltaX     = nEndX - nStartX;
  nDeltaY     = nEndY - nStartY;

  if (!(nDeltaX | nDeltaY))
  {
    DIRETURN(TRUE);
  }

  nAbsDeltaX  = nDeltaX & (~(1<<31));  // Kill sign bit
  nAbsDeltaY  = nDeltaY & (~(1<<31));  // Kill sign bit

  nBytesPerPixel  = _dTargetDesc.nBPP >> 3;
  nStride         = _dTargetDesc.nWidth * nBytesPerPixel;

  if (nAbsDeltaX > nAbsDeltaY)
  {
    // Line direction close to Horizontal

    // Sort params in +x direction
    if (nStartX > nEndX)
    {
      diRENDER_SWAP(nStartX, nEndX, nTemp);
      diRENDER_SWAP(nStartY, nEndY, nTemp);
    }

    nStep = (nDeltaY << diRENDER_ACC_BITS) / nDeltaX;
    nAccY = (nStartY << diRENDER_ACC_BITS) + diRENDER_ACC_HALF;
    nOffX = nStartX * nBytesPerPixel;
    for (nX = nStartX; nX <= nEndX; nX ++)
    {
      nY = nAccY >> diRENDER_ACC_BITS;
      npDest = 
                _dTargetDesc.npDestBuffer + 
                nY * nStride + nOffX;
      *((DiInt32*)(npDest)) = pParams->nColor;

      nOffX += nBytesPerPixel;
      nAccY += nStep;
    } // for (all pixel)
  }
  else
  {
    // Line direction close to Vertical

    // Sort params in +y direction
    if (nStartY > nEndY)
    {
      diRENDER_SWAP(nStartX, nEndX, nTemp);
      diRENDER_SWAP(nStartY, nEndY, nTemp);
    }

    nStep = (nDeltaX << diRENDER_ACC_BITS) / nDeltaY;
    nAccX = (nStartX << diRENDER_ACC_BITS) + diRENDER_ACC_HALF;
    nOffY = nStartY * nStride;
    for (nY = nStartY; nY <= nEndY; nY ++)
    {
      nX = nAccX >> diRENDER_ACC_BITS;
      npDest = 
                _dTargetDesc.npDestBuffer + 
                nOffY + nX * nBytesPerPixel;
      *((DiInt32*)(npDest)) = pParams->nColor;

      nOffY += nStride;
      nAccX += nStep;
    } // for (all pixel)
  }

  DIRETURN(TRUE);
} // end of DiRenderLine


/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   pParams
 * @author  Vlad
 * @see     <see>
 */
DiBool DiRenderTriangle(DiRenderTriangleParams *pParams)
{
  register DiUInt32   nFillColor;     // color to fill triangle with
  DiRenderVertex     *vpVertex2;      // aliases of triangle vertices
  DiRenderVertex     *vpVertex1;      //
  DiRenderVertex     *vpVertex0;      //
  DiRenderVertex      sTmpVertex;     // temporary vertex for SWAP
  DiInt32             nPolyMinY;      // lower Y of the triangle
  DiInt32             nPolyMaxY;      // upper Y of the triangle
  DiRenderVertex     *vpLineStart;    // start of line being processed
  DiRenderVertex     *vpLineEnd;      // end of line being processed
  DiInt32             nI, nJ;         // counter
  DiInt32             nDX, nDY, nDZ;  // increments
  DiInt32             nDX02, nDX12;   //
  DiInt32             nDY02, nDY12;   //
  DiInt32             nT;             //
  DiUInt32            aX, kX;         //
  DiUInt32            aZ, kZ;         //
  DiInt32             nX, nY, nZ;     // current X, Y, z
  DiInt32             nXStart, nXEnd; // scanline borders
  DiUInt8            *pVideo;
  register DiUInt8   *pVideoDest;
  DiUInt16           *pZBuff;
  register DiUInt16  *pZBuffDest;
  DiUInt8            *pTargetBuffer;
  DiInt32             nBytesPerPixel;
  DiInt32             nStride;

  DIFUNCTION("DiRenderTriangle");

  DIASSERT(_bRenderPrimitiveValid);

  DIASSERT(_pZBuffer);
  DIASSERT(_spScanLines);

  // Emulate triangle as 3 edges
  if (_mShapeMode == diRENDER_MODE_WIREFRAME)
  {
    DiRenderLineParams    pLine;

    pLine.nColor  = pParams->nColor;

    pLine.nStartX = pParams->vaVertices[0].nX;
    pLine.nStartY = pParams->vaVertices[0].nY;
    pLine.nEndX   = pParams->vaVertices[1].nX;
    pLine.nEndY   = pParams->vaVertices[1].nY;
    DiRenderLine(&pLine);

    pLine.nStartX = pParams->vaVertices[1].nX;
    pLine.nStartY = pParams->vaVertices[1].nY;
    pLine.nEndX   = pParams->vaVertices[2].nX;
    pLine.nEndY   = pParams->vaVertices[2].nY;
    DiRenderLine(&pLine);

    pLine.nStartX = pParams->vaVertices[2].nX;
    pLine.nStartY = pParams->vaVertices[2].nY;
    pLine.nEndX   = pParams->vaVertices[0].nX;
    pLine.nEndY   = pParams->vaVertices[0].nY;
    DiRenderLine(&pLine);

    DIRETURN(TRUE);
  }

  // define fill color as monochrome settings
  pTargetBuffer = _dTargetDesc.npDestBuffer;
  nFillColor    = pParams->nColor;

  // store pointers to vertices
  vpVertex0 = &(pParams->vaVertices[0]);
  vpVertex1 = &(pParams->vaVertices[1]);
  vpVertex2 = &(pParams->vaVertices[2]);

  //////////////////////////////////////////////////////////////////
  //  Sort triangle vertices by such rules:
  //	- Vertex 2 has minimum Y coordinate
  //	- Order of vertex traversal is (2, 0, 1) counterclockwise.
  //////////////////////////////////////////////////////////////////
  if (vpVertex2->nY > vpVertex0->nY)
  {
    diRENDER_SWAP(*vpVertex0, *vpVertex2, sTmpVertex);
  }
  if ( vpVertex2->nY > vpVertex1->nY )
  {
    diRENDER_SWAP(*vpVertex1, *vpVertex2, sTmpVertex);
  }

  nDX12  = vpVertex1->nX - vpVertex2->nX;
  nDX02  = vpVertex0->nX - vpVertex2->nX;
  nDY12  = vpVertex1->nY - vpVertex2->nY;
  nDY02  = vpVertex0->nY - vpVertex2->nY;
  nT    = nDX12 * nDY02 - nDX02 * nDY12;

  if (!nT)
  {
    // collapsed triangle
    DIRETURN(TRUE);
  }

  // clockwise test
  if (nT < 0L)
  {
    diRENDER_SWAP(*vpVertex0, *vpVertex1, sTmpVertex);
  }

  // find poly bounds
  nPolyMinY = vpVertex2->nY;
  nPolyMaxY = (vpVertex0->nY > vpVertex1->nY) ? vpVertex0->nY : vpVertex1->nY;

  if (!(nPolyMinY - nPolyMaxY))
  {
    // collapsed triangle
    DIRETURN(TRUE);
  }

  // fill scan arrays 		
  vpLineStart = vpVertex2;
  vpLineEnd   = vpVertex0;
  for (nI = 3L; nI > 0L; nI--)	// process each line
  {
    nDY = vpLineEnd->nY - vpLineStart->nY;
    if (nDY)
    {
      if (nDY > 0L)
      {
        nY = vpLineStart->nY;

        nDX = (DiInt32)(vpLineEnd->nX - vpLineStart->nX);
        nDZ = (DiInt32)(vpLineEnd->nZ - vpLineStart->nZ);
        aX  = diRENDER_ACC_HALF + ((DiInt32)(vpLineStart->nX) << diRENDER_ACC_BITS);
        aZ  = diRENDER_ACC_HALF + ((DiInt32)(vpLineStart->nZ) << diRENDER_ACC_BITS);
        kX  = (nDX << diRENDER_ACC_BITS) / (DiInt32)(nDY);
        kZ  = (nDZ << diRENDER_ACC_BITS) / (DiInt32)(nDY);

        for (nJ = nDY; nJ > 0; nJ--)
        {
          // fill scan values	
          _spScanLines[nY].nXStart = _spScanLines[nY].nXEnd;
          _spScanLines[nY].nXEnd   = aX;
          _spScanLines[nY].nZStart = _spScanLines[nY].nZEnd;
          _spScanLines[nY].nZEnd   = aZ ;

          // update accuracy coordinates for next point in line	
          aX += kX;
          aZ += kZ;
          nY++;
        } // endfor on line
      }
      else
      {
        nDY = -nDY;
        nY = vpLineEnd->nY;

        nDX = (DiInt32)(vpLineStart->nX - vpLineEnd->nX);
        nDZ = (DiInt32)(vpLineStart->nZ - vpLineEnd->nZ);
        aX  = diRENDER_ACC_HALF + ((DiInt32)(vpLineEnd->nX) << diRENDER_ACC_BITS);
        aZ  = diRENDER_ACC_HALF + ((DiInt32)(vpLineEnd->nZ) << diRENDER_ACC_BITS);
        kX  = (nDX << diRENDER_ACC_BITS) / (DiInt32)(nDY);
        kZ  = (nDZ << diRENDER_ACC_BITS) / (DiInt32)(nDY);

        for (nJ = nDY; nJ > 0; nJ--)
        {
          // fill scan values	
          _spScanLines[nY].nXStart = _spScanLines[nY].nXEnd;
          _spScanLines[nY].nXEnd   = aX ;
          _spScanLines[nY].nZStart = _spScanLines[nY].nZEnd;
          _spScanLines[nY].nZEnd   = aZ ;

          // update accuracy coordinates for next point in line	
          aX += kX;
          aZ += kZ;
          nY++;
        } // endfor on line
      } // endif direction
    } // endif not horizontal

    // advance
    vpLineStart = vpLineEnd;
    vpLineEnd++;
  } // endfor each line

  nBytesPerPixel  = _dTargetDesc.nBPP >> 3;
  nStride         = _dTargetDesc.nWidth * nBytesPerPixel;

  // output scan arrays to virtual screen buffer		            
  pVideo = (DiUInt8*)(pTargetBuffer) + (nPolyMinY * nStride);
  pZBuff = (DiUInt16*)(_pZBuffer) + (nPolyMinY * _dTargetDesc.nWidth);
  for (nY = nPolyMinY; nY < nPolyMaxY; nY++)
  {
    nXStart = _spScanLines[nY].nXStart >> diRENDER_ACC_BITS;
    nXEnd   = _spScanLines[nY].nXEnd   >> diRENDER_ACC_BITS;
    nDX     = nXEnd - nXStart;
    if (nDX > 0)
    {
      pVideoDest  = pVideo + nXStart * nBytesPerPixel;
      pZBuffDest  = pZBuff + nXStart;

      aZ =  _spScanLines[nY].nZStart;
      kZ = (_spScanLines[nY].nZEnd - _spScanLines[nY].nZStart) / nDX;
      for (nX = nDX; nX > 0L; nX--)
      {
        nZ = aZ >> diRENDER_ACC_BITS;
        if (nZ < *pZBuffDest)
        {
          *((DiInt32*)(pVideoDest)) = nFillColor;
          *pZBuffDest               = (DiInt16)nZ;
        }

        // advance
        pVideoDest += nBytesPerPixel;
        pZBuffDest ++;
        aZ += kZ;
      }
    }

    // advance
    pVideo += nStride;
    pZBuff += _dTargetDesc.nWidth;
  }

  DIRETURN(TRUE);
} // end of DiRenderTriangle

// END
