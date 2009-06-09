//  **********************************************************
//  FILE NAME   TaCamera.h
//  PURPOSE     Camera (player orientation)
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

#ifndef _TACAMERA_H_
#define _TACAMERA_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "TaTypes.h"
#include "TaWorld.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define taCAMERA_WIDTH        0.5f

// Camera (player) movements
#define taCAMERA_MOVE_STEP    (taWORLD_CELL_LENGTH * 0.1f)
#define taCAMERA_ROTATE_ANGLE (3.1515926f*2.0f/180.0f)

//  **********************************************************
//  Functions
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  TaBool  TaCameraOpen(TaFloat rStartX, TaFloat rStartZ);
extern  TaBool  TaCameraClose(TaVoid);

extern  TaBool  TaCameraGetPosition(TaV3d *vpPos);
extern  TaBool  TaCameraGetOrientation(TaV3d *vpAt, TaV3d *vpRight);

extern  TaBool  TaCameraGetRay(
                              TaInt32 nScreenX,
                              TaV3d   *vpNormal
                            );

extern  TaBool  TaCameraUp(TaVoid);
extern  TaBool  TaCameraDown(TaVoid);
extern  TaBool  TaCameraLeft(TaVoid);
extern  TaBool  TaCameraRight(TaVoid);
extern  TaBool  TaCameraRotateLeft(TaVoid);
extern  TaBool  TaCameraRotateRight(TaVoid);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */


#endif  // _TACAMERA_H_
