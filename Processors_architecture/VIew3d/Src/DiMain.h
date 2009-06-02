//  **********************************************************
//  FILE NAME   DiMain.h
//  PURPOSE     Main module for View3D
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiMain
//  NOTES
//  **********************************************************

#ifndef _DIMAIN_H_
#define _DIMAIN_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diMAIN_WINDOW_WIDTH     640
#define diMAIN_WINDOW_HEIGHT    480
#define diMAIN_WINDOW_BPP       24

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

extern  DiBool    DiMainGetPath(DiChar *cpBuffer, DiInt32 nMaxChars);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DIMAIN_H_
