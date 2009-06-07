//  **********************************************************
//  FILE NAME   DiMem.h
//  PURPOSE     Memory definition
//  START       15 jan 2000
//  AUTHOR      Vlad
//  PREFIX      DiMem
//  NOTES
//  **********************************************************

#ifndef _DIMEM_H_
#define _DIMEM_H_

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
#endif    /* __cplusplus */

extern  DiBool    DiMemorySystemOpen(DiVoid);
extern  DiBool    DiMemorySystemClose(DiVoid);

extern  DiVoid    *DiMemAllocate( DiInt32   nSize);
extern  DiBool    DiMemFree(      DiVoid    *pMem);

extern  DiVoid    *DiMemAllocateTmp(DiInt32 nSize);
extern  DiBool    DiMemFreeTmp(     DiVoid  *pMem);

#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // #ifndef _DIMEM_H_
