//  **********************************************************
//  FILE NAME   DiMove.h
//  PURPOSE     Move viewer (camera)
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiMove
//  NOTES
//  **********************************************************

#ifndef _DIMOVE_H_
#define _DIMOVE_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diMOVE_ROTATE_SPEED       0.0004f
#define diMOVE_TRANSLATE_SPEED    0.1f

//  **********************************************************
//  Types
//  **********************************************************

typedef enum tagDiMoveDirection
{
  diMOVE_LEFT         = 0,
  diMOVE_RIGHT        = 1,
  diMOVE_UP           = 2,
  diMOVE_DOWN         = 3,
  diMOVE_FORWARD      = 4,
  diMOVE_BACKWARD     = 5
} DiMoveDirection;

//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiBool  DiMoveCamera(DiMoveDirection mDir);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DIMOVE_H_
