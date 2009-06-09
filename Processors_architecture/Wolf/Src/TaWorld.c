//  **********************************************************
//  FILE NAME   TaWorld.c
//  PURPOSE     World (rooms) definitions
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
#include "TaMain.h"
#include "TaTextur.h"
#include "TaCamera.h"
#include "TaWorld.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

// Map of "level"
// 0 means no wall
// 1 means wall with texture1
// 2 means wall with texture2
static  TaInt32   _taMap[taWORLD_NUM_CELLS * taWORLD_NUM_CELLS];

static  TaInt32 _nStartPointX = 1;
static  TaInt32 _nStartPointZ = 1;

//  **********************************************************
//  Functions
//  **********************************************************

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaBool  _taWorldReadFile(void)
{
  HFILE               hFile;
  TaInt32             nFileSize;
  TaChar              *cpFileData;
  TaChar              *cpSrc;
  TaInt32             *npDest;
  TaInt32             nI, nJ;
  TaChar              *cpArgs;
  TaChar              caFileName[32];

  cpArgs = TaMainGetArgs();
  if (!cpArgs)
  {
    strcpy(caFileName, "data\\Level01.txt");
  }
  else
  {
    strcpy(caFileName, "data\\");
    strcat(caFileName, cpArgs);
  }

  hFile = _lopen(caFileName, OF_READ);
  if (hFile == HFILE_ERROR)
  {
    return (taFAIL);
  }
  nFileSize = _llseek(hFile, 0, FILE_END);
  _llseek(hFile, 0, FILE_BEGIN);

  cpFileData = (TaChar*)TaMemAllocate(nFileSize + 2);
  if (!cpFileData)
  {
    _lclose(hFile);
    return (taFAIL);
  }
  _lread(hFile, cpFileData, nFileSize);
  _lclose(hFile);

  cpSrc  = cpFileData;
  npDest = _taMap;
  for (nJ = 0; nJ < taWORLD_NUM_CELLS; nJ++)
  {
    for (nI = 0; nI < taWORLD_NUM_CELLS; nI++)
    {
      // default value: no wall here
      *npDest = -1;
      switch(*cpSrc)
      {
        case '1':       // Texture case
        {
          *npDest = 0; 
          break;
        }
        case '2':       // Texture case
        {
          *npDest = 1; 
          break;
        }
        case '3':       // Texture case
        {
          *npDest = 2; 
          break;
        }
        case '4':       // Texture case
        {
          *npDest = 3; 
          break;
        }
        case '*':       // Start point
        {
          _nStartPointX = nI;
          _nStartPointZ = nJ;
          break;
        }
      } // switch (symbol)
      cpSrc ++;
      npDest++;
    } // for (all hor wall units)
    // read for CR,LF
    while (*cpSrc != '\n')
      cpSrc ++;
    cpSrc ++;
  } // for (all hor lines)
  TaMemFree(cpFileData);

  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaWorldCreate(void)
{
  TaBool    bOK;

  bOK = _taWorldReadFile();
  if (bOK != taSUCCESS)
  {
    return (taFAIL);
  }

  bOK = TaTextureOpen();
  if (bOK != taSUCCESS)
  {
    return (taFAIL);
  }
  bOK = TaCameraOpen(
                    (TaFloat)(_nStartPointX * taWORLD_CELL_LENGTH + (taWORLD_CELL_LENGTH/2)),
                    (TaFloat)(_nStartPointZ * taWORLD_CELL_LENGTH + (taWORLD_CELL_LENGTH/2))
                  );
  if (bOK != taSUCCESS)
  {
    return (taFAIL);
  }

  return(taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaWorldDestroy(void)
{
  TaCameraClose();
  TaTextureClose();
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid _taWorldClearScreen(TaUInt8 *cpPixels)
{
  memset(cpPixels, 0, taWINDOW_WIDTH * taWINDOW_HEIGHT * 2);
}

// NAME        _worldTestRender
// PURPOSE     Render stretched vertically 0 texture on screen
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid  _taWorldTestRender(TaUInt8 *cpPixels)
{
  TaInt32     nI;

  for (nI = 0; nI < taWINDOW_WIDTH; nI++)
  {
    TaTextureDrawVertLine(
                        cpPixels,
                        0,
                        taWINDOW_HEIGHT - 1,
                        0,
                        (nI & (taTEXTURE_SIDE - 1)),
                        nI
                       );
  }
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid  _taWorldRender(TaUInt8  *cpPixels)
{
  TaInt32     nX;
  TaV3d       vRayNormal;
  TaV3d       vCamPos;
  TaV3d       vCamAt;
  TaV3d       vCamRight;
  TaInt32     nStartCellX, nStartCellZ;
  TaInt32     nCellX, nCellZ;
  TaInt32     nCellStepX, nCellStepZ;
  TaFloat     rRayNormalZPerX;
  TaFloat     rRayNormalXPerZ;
  TaInt32     nIterations;
  TaBool      bIntersectionFound;
  TaFloat     rX, rZ;
  TaBool      bVertIntersectFound;
  TaV3d       vIntersection;
  TaInt32     nTexX;
  TaV3d       v;
  TaV3d       vIntCamera;
  TaFloat     rScreenY;
  TaBool      bOK;
  TaInt32     nCellSearchX, nCellSearchZ;

  TaCameraGetPosition(&vCamPos);
  TaCameraGetOrientation(&vCamAt, &vCamRight);

  // Get cell index for this camera position
  nStartCellX = (TaInt32)(vCamPos.x) >> taWORLD_CELL_LENGTH_DEGREE;
  nStartCellZ = (TaInt32)(vCamPos.z) >> taWORLD_CELL_LENGTH_DEGREE;


  // Check that start position is not inside wall
  taASSERT(_taMap[nStartCellZ * taWORLD_NUM_CELLS + nStartCellX] == -1);

  // make compiler happy
  vIntersection.x = vIntersection.y = vIntersection.z = 0.0f;

  for (nX = 0; nX < taWINDOW_WIDTH; nX++)
  {
    // Get ray direction
    TaCameraGetRay(nX, &vRayNormal);

    // If normal along any axis slightly move it to avoid
    // div by zero problem
    if (vRayNormal.x == 0.0f)
      vRayNormal.x = 1e-8f;
    if (vRayNormal.z == 0.0f)
      vRayNormal.z = 1e-8f;

    // Find tg and ctg for ray normal
    rRayNormalZPerX = vRayNormal.z / vRayNormal.x;
    rRayNormalXPerZ = vRayNormal.x / vRayNormal.z;

    nCellX = nStartCellX;
    nCellZ = nStartCellZ;

    // Define steps for cell indices
    // And closest probable wall indices
    if (vRayNormal.x > 0.0f && vRayNormal.z > 0.0f)
    {
      nCellStepX = 1;
      nCellStepZ = 1;
    }
    else if (vRayNormal.x < 0.0f && vRayNormal.z > 0.0f)
    {
      nCellStepX = -1;
      nCellStepZ = 1;
    }
    else if (vRayNormal.x < 0.0f && vRayNormal.z < 0.0f)
    {
      nCellStepX = -1;
      nCellStepZ = -1;
    }
    else
    {
      nCellStepX = 1;
      nCellStepZ = -1;
    }

    nIterations         = 0;
    bIntersectionFound  = FALSE;
    while (TRUE)
    {
      // Check infinite loop
      nIterations ++;
      if(nIterations > taWORLD_NUM_CELLS*2)
      {
        taASSERT(nIterations <= taWORLD_NUM_CELLS*2);
        break;
      }

      nCellSearchX = nCellX;
      if (nCellStepX > 0)
        nCellSearchX++;
      nCellSearchZ = nCellZ;
      if (nCellStepZ > 0)
        nCellSearchZ++;

      // Calc probable intersection of ray and horiz and vert wall
      rZ = vCamPos.z + 
          rRayNormalZPerX * 
          ( (nCellSearchX<<taWORLD_CELL_LENGTH_DEGREE) - vCamPos.x);
      rX = vCamPos.x + 
          rRayNormalXPerZ * 
          ( (nCellSearchZ<<taWORLD_CELL_LENGTH_DEGREE) - vCamPos.z);
      // check intersection with vert wall (rZ)
      // it should be in 
      bVertIntersectFound = FALSE;
      if (nCellStepZ > 0)
      {
        if (rZ <= (nCellSearchZ<<taWORLD_CELL_LENGTH_DEGREE))
          bVertIntersectFound = TRUE;
      }
      else
      {
        if (rZ >= (nCellSearchZ<<taWORLD_CELL_LENGTH_DEGREE))
          bVertIntersectFound = TRUE;
      }
      if (bVertIntersectFound)
      {
        vIntersection.x = (TaFloat)(nCellSearchX<<taWORLD_CELL_LENGTH_DEGREE);
        vIntersection.z = rZ;
        nCellX += nCellStepX;
        if (_taMap[nCellZ * taWORLD_NUM_CELLS + nCellX] != -1)
        {
          bIntersectionFound = TRUE;
          //nTexX = (TaInt32)(rZ) & (taWORLD_CELL_LENGTH - 1);
          //nTexX = nTexX * taTEXTURE_SIDE / taWORLD_CELL_LENGTH;
          nTexX = (TaInt32)(rZ * taTEXTURE_SIDE / taWORLD_CELL_LENGTH);
          nTexX &= (taTEXTURE_SIDE - 1);
          break;
        }
        taASSERT(nCellX >= 0);
        taASSERT(nCellX < taWORLD_NUM_CELLS);
      }
      else
      {
        // Additional check for intersection integrity
        bOK = FALSE;
        if (nCellStepX > 0)
        {
          if (rX <= (nCellSearchX<<taWORLD_CELL_LENGTH_DEGREE))
            bOK = TRUE;
        }
        else
        {
          if (rX >= (nCellSearchX<<taWORLD_CELL_LENGTH_DEGREE))
            bOK = TRUE;
        }
        taASSERT(bOK);

        // Intersection with horizontal possible wall found
        vIntersection.x = rX;
        vIntersection.z = (TaFloat)(nCellSearchZ<<taWORLD_CELL_LENGTH_DEGREE);
        nCellZ += nCellStepZ;
        if (_taMap[nCellZ * taWORLD_NUM_CELLS + nCellX] != -1)
        {
          bIntersectionFound = TRUE;
          //nTexX = (TaInt32)(rX) & (taWORLD_CELL_LENGTH - 1);
          //nTexX = nTexX * taTEXTURE_SIDE / taWORLD_CELL_LENGTH;
          nTexX = (TaInt32)(rX * taTEXTURE_SIDE / taWORLD_CELL_LENGTH);
          nTexX &= (taTEXTURE_SIDE - 1);
          break;
        }

        taASSERT(nCellZ >= 0);
        taASSERT(nCellZ < taWORLD_NUM_CELLS);
      }

    }
    if (!bIntersectionFound)
      continue;

    // Transform intersection point into
    // camera coordinate system
    TaV3dSub(&vIntersection, &vCamPos, &v);
    //vIntCamera.x = TaV3dDotProduct(&v, &vCamRight);
    vIntCamera.z = TaV3dDotProduct(&v, &vCamAt);

    // Find screen Y projections of vertical line
    rScreenY = 
                (taWINDOW_HEIGHT/2) * (taWALL_HEIGHT/2) * 
                taVERT_PROJ_KOEF / vIntCamera.z;

    // Draw vertical line corresponded to ray & wall intersection
    TaTextureDrawVertLine(
                        cpPixels,
                        (TaInt32)(taWINDOW_HEIGHT/2 - rScreenY),
                        (TaInt32)(taWINDOW_HEIGHT/2 + rScreenY),
                        _taMap[nCellZ * taWORLD_NUM_CELLS + nCellX],
                        nTexX,
                        nX
                       );

  } // for (all screen vertical lines)
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaWorldUpdate(TaUInt8 *cpPixels)
{

  // Clear screen
  _taWorldClearScreen(cpPixels);

  // Test
  //_taWorldTestRender(cpPixels);

  // Draw wolfenstein world
  _taWorldRender(cpPixels);

  return(taSUCCESS);
}

// END
