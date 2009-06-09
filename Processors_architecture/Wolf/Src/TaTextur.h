//  **********************************************************
//  FILE NAME   TaTextur.h
//  PURPOSE     Texture load module
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

#ifndef _TATEXTUR_H_
#define _TATEXTUR_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "TaTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define	taTEXTURE_SIDE_DEGREE       7
#define taTEXTURE_SIDE              (1L<<taTEXTURE_SIDE_DEGREE)


#define taTEXTURE_MAX_TEXTURES      4

//  **********************************************************
//  Functions
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  TaBool  TaTextureOpen(TaVoid);
extern  TaBool  TaTextureClose(TaVoid);

extern  TaUInt8 *TaTextureGet(TaInt32 nTextureIndex);
extern  TaBool  TaTextureDrawVertLine(
                                    TaUInt8 *cpScreen,
                                    TaInt32 nScreenYUp,
                                    TaInt32 nScreenYDown,
                                    TaInt32 nTextureIndex,
                                    TaInt32 nTextureX,
                                    TaInt32 nScreenX
                                   );


extern  TaBool  TaTextureDrawScreen(
                                    TaUInt8 *cpScreen,
                                    TaInt32 nTextureIndex
                                 );


#ifdef    __cplusplus
}
#endif  /* __cplusplus */


#endif  // _TATEXTUR_H_
