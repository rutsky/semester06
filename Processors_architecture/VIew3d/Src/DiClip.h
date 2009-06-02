//  **********************************************************
//  FILE NAME   DiClip.h
//  PURPOSE     Clip codes etc
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiClip
//  NOTES
//  **********************************************************

#ifndef _DICLIP_H_
#define _DICLIP_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTransf.h"

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

extern DiBool DiClipCodesBuild(
                                DiMatrix4d  *mpClipTransform,
                                DiInt32     nNumVertices,
                                DiV4d       *vpVertices,
                                DiInt32     *npClipCodes
                              );

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DICLIP_H_
