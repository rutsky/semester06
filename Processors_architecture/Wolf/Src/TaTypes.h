//  **********************************************************
//  FILE NAME   TaTypes.h
//  PURPOSE     Types definition for ASM tasks
//  START       24 nov 2000
//  AUTHOR      Vlad
//  PREFIX      None
//  NOTES
//  **********************************************************

#ifndef _TATYPES_H_
#define _TATYPES_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include <windows.h>
#include <windowsx.h>
#include <assert.h>

//  **********************************************************
//  Defines
//  **********************************************************

#define taFAIL              0
#define taSUCCESS           1


#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else   /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif


#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE   (!FALSE)

//  **********************************************************
//  Types
//  **********************************************************

typedef char                TaChar;

typedef unsigned char       TaUInt8;
typedef char                TaInt8;
typedef unsigned short int  TaUInt16;
typedef short int           TaInt16;
typedef unsigned long int   TaUInt32;
typedef long int            TaInt32;

typedef float               TaFloat;

typedef unsigned long int   TaBool;
typedef void                TaVoid;

typedef struct tagTaV3d
{
  TaFloat   x,y,z;
} TaV3d;

//  **********************************************************
//  Functions
//  **********************************************************

#define TaMemAllocate(nSize)  GlobalAllocPtr(GPTR,(nSize))
#define TaMemFree(pPtr)       GlobalFreePtr(pPtr)


// c = a + b
#define TaV3dAdd( a, b, c )        \
{                                  \
 (c)->x = (a)->x + (b)->x;         \
 (c)->y = (a)->y + (b)->y;         \
 (c)->z = (a)->z + (b)->z;         \
}

// c = a - b
#define TaV3dSub( a, b, c )        \
{                                  \
 (c)->x = (a)->x - (b)->x;         \
 (c)->y = (a)->y - (b)->y;         \
 (c)->z = (a)->z - (b)->z;         \
}

// r = v * scalar
#define TaV3dScale( v, s, r )      \
{                                  \
  (r)->x = (v)->x * (s);           \
  (r)->y = (v)->y * (s);           \
  (r)->z = (v)->z * (s);           \
}

#define TaV3dDotProduct(a,b)       \
  ( (a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z )

#define taASSERT  assert


#endif  // _TATYPES_H_
