//  **********************************************************
//  FILE NAME   DiCamera.c
//  PURPOSE     Camera rendering
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiCamera
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiErr.h"
#include "DiMem.h"

#include "DiCamera.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  DiCamera    *_cpCamera = NULL;

//  **********************************************************
//  Functions
//  **********************************************************

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   pCamParams
 * @author  Vlad
 * @see     <see>
 */
DiCamera    *DiCameraCreate(DiCameraParams *pCamParams)
{
  DiCamera    *cpCamera;
  DiFloat     rSx, rSy;

  DIFUNCTION("DiCameraCreate");
  cpCamera = (DiCamera*)DiMemAllocate(sizeof(DiCamera));
  cpCamera->pParams = *pCamParams;
  cpCamera->fpFrame = NULL;

  // Build InvTransform
  DiMatrixIdentity(&cpCamera->mInvTransform);

  // Build ViewClip matrix
  // in canonical view volume
  //
  // | 1  0     0                  0             |
  // | 0  Hx/Hy 0                  0             |
  // | 0  0     (F+N)*Hx/(F-N)/N   -2*F*Hx/(F-N) |
  // | 0  0     Hx/N               0             |
  //
  //
  DiMatrixIdentity(&cpCamera->mViewClip);
  cpCamera->mViewClip.raMatrix[0][0] = 1.0f;
  cpCamera->mViewClip.raMatrix[1][1] = pCamParams->rHx / pCamParams->rHy;
  cpCamera->mViewClip.raMatrix[2][2] = 
        (pCamParams->rFarZ + pCamParams->rNearZ) * pCamParams->rHx /
        ( (pCamParams->rFarZ - pCamParams->rNearZ)  * pCamParams->rNearZ);
  cpCamera->mViewClip.raMatrix[2][3] = 
        - 2.0f * pCamParams->rFarZ * pCamParams->rHx / 
        (pCamParams->rFarZ - pCamParams->rNearZ);
  cpCamera->mViewClip.raMatrix[3][3] = 0.0f;
  cpCamera->mViewClip.raMatrix[3][2] = pCamParams->rHx / pCamParams->rNearZ;

  // Build projection matrix
  rSx = pCamParams->rScreenX * 0.5f;
  rSy = pCamParams->rScreenY * 0.5f;

  // | Sx  0          Sx*Hx/N                     0                       |
  // |                                                                    |
  // | 0   -Sy*Hx/Hy  Sy*Hx/N                     0                       |
  // |                                                                    |
  // | 0   0          (zMax*F-zMin*N)*Hx/(F-N)/N  -(zMax-zMin)*F*Hx/(F-N) |
  // |                                                                    |
  // | 0   0          Hx/N                        0                       |
  DiMatrixIdentity(&cpCamera->mProjection);
  cpCamera->mProjection.raMatrix[0][0] = rSx;
  cpCamera->mProjection.raMatrix[0][2] = 
      rSx *  pCamParams->rHx / pCamParams->rNearZ;
  cpCamera->mProjection.raMatrix[1][1] = 
      -rSy * pCamParams->rHx / pCamParams->rHy;
  cpCamera->mProjection.raMatrix[1][2] = 
      rSy * pCamParams->rHx / pCamParams->rNearZ;
  cpCamera->mProjection.raMatrix[2][2] = 
      (pCamParams->rMaxZ * pCamParams->rFarZ - 
       pCamParams->rMinZ * pCamParams->rNearZ) * pCamParams->rHx / 
       ( (pCamParams->rFarZ - pCamParams->rNearZ) * pCamParams->rNearZ);
  cpCamera->mProjection.raMatrix[2][3] = 
      - (pCamParams->rMaxZ - pCamParams->rMinZ) * 
      pCamParams->rFarZ * pCamParams->rHx / 
      (pCamParams->rFarZ - pCamParams->rNearZ);
  cpCamera->mProjection.raMatrix[3][3] = 0.0f;
  cpCamera->mProjection.raMatrix[3][2] = 
      pCamParams->rHx / pCamParams->rNearZ;
  
  DIRETURN(cpCamera);
} // end of DiCameraCreate

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @param   fpFrame
 * @author  Vlad
 * @see     <see>
 */
DiBool      DiCameraConnectToFrame(
                                    DiCamera    *cpCamera, 
                                    DiFrameBase *fpFrame
                                  )
{
  DIFUNCTION("DiCameraConnectToFrame");
  DIASSERT(cpCamera);
  DIASSERT(fpFrame);
  cpCamera->fpFrame = fpFrame;
  DIRETURN(TRUE);
} // end of DiCameraConnectToFrame

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiCameraDestroy(DiCamera *cpCamera)
{
  DIFUNCTION("DiCameraDestroy");
  DIRETURN(TRUE);
} // end of DiCameraDestroy

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiCameraRenderBegin(DiCamera *cpCamera)
{
  DIFUNCTION("DiCameraRenderBegin");

  _cpCamera = cpCamera;

  // Build invert camera matrix
  DiMatrixInvert(&cpCamera->fpFrame->mGTM, &cpCamera->mInvTransform);

  DIRETURN(TRUE);
} // end of DiCameraRenderBegin

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiCameraRenderEnd(DiCamera *cpCamera)
{
  DIFUNCTION("DiCameraRenderEnd");

  _cpCamera = NULL;

  DIRETURN(TRUE);
} // end of DiCameraRenderEnd

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiCamera  *DiCameraGetCurrent(DiVoid)
{
  DIFUNCTION("DiCameraGetCurrent");
  DIRETURN(_cpCamera);
} // end of DiCameraGetCurrent

/**
 * Function creates camera matrix orientation
   based on following correspondance between
   orientation basic vectors:

   DiV4dCrossProduct(&vUp, &vAt, &vRight);
   DiV4dCrossProduct(&vRight, &vUp, &vAt);
   DiV4dCrossProduct(&vAt, &vRight, &vUp);

 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpCamera
 * @param   vpPoint
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiCameraLookToPoint(
                              DiCamera  *cpCamera, 
                              DiV4d     *vpPoint
                           )
{
  DiV4d   vCamPos;
  DiV4d   vRight, vUp, vAt;

  DIFUNCTION("DiCameraLookToPoint");

  DiMatrixGetPos( &cpCamera->fpFrame->mGTM, &vCamPos);
  DiV4dSub(vpPoint, &vCamPos, &vAt);
  DiV4dNormalize(&vAt, &vAt);

  // Preliminary Up direction
  vUp.x = 0.0f;
  vUp.y = 1.0f;
  vUp.z = 0.0f;
  vUp.w = 1.0f;

  DiV4dCrossProduct(&vUp, &vAt, &vRight);
  // Finally Up direction
  DiV4dCrossProduct(&vAt, &vRight, &vUp);

  DiV4dNormalize(&vRight, &vRight);
  DiV4dNormalize(&vUp, &vUp);

  DiMatrixSetRight( &cpCamera->fpFrame->mGTM, &vRight);
  DiMatrixSetUp(    &cpCamera->fpFrame->mGTM, &vUp);
  DiMatrixSetAt(    &cpCamera->fpFrame->mGTM, &vAt);

  DIRETURN(TRUE);
} // end of DiCameraLookToPoint

// END
