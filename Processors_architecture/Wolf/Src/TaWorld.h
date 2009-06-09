//  **********************************************************
//  FILE NAME   TaWorld.h
//  PURPOSE     World (rooms) definition
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

#ifndef _TAWORLD_H_
#define _TAWORLD_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "TaTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define	taWINDOW_WIDTH                640
#define	taWINDOW_HEIGHT               480


#define taWORLD_NUM_CELLS             16

// In texels
#define taWORLD_CELL_LENGTH_DEGREE    4
#define taWORLD_CELL_LENGTH           (1L<<taWORLD_CELL_LENGTH_DEGREE)

// Wall height
#define taWALL_HEIGHT                 (taWORLD_CELL_LENGTH * 1.0f)

// Proj koef
#define taVERT_PROJ_KOEF              1.0f


//  **********************************************************
//  Functions
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  TaBool  TaWorldCreate(TaVoid);
extern  TaBool  TaWorldDestroy(TaVoid);
extern  TaBool  TaWorldUpdate(TaUInt8 *cpPixels);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */


#endif  // _TAWORLD_H_
