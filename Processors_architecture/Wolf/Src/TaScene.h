//  **********************************************************
//  FILE NAME   TaScene.h
//  PURPOSE     Scene definitions
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

#ifndef _TASCENE_H_
#define _TASCENE_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "TaTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Functions
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  TaBool  TaScenePaint(HWND hWnd);
extern  TaBool  TaSceneCreate(HWND hWnd);
extern  TaBool  TaSceneDestroy(TaVoid);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */


#endif  // _TASCENE_H_
