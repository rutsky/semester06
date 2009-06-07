//  **********************************************************
//  FILE NAME   DiScene.c
//  PURPOSE     Scene handling/rendering
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiScene
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include "stdio.h"
#include "string.h"

#include "DiMem.h"
#include "DiErr.h"

#include "DiMain.h"
#include "DiGeo.h"
#include "DiParse.h"
#include "DiRender.h"
#include "DiCamera.h"
#include "DiTime.h"

#include "DiScene.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  DiWorld             *_wpWorld = NULL;

static  DiCamera            *_cpCamera = NULL;

//  **********************************************************
//  Functions
//  **********************************************************

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiWorld *DiSceneGetWorld(DiVoid)
{
  DIFUNCTION("DiSceneGetWorld");
  DIRETURN(_wpWorld);
} // end of DiSceneGetWorld

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiCamera  *DiSceneGetCamera(DiVoid)
{
  DIFUNCTION("DiSceneGetCamera");
  DIRETURN(_cpCamera);
} // end of DiSceneGetCamera

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiSceneMakeCameraLookToWorld(DiVoid)
{
  DiV4d   vCenter;

  DIFUNCTION("DiSceneMakeCameraLookToWorld");
  DiWorldGetCenter(_wpWorld, &vCenter);
  DiCameraLookToPoint(_cpCamera, &vCenter);
  DIRETURN(TRUE);
}



/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpFileName
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiSceneCreate(DiChar *cpFileName)
{
  FILE            *fpFile;
  DiInt32         nFileSize;
  DiChar          *cpFileBuffer;
  DiInt32         nRet;
  DiChar          caFullName[diMAX_PATH];
  DiCameraParams  pParams;
  DiCameraFrame   *fpCamFrame;


  DIFUNCTION("DiSceneCreate");

  DiParseSystemOpen();

  DiMainGetPath(caFullName, diMAX_PATH);
  strcat(caFullName, "Data\\");
  strcat(caFullName, cpFileName);
  fpFile = fopen(caFullName, "rb");
  DIASSERT(fpFile);
  if (!fpFile)
  {
    DIRETURN(FALSE);
  }
  fseek(fpFile, 0L, SEEK_END);
  nFileSize = ftell(fpFile);
  fseek(fpFile, 0L, SEEK_SET);

  cpFileBuffer = (DiChar*)DiMemAllocate(nFileSize + 1);
  DIASSERT(cpFileBuffer);
  if (!cpFileBuffer)
  {
    DIRETURN(FALSE);
  }
  nRet = fread( (DiVoid*)cpFileBuffer, 1, nFileSize, fpFile);
  DIASSERT( nRet == nFileSize );
  fclose(fpFile);
  cpFileBuffer[nFileSize] = 0;

  // Parse file buffer now
  _wpWorld = DiWorldCreate(cpFileBuffer);

  DiMemFree(cpFileBuffer);

  // Search camera data in scene
  fpCamFrame  = DiWorldGetCameraFrame(_wpWorld);
  DIASSERT(fpCamFrame);

  pParams.rNearZ    = fpCamFrame->cCamera.rNear;
  pParams.rFarZ     = fpCamFrame->cCamera.rFar;

  pParams.rMinZ     = 1.0f;
  pParams.rMaxZ     = 65535.0f;

  pParams.rHx       = 1.0f;
  pParams.rHy       = 1.0f;

  pParams.rScreenX  = (DiFloat)diMAIN_WINDOW_WIDTH;
  pParams.rScreenY  = (DiFloat)diMAIN_WINDOW_HEIGHT;

  _cpCamera = DiCameraCreate(&pParams);

  DiCameraConnectToFrame(_cpCamera, (DiFrameBase*)fpCamFrame);

  DiSceneMakeCameraLookToWorld();

  DIRETURN(TRUE);
} // end of DiSceneCreate

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiSceneDestroy(DiVoid)
{
  DIFUNCTION("DiSceneDestroy");
  DiWorldDestroy(_wpWorld);
  DiParseSystemClose();
  DiCameraDestroy(_cpCamera);

  DIRETURN(TRUE);
} // end of DiSceneDestroy

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   npDestBuffer
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiSceneRender(DiUInt8 *npDestBuffer)
{
  DIFUNCTION("DiSceneRender");

  DiTimeUpdate();
  DiRenderBegin();
  {
    DiCameraRenderBegin(_cpCamera);
    {
      DiWorldRender(_wpWorld);
    }
    DiCameraRenderEnd(_cpCamera);
  }
  DiRenderEnd();


  /*
  DiRenderBegin();
  {
    DiRenderLineParams      pLine;
    DiRenderTriangleParams  pTri;

    pLine.nColor  = 0x00ff0000;  // Red
    pLine.nStartX = 10;
    pLine.nStartY = 10;
    pLine.nEndX   = 400;
    pLine.nEndY   = 100;

    DiRenderLine(&pLine);

    pLine.nColor  = 0x0000ffff;  // Cyan
    pLine.nStartX = 10;
    pLine.nStartY = 10;
    pLine.nEndX   = 100;
    pLine.nEndY   = 400;

    DiRenderLine(&pLine);

    pTri.nColor           = 0x00ffff00;   // Yellow

    pTri.vaVertices[0].nX = 40;
    pTri.vaVertices[0].nY = 40;
    pTri.vaVertices[0].nZ = 256;

    pTri.vaVertices[1].nX = 40;
    pTri.vaVertices[1].nY = 100;
    pTri.vaVertices[1].nZ = 256;

    pTri.vaVertices[2].nX = 200;
    pTri.vaVertices[2].nY = 40;
    pTri.vaVertices[2].nZ = 256;

    DiRenderTriangle(&pTri);
  }
  DiRenderEnd();
  */

  DIRETURN(TRUE);
} // end of DiSceneRender


// END
