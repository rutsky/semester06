//  **********************************************************
//  FILE NAME   DiRender.h
//  PURPOSE     Render primitives
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiRender
//  NOTES
//  **********************************************************

#ifndef _DIRENDER_H_
#define _DIRENDER_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTransf.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

typedef enum   tagDiRenderShapeMode
{
  diRENDER_MODE_SOLID       = 0,
  diRENDER_MODE_WIREFRAME   = 1
} DiRenderShapeMode;

typedef struct tagDiRenderTargetDesc
{
  DiUInt8   *npDestBuffer;
  DiInt32   nWidth;
  DiInt32   nHeight;
  DiInt32   nBPP;
} DiRenderTargetDesc;

typedef struct tagDiRenderLineParams
{
  DiInt32   nStartX;
  DiInt32   nStartY;
  DiInt32   nEndX;
  DiInt32   nEndY;
  DiInt32   nColor;
} DiRenderLineParams;

typedef struct tagDiRenderVertex
{
  DiInt32 nX;          // Without accuracy
  DiInt32 nY;          //
  DiInt32 nZ;          // 

  DiInt32 nU;          // 
  DiInt32 nV;          //

  DiInt32 naPad[3];    // Just to align structure size on 32 bytes
} DiRenderVertex;

typedef struct tagDiRenderTriangleParams
{
  DiRenderVertex  vaVertices[3];
  DiInt32         nColor;
} DiRenderTriangleParams;


//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiBool DiRenderSystemOpen(  DiRenderTargetDesc *rpDesc);
extern  DiBool DiRenderSystemClose( DiVoid);

extern  DiBool DiRenderSetShapeMode(DiRenderShapeMode mMode);
extern  DiRenderShapeMode DiRenderGetShapeMode(DiVoid);

extern  DiBool DiRenderLine(        DiRenderLineParams *pParams);
extern  DiBool DiRenderTriangle(    DiRenderTriangleParams *pParams);

extern  DiBool DiRenderBegin(DiVoid);
extern  DiBool DiRenderEnd(DiVoid);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DIRENDER_H_
