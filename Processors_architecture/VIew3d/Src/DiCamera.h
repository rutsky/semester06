//  **********************************************************
//  FILE NAME   DiCamera.h
//  PURPOSE     Camera management
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiCamera
//  NOTES
//  **********************************************************

#ifndef _DICAMERA_H_
#define _DICAMERA_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiGeo.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

typedef struct tagDiCameraParams
{
  DiFloat         rNearZ;   // Clipping
  DiFloat         rFarZ;    // plane on Z axis

  DiFloat         rHx;      // View volume 
  DiFloat         rHy;      // plane width & height

  DiFloat         rScreenX; // Screen size
  DiFloat         rScreenY; // Screen size

  DiFloat         rMinZ;    // Z values range in 
  DiFloat         rMaxZ;    // ZBuffer

} DiCameraParams;

typedef struct tagDiCamera
{
  DiCameraParams  pParams;
  DiFrameBase     *fpFrame;

  DiMatrix4d      mInvTransform;
  DiMatrix4d      mProjection;
  DiMatrix4d      mViewClip;
} DiCamera;

//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiCamera    *DiCameraCreate(DiCameraParams *pCamParams);
extern  DiBool      DiCameraConnectToFrame(
                                    DiCamera *cpCamera, 
                                    DiFrameBase *fpFrame
                                          );
extern  DiBool      DiCameraDestroy(DiCamera *cpCamera);

extern  DiBool      DiCameraRenderBegin(DiCamera *cpCamera);
extern  DiBool      DiCameraRenderEnd(DiCamera *cpCamera);
extern  DiCamera    *DiCameraGetCurrent(DiVoid);

extern  DiBool      DiCameraLookToPoint(
                                        DiCamera  *cpCamera, 
                                        DiV4d     *vpPoint
                                       );

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DICAMERA_H_
