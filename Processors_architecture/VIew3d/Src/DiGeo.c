//  **********************************************************
//  FILE NAME   DiGeo.c
//  PURPOSE     Geometry structure
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiGeometry
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <math.h>

#include "DiMem.h"
#include "DiErr.h"
#include "DiParse.h"
#include "DiCamera.h"
#include "DiClip.h"
#include "DiRender.h"
#include "DiMain.h"

#include "DiGeo.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  DiMatrix4d  _mMax2Di;
static  DiMatrix4d  _mDi2Max;

// Dummy colors for different materials
static  DiInt32     _naMatColors[] =
{
  0x00ff0000,
  0x007f0000,
  0x007f7f00,
  0x0000ff00,
  0x00007f00,
  0x00007f7f,
  0x000000ff,
  0x0000007f
};

//  **********************************************************
//  Functions
//  **********************************************************


/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @param   fpBase
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoAddFrameToWorld(
                                        DiWorld     *wpWorld, 
                                        DiFrameBase *fpBase
                                     )
{
  DiFrameBase   *fpCur;

  DIFUNCTION("_diGeoAddFrameToWorld");
  DIASSERT(wpWorld);
  DIASSERT(fpBase);

  if (!wpWorld->fpFrame)
  {
    wpWorld->fpFrame = fpBase;
    DIRETURN(TRUE);
  }
  // search for end of list
  for (
        fpCur = wpWorld->fpFrame; 
        fpCur->fpNext; 
        fpCur = fpCur->fpNext
      );

  fpCur->fpNext = fpBase;

  DIRETURN(TRUE);
} // end of _diGeoAddFrameToWorld

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
static  DiGeometryFrame *_diGeoCreateGeometryFrame(DiVoid)
{
  DiGeometryFrame *fpFrame;

  DIFUNCTION("_diGeoCreateGeometryFrame");
  fpFrame = (DiGeometryFrame*)DiMemAllocate(sizeof(DiGeometryFrame));
  fpFrame->fBase.fpNext     = NULL;
  fpFrame->fBase.fpSon      = NULL;
  fpFrame->fBase.caName[0]  = 0;
  fpFrame->fBase.tType      = diFRAME_TYPE_GEOMETRY;

  DIRETURN(fpFrame);
} // end of _diGeoCreateGeometryFrame

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
static  DiCameraFrame *_diGeoCreateCameraFrame(DiVoid)
{
  DiCameraFrame *fpFrame;

  DIFUNCTION("_diGeoCreateCameraFrame");
  fpFrame = (DiCameraFrame*)DiMemAllocate(sizeof(DiCameraFrame));

  fpFrame->fBase.fpNext     = NULL;
  fpFrame->fBase.fpSon      = NULL;
  fpFrame->fBase.caName[0]  = 0;
  fpFrame->fBase.tType      = diFRAME_TYPE_CAMERA;

  DIRETURN(fpFrame);
} // end of _diGeoCreateCameraFrame


/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   gpGeoFrame
 * @author  Vlad
 * @see     <see>
 */
static  DiChar  *_diGeoReadGeometry(
                                      DiChar          *cpBuffer, 
                                      DiGeometryFrame *gpGeoFrame
                                   )
{
  DiChar          *cpS;
  DiKeywordType   ktType;
  DiGeometry      *gpGeometry;
  DiFrameBase     *fpBase;
  DiInt32         nI;
  DiInt32         nDummy;
  DiMatrix4d      *mpMatrix;

  DIFUNCTION("_diGeoReadGeometry");

  gpGeometry  = &gpGeoFrame->gGeometry;
  fpBase      = &gpGeoFrame->fBase;

  cpS = cpBuffer;
  // Wait open brace
  cpS = DiParseWaitForChar(cpS, '{');
  {
    // Wait for *NODE_NAME
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_NODE_NAME);

    cpS = DiParseReadQuoted(cpS, fpBase->caName, diGEOMETRY_MAX_FRAME_NAME);

    // Wait for *NODE_TM
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_NODE_TM);
    // Wait open brace
    cpS = DiParseWaitForChar(cpS, '{');
    {

      mpMatrix = (DiMatrix4d*)&(fpBase->mGTM);

      // Search for TM_ROW0
      cpS = DiParseWaitForString( cpS, "TM_ROW0");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][0]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][0]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][0]);
      mpMatrix->raMatrix[3][0] = 0.0f;

      // Search for TM_ROW1
      cpS = DiParseWaitForString( cpS, "TM_ROW1");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][1]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][1]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][1]);
      mpMatrix->raMatrix[3][1] = 0.0f;

      // Search for TM_ROW2
      cpS = DiParseWaitForString( cpS, "TM_ROW2");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][2]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][2]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][2]);
      mpMatrix->raMatrix[3][2] = 0.0f;

      // Search for TM_ROW3
      cpS = DiParseWaitForString( cpS, "TM_ROW3");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][3]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][3]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][3]);
      mpMatrix->raMatrix[3][3] = 1.0f;

    }
    // Wait close brace
    cpS = DiParseWaitForChar(cpS, '}');

    // Wait for *MESH
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_MESH);

    // Wait open brace
    cpS = DiParseWaitForChar(cpS, '{');
    {
      // Search for *MESH_NUMVERTEX
      cpS = DiParseWaitForString( cpS, "MESH_NUMVERTEX");
      cpS = DiParseReadInt32( cpS, &gpGeometry->nNumVertices);

      // Search for *MESH_NUMFACES
      cpS = DiParseWaitForString( cpS, "MESH_NUMFACES");
      cpS = DiParseReadInt32( cpS, &gpGeometry->nNumTriangles);

      // Wait for *MESH_VERTEX_LIST
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_MESH_VERTEX_LIST);

      // Allocate memory
      gpGeometry->vpVertices = (DiV4d*)DiMemAllocate(
                                  sizeof(DiV4d) * gpGeometry->nNumVertices
                                                    );
      gpGeometry->vpNormals = (DiV4d*)DiMemAllocate(
                                  sizeof(DiV4d) * gpGeometry->nNumVertices
                                                    );
      gpGeometry->tpTriangles = (DiTriangle*)DiMemAllocate(
                                  sizeof(DiTriangle) * gpGeometry->nNumTriangles
                                                          );

      // Read vertex list
      cpS = DiParseWaitForChar(cpS, '{');
      {
        for (nI = 0; nI < gpGeometry->nNumVertices; nI++)
        {
          // Wait for *MESH_VERTEX
          cpS = DiParseWaitForChar(cpS, '*');
          cpS = DiParseReadKeyword(cpS, &ktType);
          DIASSERT(ktType == diKEYWORD_TYPE_MESH_VERTEX);

          cpS = DiParseReadInt32( cpS, &nDummy);
          DIASSERT(nDummy == nI);
          cpS = DiParseReadFloat( cpS, &gpGeometry->vpVertices[nI].x);
          cpS = DiParseReadFloat( cpS, &gpGeometry->vpVertices[nI].y);
          cpS = DiParseReadFloat( cpS, &gpGeometry->vpVertices[nI].z);
          gpGeometry->vpVertices[nI].w = 1.0f;  // W is always 1.0f
        }
      }
      // Wait close brace
      cpS = DiParseWaitForChar(cpS, '}');

      // Wait for *MESH_FACE_LIST
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_MESH_FACE_LIST);

      // Read triangle list
      cpS = DiParseWaitForChar(cpS, '{');
      {
        for (nI = 0; nI < gpGeometry->nNumTriangles; nI++)
        {
          // Wait for *MESH_FACE
          cpS = DiParseWaitForChar(cpS, '*');
          cpS = DiParseReadKeyword(cpS, &ktType);
          DIASSERT(ktType == diKEYWORD_TYPE_MESH_FACE);

          cpS = DiParseWaitForChar(cpS, ':');
          cpS = DiParseWaitForChar(cpS, ':');

          cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naVertexIndices[0]);
          cpS = DiParseWaitForChar(cpS, ':');
          cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naVertexIndices[1]);
          cpS = DiParseWaitForChar(cpS, ':');
          cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naVertexIndices[2]);

          // Wait for *MESH_SMOOTHING
          cpS = DiParseWaitForChar(cpS, '*');
          cpS = DiParseReadKeyword(cpS, &ktType);
          DIASSERT(ktType == diKEYWORD_TYPE_MESH_SMOOTHING);

          // Wait for *MESH_MTLID
          cpS = DiParseWaitForChar(cpS, '*');
          cpS = DiParseReadKeyword(cpS, &ktType);
          DIASSERT(ktType == diKEYWORD_TYPE_MESH_MTLID);

          DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].nMaterialIndex);

        } // for (all triangles)
      }
      // Wait close brace
      cpS = DiParseWaitForChar(cpS, '}');

      // Wait for *MESH_NUMTVERTEX
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_MESH_NUMTVERTEX);

      cpS = DiParseReadInt32(cpS, &gpGeometry->nNumTexCoords);
      if (gpGeometry->nNumTexCoords)
      {
        // Allocate memory for tex coords
        gpGeometry->uvpTexCoords = (DiUV*)DiMemAllocate(
                                      sizeof(DiUV)*
                                      gpGeometry->nNumTexCoords
                                                       );
        // Wait for *MESH_TVERTLIST
        cpS = DiParseWaitForChar(cpS, '*');
        cpS = DiParseReadKeyword(cpS, &ktType);
        DIASSERT(ktType == diKEYWORD_TYPE_MESH_TVERTLIST);

        // Read tex coords
        cpS = DiParseWaitForChar(cpS, '{');
        {
          for (nI = 0; nI < gpGeometry->nNumTexCoords; nI++)
          {
            // Wait for *MESH_TVERT
            cpS = DiParseWaitForChar(cpS, '*');
            cpS = DiParseReadKeyword(cpS, &ktType);
            DIASSERT(ktType == diKEYWORD_TYPE_MESH_TVERT);

            cpS = DiParseReadInt32(cpS, &nDummy);
            DIASSERT(nDummy == nI);

            cpS = DiParseReadFloat( cpS, &gpGeometry->uvpTexCoords[nI].u);
            cpS = DiParseReadFloat( cpS, &gpGeometry->uvpTexCoords[nI].v);

            cpS = DiParseReadInt32(cpS, &nDummy);

          }
        }
        // Wait close brace
        cpS = DiParseWaitForChar(cpS, '}');

        // Wait for *MESH_NUMTVFACES
        cpS = DiParseWaitForChar(cpS, '*');
        cpS = DiParseReadKeyword(cpS, &ktType);
        DIASSERT(ktType == diKEYWORD_TYPE_MESH_NUMTVFACES);

        cpS = DiParseReadInt32(cpS, &nDummy);
        DIASSERT(nDummy == gpGeometry->nNumTriangles);

        // Wait for *MESH_TFACELIST
        cpS = DiParseWaitForChar(cpS, '*');
        cpS = DiParseReadKeyword(cpS, &ktType);
        DIASSERT(ktType == diKEYWORD_TYPE_MESH_TFACELIST);

        // Read tex indices
        cpS = DiParseWaitForChar(cpS, '{');
        {
          for (nI = 0; nI < gpGeometry->nNumTriangles; nI++)
          {
            // Wait for *MESH_TFACE
            cpS = DiParseWaitForChar(cpS, '*');
            cpS = DiParseReadKeyword(cpS, &ktType);
            DIASSERT(ktType == diKEYWORD_TYPE_MESH_TFACE);

            cpS = DiParseReadInt32(cpS, &nDummy);
            DIASSERT(nDummy == nI);

            cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naTexIndices[0]);
            cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naTexIndices[1]);
            cpS = DiParseReadInt32(cpS, &gpGeometry->tpTriangles[nI].naTexIndices[2]);

          }
        }
        // Wait close brace
        cpS = DiParseWaitForChar(cpS, '}');

      }
      else
      {
        gpGeometry->uvpTexCoords = NULL;
      }

    } // *MESH
    // Wait close brace
    cpS = DiParseWaitForChar(cpS, '}');
  } // *GEOMOBJECT
  // Wait close brace
  cpS = DiParseWaitForChar(cpS, '}');

  DIRETURN(cpS);
}

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   fpCamFrame
 * @author  Vlad
 * @see     <see>
 */
static  DiChar  *_diGeoReadCamera(
                                    DiChar        *cpBuffer, 
                                    DiCameraFrame *fpCamFrame
                                 )
{
  DiChar          *cpS;
  DiKeywordType   ktType;
  DiCameraValues  *cpCamera;
  DiFrameBase     *fpBase;
  DiMatrix4d      *mpMatrix;

  DIFUNCTION("_diGeoReadCamera");
  cpS = cpBuffer;

  cpCamera  = &fpCamFrame->cCamera;
  fpBase    = &fpCamFrame->fBase;

  // Wait open brace
  cpS = DiParseWaitForChar(cpS, '{');
  {
    // Wait for *NODE_NAME
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_NODE_NAME);

    cpS = DiParseReadQuoted(cpS, fpBase->caName, diGEOMETRY_MAX_FRAME_NAME);

    // Wait for *CAMERA_TYPE
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_TYPE);

    // Wait for *NODE_TM
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_NODE_TM);

    // Wait open brace
    cpS = DiParseWaitForChar(cpS, '{');
    {
      mpMatrix = (DiMatrix4d*)&(fpBase->mGTM);

      // Search for TM_ROW0
      cpS = DiParseWaitForString( cpS, "TM_ROW0");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][0]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][0]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][0]);
      mpMatrix->raMatrix[3][0] = 0.0f; 

      // Search for TM_ROW1
      cpS = DiParseWaitForString( cpS, "TM_ROW1");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][1]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][1]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][1]);
      mpMatrix->raMatrix[3][1] = 0.0f;

      // Search for TM_ROW2
      cpS = DiParseWaitForString( cpS, "TM_ROW2");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][2]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][2]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][2]);
      mpMatrix->raMatrix[3][2] = 0.0f;

      // Search for TM_ROW3
      cpS = DiParseWaitForString( cpS, "TM_ROW3");
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[0][3]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[1][3]);
      cpS = DiParseReadFloat( cpS, &mpMatrix->raMatrix[2][3]);
      mpMatrix->raMatrix[3][3] = 1.0f;

    }
    // Wait close brace
    cpS = DiParseWaitForChar(cpS, '}');

    // Wait for *CAMERA_SETTINGS
    cpS = DiParseWaitForChar(cpS, '*');
    cpS = DiParseReadKeyword(cpS, &ktType);
    DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_SETTINGS);

    // Wait open brace
    cpS = DiParseWaitForChar(cpS, '{');
    {
      // Wait for *TIMEVALUE
      cpS = DiParseWaitForChar(cpS, '*');

      // Wait for *CAMERA_HITHER
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_HITHER);

      // This is really near clip value
      cpS = DiParseReadFloat(cpS, &cpCamera->rNear);

      // Wait for *CAMERA_YON
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_YON);

      // This is really far clip value
      cpS = DiParseReadFloat(cpS, &cpCamera->rFar);

      // Wait for *CAMERA_NEAR
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_NEAR);

      // Wait for *CAMERA_FAR
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_FAR);

      // Wait for *CAMERA_FOV
      cpS = DiParseWaitForChar(cpS, '*');
      cpS = DiParseReadKeyword(cpS, &ktType);
      DIASSERT(ktType == diKEYWORD_TYPE_CAMERA_FOV);
      cpS = DiParseReadFloat(cpS, &cpCamera->rFieldOfView);

    }
    // Wait close brace
    cpS = DiParseWaitForChar(cpS, '}');

  } // *CAMERAOBJECT
  // Wait close brace
  cpS = DiParseWaitForChar(cpS, '}');


  DIRETURN(cpS);
} // end of _diGeoReadCamera

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   fpBase
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoFrameCallback(DiFrameBase *fpBase, DiVoid *pA)
{
  DiMatrix4d    mTmp;

  DIFUNCTION("_diGeoFrameCallback");

  DiMatrixMultiply( &_mMax2Di, &fpBase->mGTM, &mTmp);
  DiMatrixMultiply( &mTmp, &_mDi2Max, &fpBase->mGTM);

  DIRETURN(TRUE);
} // end of _diGeoFrameCallback


/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   gfpFrame
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoFrameCoordCallback(DiGeometryFrame *gfpFrame, DiVoid *pA)
{
  DiGeometry    *gpGeometry;
  DiMatrix4d    mInv;
  DiV4d         *vpVerticesTemp;

  DIFUNCTION("_diGeoFrameCoordCallback");
  gpGeometry  = &gfpFrame->gGeometry;

  DiMatrixInvert(&gfpFrame->fBase.mGTM, &mInv);

  vpVerticesTemp = (DiV4d*)DiMemAllocateTmp(
        sizeof(DiV4d)*gpGeometry->nNumVertices);
  DIASSERT(vpVerticesTemp);

  DiMatrixTransformPoints(
                          &_mMax2Di,
                          gpGeometry->nNumVertices,
                          gpGeometry->vpVertices,
                          vpVerticesTemp
                         );
  DiMatrixTransformPoints(
                          &mInv,
                          gpGeometry->nNumVertices,
                          vpVerticesTemp,
                          gpGeometry->vpVertices
                         );
  DiMemFreeTmp(vpVerticesTemp);

  DIRETURN(TRUE);
} // end of _diGeoFrameCoordCallback

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpTextBuffer
 * @author  Vlad
 * @see     <see>
 */
DiWorld *DiWorldCreate(DiChar *cpTextBuffer)
{
  DiWorld           *wpWorld;
  DiChar            *cpBuffer;
  DiKeywordType     ktType;
  DiInt32           nInt;
  DiChar            caString[64];
  DiGeometryFrame   *gpGeoFrame;
  DiCameraFrame     *fpCamFrame;

  DIFUNCTION("DiWorldCreate");
  wpWorld = (DiWorld*)DiMemAllocate(sizeof(DiWorld));
  wpWorld->fpFrame = NULL;

  cpBuffer = cpTextBuffer;
  while (TRUE)
  {
    cpBuffer  = DiParseWaitForChar(cpBuffer, '*');
    if (!cpBuffer)
      break;
    cpBuffer  = DiParseReadKeyword(cpBuffer, &ktType);
    if (!cpBuffer)
      break;

    switch (ktType)
    {
      case diKEYWORD_TYPE_HEADER:
      {
        cpBuffer = DiParseReadInt32(cpBuffer, &nInt);
        break;
      }
      case diKEYWORD_TYPE_COMMENT:
      {
        cpBuffer = DiParseReadQuoted(cpBuffer, caString, 64);
        break;
      }
      case diKEYWORD_TYPE_SCENE:
      {
        cpBuffer = DiParseWaitForChar(cpBuffer, '{');
        if (!cpBuffer)
          break;
        cpBuffer = DiParseWaitForChar(cpBuffer, '}');
        if (!cpBuffer)
          break;

        break;
      }
      case diKEYWORD_TYPE_MATERIAL_LIST:
      {
        // This is too incorrect code
        // really need to parse material properties
        cpBuffer = DiParseSkipBlock(cpBuffer);

        break;
      }
      case diKEYWORD_TYPE_GEOMOBJECT:
      {
        gpGeoFrame = _diGeoCreateGeometryFrame();
        cpBuffer = _diGeoReadGeometry(cpBuffer, gpGeoFrame);

        _diGeoAddFrameToWorld(wpWorld, &gpGeoFrame->fBase);

        break;
      }
      case diKEYWORD_TYPE_CAMERAOBJECT:
      {
        fpCamFrame  = _diGeoCreateCameraFrame();
        cpBuffer    = _diGeoReadCamera(cpBuffer, fpCamFrame);
        _diGeoAddFrameToWorld(wpWorld, &fpCamFrame->fBase);

        break;
      }
    } // switch
    if (!cpBuffer)
      break;
  } // forever

  // Find 2 matrices for coord system change
  DiMatrixIdentity( &_mMax2Di );
  _mMax2Di.raMatrix[1][1] =  0.0f;
  _mMax2Di.raMatrix[1][2] =  1.0f;
  _mMax2Di.raMatrix[2][2] =  0.0f;
  _mMax2Di.raMatrix[2][1] = -1.0f;
  DiMatrixInvert( &_mMax2Di, &_mDi2Max);

  DiWorldForAllFrames(wpWorld, _diGeoFrameCallback, NULL);
  DiWorldForAllGeometryFrames(wpWorld, _diGeoFrameCoordCallback, NULL);

  DIRETURN(wpWorld);
} // end of DiWorldCreate

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   gpGeometry
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoDestroyGeometry(DiGeometry *gpGeometry)
{
  DIFUNCTION("_diGeoDestroyGeometry");
  if (gpGeometry->tpTriangles)
  {
    DiMemFree(gpGeometry->tpTriangles);
  }
  if (gpGeometry->uvpTexCoords)
  {
    DiMemFree(gpGeometry->uvpTexCoords);
  }
  if (gpGeometry->vpNormals)
  {
    DiMemFree(gpGeometry->vpNormals);
  }
  if (gpGeometry->vpVertices)
  {
    DiMemFree(gpGeometry->vpVertices);
  }

  DIRETURN(TRUE);
} // end of _diGeoDestroyGeometry

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoDestroyCamera(DiCameraValues *cpCamera)
{
  DIFUNCTION("_diGeoDestroyCamera");
  cpCamera = cpCamera;
  DIRETURN(TRUE);
} // end of _diGeoDestroyCamera

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiWorldDestroy(DiWorld  *wpWorld)
{
  DiFrameBase     *fpBase;
  DiFrameBase     *fpDel;
  DiGeometryFrame *fpGeoFrame;
  DiCameraFrame   *fpCamFrame;

  DIFUNCTION("DiWorldDestroy");
  DIASSERT(wpWorld);

  fpBase = wpWorld->fpFrame;
  while (fpBase)
  {
    fpDel   = fpBase;
    fpBase  = fpBase->fpNext;

    switch(fpDel->tType)
    {
      case diFRAME_TYPE_GEOMETRY:
      {
        fpGeoFrame = (DiGeometryFrame*)fpDel;
        _diGeoDestroyGeometry(&fpGeoFrame->gGeometry);
        break;
      } // case
      case diFRAME_TYPE_CAMERA:
      {
        fpCamFrame = (DiCameraFrame*)fpDel;
        _diGeoDestroyCamera(&fpCamFrame->cCamera);
        break;
      } // case
    } // switch
    DiMemFree(fpDel);
  }

  DiMemFree(wpWorld);
  DIRETURN(TRUE);
} // end of DiWorldDestroy

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @param   fpCallback
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiWorldForAllGeometryFrames(
                                    DiWorld   *wpWorld,
                                    DiBool (*fpCallback)(DiGeometryFrame *gpGeometry, DiVoid *pA),
                                    DiVoid    *pA
                                   )
{
  DiFrameBase     *fpBase;
  DiGeometryFrame *fpGeoFrame;
  DiBool          bRet;

  DIFUNCTION("DiWorldForAllGeometryFrames");
  DIASSERT(wpWorld);
  DIASSERT(fpCallback);
  for (fpBase = wpWorld->fpFrame; fpBase; fpBase = fpBase->fpNext)
  {
    if (fpBase->tType != diFRAME_TYPE_GEOMETRY)
      continue;
    fpGeoFrame = (DiGeometryFrame*)fpBase;
    bRet = (*fpCallback)(fpGeoFrame, pA);
    if (!bRet)
    {
      DIRETURN(FALSE);
    }

  } // for (all frames)
  DIRETURN(TRUE);
} // end of DiWorldForAllGeometryFrames(DiGeometryFrame *gpGeometry)
                                 
/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @param   fpCallback
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiWorldForAllFrames(
                            DiWorld  *wpWorld,
                            DiBool (*fpCallback)(DiFrameBase *fpFrame, DiVoid *pA),
                            DiVoid *pA
                            )
{
  DiFrameBase     *fpBase;
  DiBool          bRet;

  DIFUNCTION("DiWorldForAllFrames");
  DIASSERT(wpWorld);
  DIASSERT(fpCallback);
  for (fpBase = wpWorld->fpFrame; fpBase; fpBase = fpBase->fpNext)
  {
    bRet = (*fpCallback)(fpBase, pA);
    if (!bRet)
    {
      DIRETURN(FALSE);
    }

  } // for (all frames)
  DIRETURN(TRUE);
} // end of DiWorldForAllFrames(DiFrameBase *fpFrame)

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   fpFrame
 * @param   pA
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoSearchCameraCallback(
                                            DiFrameBase *fpFrame, 
                                            DiVoid      *pA
                                          )
{
  DiFrameBase **fppFrame;

  DIFUNCTION("_diGeoSearchCameraCallback");
  if (fpFrame->tType == diFRAME_TYPE_CAMERA)
  {
    fppFrame = (DiFrameBase**)pA;
    (*fppFrame) = fpFrame;
    DIRETURN(FALSE);
  }
  DIRETURN(TRUE);
} // end of _diGeoSearchCameraCallback

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @author  Vlad
 * @see     <see>
 */
DiCameraFrame   *DiWorldGetCameraFrame(DiWorld  *wpWorld)
{
  DiCameraFrame *fpFrame;

  DIFUNCTION("DiWorldGetCameraFrame");
  fpFrame = NULL;
  DiWorldForAllFrames(wpWorld, _diGeoSearchCameraCallback, &fpFrame);

  DIRETURN(fpFrame);
} // end of DiWorldGetCameraFrame

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   fpFrame
 * @param   pA
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoGetGeoCenterCallback(
                                           DiGeometryFrame  *fpFrame,
                                           DiVoid           *pA
                                          )
{
  DiV4d   *vpCenter;

  DIFUNCTION("_diGeoGetGeoCenterCallback");
  vpCenter = (DiV4d*)pA;
  DiMatrixGetPos(&fpFrame->fBase.mGTM, vpCenter);
  DIRETURN(FALSE);
} // end of _diGeoGetGeoCenterCallback

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiWorldGetCenter(DiWorld  *wpWorld, DiV4d *vpCenter)
{
  DIFUNCTION("DiWorldGetCenter");
  DiWorldForAllGeometryFrames(
                              wpWorld, 
                              _diGeoGetGeoCenterCallback, 
                              (DiVoid*)vpCenter
                             );
  DIRETURN(TRUE);
} // end of DiWorldGetCenter

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   nX
 * @param   nY
 * @author  Vlad
 * @see     <see>
 */
static  __inline DiBool _diGeoInScreen(DiInt32 nX, DiInt32 nY)
{
  DiInt32   nSign;

  DIFUNCTION("_diGeoInScreen");
  nSign = 0;
  nSign |= nX;
  nSign |= nY;
  nSign |= (diMAIN_WINDOW_WIDTH - 1 - nX);
  nSign |= (diMAIN_WINDOW_HEIGHT- 1 - nY);

  DIRETURN(! (nSign & 0x80000000) );
} // end of _diGeoInScreen

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   gfpFrame
 * @param   pA
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diGeoRenderCallback(
                                     DiGeometryFrame  *gfpFrame,
                                     DiVoid           *pA
                                    )
{
  DiCamera      *cpCamera;
  DiGeometry    *gpGeometry;
  DiMatrix4d    *mpMatrix;
  DiMatrix4d    mLocalToCamera;
  DiMatrix4d    mLocalToScreen;
  DiMatrix4d    mLocalToCanonicView;
  DiInt32       *npClipCodes;
  DiV4d         *vpScreenVertices;
  DiInt32       nV;
  DiInt32       nT;
  DiFloat       rQ;
  DiTriangle    *tpTriangle;
  DiRenderTriangleParams  pTri;
  DiInt32       nIndex;
  DiInt32       nClipOr;
  DiInt32       nClipAnd;

  DIFUNCTION("_diGeoRenderCallback");
  gpGeometry  = &gfpFrame->gGeometry;
  mpMatrix    = &gfpFrame->fBase.mGTM;
  cpCamera    = (DiCamera*)pA;

  // Build transformation matrix from Local to Camera cs.
  // (~Cam) * GTM
  DiMatrixMultiply(&cpCamera->mInvTransform,  mpMatrix , &mLocalToCamera);

  // Build result transformation matrix for projection
  // Proj * (~Cam) * GTM
  DiMatrixMultiply(&cpCamera->mProjection, &mLocalToCamera, &mLocalToScreen);

  // Build view volume clip matrix
  // Clip * (~Cam) * GTM
  DiMatrixMultiply(&cpCamera->mViewClip, &mLocalToCamera, &mLocalToCanonicView);


  npClipCodes = (DiInt32*)DiMemAllocateTmp(
                      sizeof(DiInt32)*gpGeometry->nNumVertices);
  DIASSERT(npClipCodes);

  DiClipCodesBuild(
                    &mLocalToCanonicView,
                    gpGeometry->nNumVertices,
                    gpGeometry->vpVertices,
                    npClipCodes
                  );

  vpScreenVertices = (DiV4d*)DiMemAllocateTmp(
                      sizeof(DiV4d)*gpGeometry->nNumVertices);
  DIASSERT(vpScreenVertices);

  DiMatrixTransformPoints(
                          &mLocalToScreen,
                          gpGeometry->nNumVertices,
                          gpGeometry->vpVertices,
                          vpScreenVertices
                         );

  for (nV = 0; nV < gpGeometry->nNumVertices; nV++)
  {
    // Receive actual screen coordinates from
    // homogenious X,Y,Z,W
    //
    // Xscr = X / W
    // Yscr = Y / W
    // Zscr = Z / W
    //
    // Q = 1/W is well known value for correct
    // texture perspective interpolation
    if (vpScreenVertices[nV].w != 0.0f)
    {
      rQ = 1.0f / vpScreenVertices[nV].w;
      DiV4dScale( &vpScreenVertices[nV], rQ, &vpScreenVertices[nV]);
    }
  } // for all vertices

  tpTriangle = gpGeometry->tpTriangles;
  for (nT = 0; nT < gpGeometry->nNumTriangles; nT++, tpTriangle++)
  {
    nClipOr   = 0;
    nClipAnd  = 0xffffffff;

    nIndex = tpTriangle->naVertexIndices[0];
    pTri.vaVertices[0].nX = (DiInt32)(vpScreenVertices[nIndex].x);
    pTri.vaVertices[0].nY = (DiInt32)(vpScreenVertices[nIndex].y);
    pTri.vaVertices[0].nZ = (DiInt32)(vpScreenVertices[nIndex].z);
    nClipOr   |= npClipCodes[nIndex];
    nClipAnd  &= npClipCodes[nIndex];

    nIndex = tpTriangle->naVertexIndices[1];
    pTri.vaVertices[1].nX = (DiInt32)(vpScreenVertices[nIndex].x);
    pTri.vaVertices[1].nY = (DiInt32)(vpScreenVertices[nIndex].y);
    pTri.vaVertices[1].nZ = (DiInt32)(vpScreenVertices[nIndex].z);
    nClipOr   |= npClipCodes[nIndex];
    nClipAnd  &= npClipCodes[nIndex];

    nIndex = tpTriangle->naVertexIndices[2];
    pTri.vaVertices[2].nX = (DiInt32)(vpScreenVertices[nIndex].x);
    pTri.vaVertices[2].nY = (DiInt32)(vpScreenVertices[nIndex].y);
    pTri.vaVertices[2].nZ = (DiInt32)(vpScreenVertices[nIndex].z);
    nClipOr   |= npClipCodes[nIndex];
    nClipAnd  &= npClipCodes[nIndex];

    // All vertices are completely out of view volume
    if (nClipAnd)
      continue;

    pTri.nColor = _naMatColors[tpTriangle->nMaterialIndex];

    if (
        _diGeoInScreen(pTri.vaVertices[0].nX, pTri.vaVertices[0].nY) &&
        _diGeoInScreen(pTri.vaVertices[1].nX, pTri.vaVertices[1].nY) &&
        _diGeoInScreen(pTri.vaVertices[2].nX, pTri.vaVertices[2].nY) 
       )
    {
      DiRenderTriangle(&pTri);
    }

  }

  DiMemFreeTmp(vpScreenVertices);
  DiMemFreeTmp(npClipCodes);
  DIRETURN(TRUE);
} // end of _diGeoRenderCallback

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   wpWorld
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiWorldRender(DiWorld *wpWorld)
{
  DiCamera    *cpCamera;

  DIFUNCTION("DiWorldRender");
  cpCamera = DiCameraGetCurrent();

  DiWorldForAllGeometryFrames(wpWorld, _diGeoRenderCallback, (DiVoid*)cpCamera);

  DIRETURN(TRUE);
} // end of DiWorldRender

// END
