//  **********************************************************
//  FILE NAME   DiTransf.h
//  PURPOSE     Matrix/Vector tyransformations
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiMatrix, DiVector
//  NOTES
//  **********************************************************

#ifndef _DITRANSF_H_
#define _DITRANSF_H_

//  **********************************************************
//  Includes
//  **********************************************************
#include "DiTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define DiV4dDotProduct(a,b)                        \
  ( (a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z)

#define DiV4dAdd(a,b,c)                             \
{                                                   \
 (c)->x = (a)->x + (b)->x;                          \
 (c)->y = (a)->y + (b)->y;                          \
 (c)->z = (a)->z + (b)->z;                          \
 (c)->w = (a)->w + (b)->w;                          \
}

#define DiV4dSub(a,b,c)                             \
{                                                   \
 (c)->x = (a)->x - (b)->x;                          \
 (c)->y = (a)->y - (b)->y;                          \
 (c)->z = (a)->z - (b)->z;                          \
 (c)->w = (a)->w - (b)->w;                          \
}

#define DiV4dScale(v,s,r)                           \
{                                                   \
  (r)->x = (v)->x * (s);                            \
  (r)->y = (v)->y * (s);                            \
  (r)->z = (v)->z * (s);                            \
  (r)->w = (v)->w;                                  \
}

#define DiV4dCrossProduct( a, b, c )                \
{                                                   \
  DIASSERT((a != c) && ( b != c));                  \
  (c)->x = (a)->y * (b)->z - (a)->z * (b)->y;       \
  (c)->y = (a)->z * (b)->x - (a)->x * (b)->z;       \
  (c)->z = (a)->x * (b)->y - (a)->y * (b)->x;       \
  (c)->w = (a)->w;                                  \
}



//  **********************************************************
//  Types
//  **********************************************************

typedef struct tagDiV4d
{
  DiFloat x,y,z,w;
} DiV4d;

typedef struct tagDiMatrix4d
{
  DiFloat   raMatrix[4][4];
} DiMatrix4d;

//  **********************************************************
//  Functions
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiBool  DiV4dNormalize(DiV4d *vpIn, DiV4d *vpOut);

extern  DiBool  DiMatrixIdentity(DiMatrix4d *mpMatrix);
extern  DiBool  DiMatrixCopy(DiMatrix4d *mpSrc, DiMatrix4d *mpDst);

extern  DiBool  DiMatrixMultiply(
                                  DiMatrix4d *mpA,
                                  DiMatrix4d *mpB,
                                  DiMatrix4d *mpDest
                                );
extern  DiBool  DiMatrixInvert(DiMatrix4d *mpIn, DiMatrix4d *mpOut);

extern  DiBool  DiMatrixTransformPoints(
                                  DiMatrix4d  *mpMatrix,
                                  DiInt32     nNumPoints,
                                  DiV4d       *vpPointsIn,
                                  DiV4d       *vpPointsOut
                                      );

#define DiMatrixGetRight(mpMatrix,vpRes)            \
{                                                   \
  (vpRes)->x = (mpMatrix)->raMatrix[0][0];          \
  (vpRes)->y = (mpMatrix)->raMatrix[1][0];          \
  (vpRes)->z = (mpMatrix)->raMatrix[2][0];          \
  (vpRes)->w = (mpMatrix)->raMatrix[3][0];          \
}
#define DiMatrixGetUp(mpMatrix,vpRes)               \
{                                                   \
  (vpRes)->x = (mpMatrix)->raMatrix[0][1];          \
  (vpRes)->y = (mpMatrix)->raMatrix[1][1];          \
  (vpRes)->z = (mpMatrix)->raMatrix[2][1];          \
  (vpRes)->w = (mpMatrix)->raMatrix[3][1];          \
}
#define DiMatrixGetAt(mpMatrix,vpRes)               \
{                                                   \
  (vpRes)->x = (mpMatrix)->raMatrix[0][2];          \
  (vpRes)->y = (mpMatrix)->raMatrix[1][2];          \
  (vpRes)->z = (mpMatrix)->raMatrix[2][2];          \
  (vpRes)->w = (mpMatrix)->raMatrix[3][2];          \
}
#define DiMatrixGetPos(mpMatrix,vpRes)              \
{                                                   \
  (vpRes)->x = (mpMatrix)->raMatrix[0][3];          \
  (vpRes)->y = (mpMatrix)->raMatrix[1][3];          \
  (vpRes)->z = (mpMatrix)->raMatrix[2][3];          \
  (vpRes)->w = (mpMatrix)->raMatrix[3][3];          \
}

#define DiMatrixSetRight(mpMatrix,vpV)              \
{                                                   \
  (mpMatrix)->raMatrix[0][0] = (vpV)->x;            \
  (mpMatrix)->raMatrix[1][0] = (vpV)->y;            \
  (mpMatrix)->raMatrix[2][0] = (vpV)->z;            \
  (mpMatrix)->raMatrix[3][0] = (vpV)->w;            \
}
#define DiMatrixSetUp(mpMatrix,vpV)                 \
{                                                   \
  (mpMatrix)->raMatrix[0][1] = (vpV)->x;            \
  (mpMatrix)->raMatrix[1][1] = (vpV)->y;            \
  (mpMatrix)->raMatrix[2][1] = (vpV)->z;            \
  (mpMatrix)->raMatrix[3][1] = (vpV)->w;            \
}
#define DiMatrixSetAt(mpMatrix,vpV)                 \
{                                                   \
  (mpMatrix)->raMatrix[0][2] = (vpV)->x;            \
  (mpMatrix)->raMatrix[1][2] = (vpV)->y;            \
  (mpMatrix)->raMatrix[2][2] = (vpV)->z;            \
  (mpMatrix)->raMatrix[3][2] = (vpV)->w;            \
}
#define DiMatrixSetPos(mpMatrix,vpV)                \
{                                                   \
  (mpMatrix)->raMatrix[0][3] = (vpV)->x;            \
  (mpMatrix)->raMatrix[1][3] = (vpV)->y;            \
  (mpMatrix)->raMatrix[2][3] = (vpV)->z;            \
  (mpMatrix)->raMatrix[3][3] = (vpV)->w;            \
}


#ifdef    __cplusplus
}
#endif  /* __cplusplus */


#endif  // _DITRANSF_H_
