//  **********************************************************
//  FILE NAME   DiMove.c
//  PURPOSE     Move viewer (camera)
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiMove
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <math.h>

#include "DiErr.h"
#include "DiMem.h"
#include "DiGeo.h"
#include "DiScene.h"
#include "DiCamera.h"
#include "DiTime.h"

#include "DiMove.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

//  **********************************************************
//  Functions
//  **********************************************************

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mDir
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMoveCamera(DiMoveDirection mDir)
{
  DiCamera        *cpCamera;
  DiWorld         *wpWorld;
  DiV4d           vCenter;
  DiV4d           vCamPos;
  DiV4d           vDir;
  DiFloat         rTime;
  DiFloat         rAngle;
  DiFloat         rCos;
  DiFloat         rSin;
  DiV4d           v;
  DiFloat         rDist;

  DIFUNCTION("DiMoveCamera");
  wpWorld   = DiSceneGetWorld();
  cpCamera  = DiSceneGetCamera();

  DiWorldGetCenter(wpWorld, &vCenter);
  DiMatrixGetPos(&cpCamera->fpFrame->mGTM, &vCamPos);
  DiV4dSub(&vCamPos, &vCenter, &vDir);

  rTime = (DiFloat)DiTimeGetDelta();

  // Modify vDir
  switch (mDir)
  {
    case diMOVE_LEFT:
    {
      rAngle  = rTime * diMOVE_ROTATE_SPEED;
      rCos    = (DiFloat)cos(-rAngle);
      rSin    = (DiFloat)sin(-rAngle);
      v       = vDir;
      vDir.x  = v.x * rCos - v.z * rSin;
      vDir.z  = v.x * rSin + v.z * rCos;
      break;
    }
    case diMOVE_RIGHT:
    {
      rAngle = rTime * diMOVE_ROTATE_SPEED;
      rCos    = (DiFloat)cos(rAngle);
      rSin    = (DiFloat)sin(rAngle);
      v       = vDir;
      vDir.x  = v.x * rCos - v.z * rSin;
      vDir.z  = v.x * rSin + v.z * rCos;
      break;
    }
    case diMOVE_UP:
    {
      rAngle  = rTime * diMOVE_ROTATE_SPEED;
      rCos    = (DiFloat)cos(rAngle);
      rSin    = (DiFloat)sin(rAngle);
      v       = vDir;
      vDir.y  = v.y * rCos - v.z * rSin;
      vDir.z  = v.y * rSin + v.z * rCos;
      break;
    }
    case diMOVE_DOWN:
    {
      rAngle  = rTime * diMOVE_ROTATE_SPEED;
      rCos    = (DiFloat)cos(-rAngle);
      rSin    = (DiFloat)sin(-rAngle);
      v       = vDir;
      vDir.y  = v.y * rCos - v.z * rSin;
      vDir.z  = v.y * rSin + v.z * rCos;
      break;
    }
    case diMOVE_FORWARD:
    {
      rDist   = rTime * diMOVE_TRANSLATE_SPEED;
      DiV4dNormalize(&vDir, &v);
      DiV4dScale(&v, -rDist, &v);
      DiV4dAdd(&vDir, &v, &vDir);
      break;
    }
    case diMOVE_BACKWARD:
    {
      rDist   = rTime * diMOVE_TRANSLATE_SPEED;
      DiV4dNormalize(&vDir, &v);
      DiV4dScale(&v, rDist, &v);
      DiV4dAdd(&vDir, &v, &vDir);
      break;
    }
  } // switch on moving direction

  DiV4dAdd(&vCenter, &vDir, &vCamPos);
  DiMatrixSetPos(&cpCamera->fpFrame->mGTM, &vCamPos);
  DiSceneMakeCameraLookToWorld();

  DIRETURN(TRUE);
} // end of DiMoveCamera

// END