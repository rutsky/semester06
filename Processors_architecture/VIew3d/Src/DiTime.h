//  **********************************************************
//  FILE NAME   DiTime.h
//  PURPOSE     System time
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiTime
//  NOTES
//  **********************************************************

#ifndef _DITIME_H_
#define _DITIME_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTypes.h"

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

extern  DiInt32   DiTimeGetCurrentTime(DiVoid);
extern  DiInt32   DiTimeGetDelta(DiVoid);

extern  DiBool    DiTimeUpdate(DiVoid);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DITIME_H_
