//  **********************************************************
//  FILE NAME   DiScene.h
//  PURPOSE     Scene management
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiScene
//  NOTES
//  **********************************************************

#ifndef _DISCENE_H_
#define _DISCENE_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiGeo.h"
#include "DiCamera.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiBool  DiSceneCreate(DiChar *cpFileName);
extern  DiBool  DiSceneDestroy(DiVoid);
extern  DiInt32 DiSceneRender(DiUInt8 *npDestBuffer);

extern  DiWorld   *DiSceneGetWorld(DiVoid);
extern  DiCamera  *DiSceneGetCamera(DiVoid);
extern  DiBool  DiSceneMakeCameraLookToWorld(DiVoid);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DISCENE_H_
