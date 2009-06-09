//  **********************************************************
//  FILE NAME   TaCamera.c
//  PURPOSE     Camera (player orientation)
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************


//  **********************************************************
//  Includes
//  **********************************************************

#include <math.h>

#include "TaTypes.h"
#include "TaWorld.h"
#include "TaCamera.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  TaV3d     _vCameraPos;
static  TaV3d     _vCameraAt;
static  TaV3d     _vCameraRight;

//  **********************************************************
//  Functions
//  **********************************************************

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraOpen(TaFloat rStartX, TaFloat rStartZ)
{
  // Initialize camera position
  _vCameraPos.x = rStartX;
  _vCameraPos.z = rStartZ;
  _vCameraPos.y = 0.0f;

  // Initialize camera orientation
  _vCameraAt.x = 0.0f;
  _vCameraAt.z = 1.0f;
  _vCameraAt.y = 0.0f;

  _vCameraRight.x = -1.0f;
  _vCameraRight.z = 0.0f;
  _vCameraRight.y = 0.0f;

  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraClose(TaVoid)
{
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraGetPosition(TaV3d *vpPos)
{
  *vpPos = _vCameraPos;
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraGetOrientation(TaV3d *vpAt, TaV3d *vpRight)
{
  *vpAt     = _vCameraAt;
  *vpRight  = _vCameraRight;
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraGetRay(
                      TaInt32 nScreenX,
                      TaV3d   *vpNormal
                    )
{
  TaFloat rScale;

  // rScale will be in [-0.5  0.5]
  rScale = 
            (TaFloat)(nScreenX - taWINDOW_WIDTH/2) / 
            (TaFloat)(taWINDOW_WIDTH);

  vpNormal->x = _vCameraAt.x + _vCameraRight.x * rScale * taCAMERA_WIDTH;
  vpNormal->z = _vCameraAt.z + _vCameraRight.z * rScale * taCAMERA_WIDTH;
  vpNormal->y = 0.0f;

  // Normalization of normal iz not need

  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraUp(TaVoid)
{
  TaV3d   v;

  TaV3dScale(&_vCameraAt, taCAMERA_MOVE_STEP, &v);
  TaV3dAdd(&_vCameraPos, &v, &_vCameraPos);
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraDown(TaVoid)
{
  TaV3d   v;

  TaV3dScale(&_vCameraAt, -taCAMERA_MOVE_STEP, &v);
  TaV3dAdd(&_vCameraPos, &v, &_vCameraPos);
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraLeft(TaVoid)
{
  TaV3d   v;

  TaV3dScale(&_vCameraRight, -taCAMERA_MOVE_STEP, &v);
  TaV3dAdd(&_vCameraPos, &v, &_vCameraPos);
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraRight(TaVoid)
{
  TaV3d   v;

  TaV3dScale(&_vCameraRight, taCAMERA_MOVE_STEP, &v);
  TaV3dAdd(&_vCameraPos, &v, &_vCameraPos);
  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
static  TaVoid  _taRotateVector(
                              TaV3d   *vpVector, 
                              TaFloat rSin, 
                              TaFloat rCos
                             )
{
  TaV3d   v;

  v.x = vpVector->x * rCos + vpVector->z * rSin;
  v.z = vpVector->x * (-rSin) + vpVector->z * rCos;
  vpVector->x = v.x;
  vpVector->z = v.z;
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraRotateLeft(TaVoid)
{
  TaFloat   rSin, rCos;

  rSin = (TaFloat)sin(taCAMERA_ROTATE_ANGLE);
  rCos = (TaFloat)cos(taCAMERA_ROTATE_ANGLE);
  _taRotateVector(&_vCameraAt, rSin, rCos);
  _taRotateVector(&_vCameraRight, rSin, rCos);

  return (taSUCCESS);
}

// NAME        
// PURPOSE     
// ARGUMENTS   None
// RETURNS     None
// NOTES       None
//
TaBool  TaCameraRotateRight(TaVoid)
{
  TaFloat   rSin, rCos;

  rSin = (TaFloat)sin(-taCAMERA_ROTATE_ANGLE);
  rCos = (TaFloat)cos(-taCAMERA_ROTATE_ANGLE);
  _taRotateVector(&_vCameraAt, rSin, rCos);
  _taRotateVector(&_vCameraRight, rSin, rCos);

  return (taSUCCESS);
}

// END
