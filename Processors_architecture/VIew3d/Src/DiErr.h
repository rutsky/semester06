//  **********************************************************
//  FILE NAME   DiErr.h
//  PURPOSE     Errors handling
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiError
//  NOTES
//  **********************************************************

#ifndef _DIERR_H_
#define _DIERR_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTypes.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define DIFUNCTION(cpName)
#define DIRETURN(bVal)        return(bVal)
#define DIRETURNVOID()        return
#define DIASSERT(bVal)                \
{                                     \
  if (!(bVal))                        \
  {                                   \
    DiAssertFunc(__FILE__, __LINE__); \
  }                                   \
}


//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif    /* __cplusplus */

extern  DiBool  DiAssertFunc(DiChar *cpFile,DiInt32 nLine);

extern  DiVoid  DiErrMessage(DiChar *cpMsg);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // #ifndef _DIERR_H_
