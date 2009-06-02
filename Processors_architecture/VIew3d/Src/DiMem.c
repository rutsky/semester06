//  **********************************************************
//  FILE NAME   DiMem.c
//  PURPOSE     Memory handling
//  START       15 jan 2000
//  AUTHOR      Vlad
//  PREFIX      DiMem
//  NOTES
//  **********************************************************


// Each memory block has a header with following structure
// {
//   DiInt32  nSign
//   DiInt32  nSize
// }
//
// And footer as 
// {
//   DiInt32  nSign
// }

//  **********************************************************
//  Includes
//  **********************************************************

// System specific includes
#include <windows.h>

// App specific includes
#include "DiTypes.h"
#include "DiErr.h"
#include "DiMem.h"

//  **********************************************************
//  Defines
//  **********************************************************

// Tmp memory size
#define diMEM_TEMP_SIZE   (2 * 1024 * 1024)

// Constant used for memory integrity checks
#define diMEM_SIGN        0x12345678

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data for this module
//  **********************************************************

static    DiUInt8       *_npTempBuffer  = NULL;
static    DiUInt8       *_npTempAvail   = NULL;

//  **********************************************************
//  Functions 
//  **********************************************************

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMemorySystemOpen(DiVoid)
{
  DIFUNCTION("DiMemorySystemOpen");
  _npTempBuffer = (DiUInt8*)DiMemAllocate(diMEM_TEMP_SIZE);
  _npTempAvail  = _npTempBuffer;

  DIRETURN(TRUE);
} // end of DiMemorySystemOpen

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMemorySystemClose(DiVoid)
{
  DIFUNCTION("DiMemorySystemClose");
  DiMemFree(_npTempBuffer);
  _npTempBuffer = NULL;
  DIRETURN(TRUE);
} // end of DiMemorySystemClose

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   nRequiredSize
 * @author  Vlad
 * @see     <see>
 */
DiVoid    *DiMemAllocate(DiInt32 nRequiredSize)
{
  DiInt32   *pMem;
  DiInt32   nMemSize;
  DiInt32   *pEnd;

  DIFUNCTION("DiMemAllocate");
  nMemSize  = nRequiredSize + sizeof(DiInt32)*3;
  pMem      = (DiInt32*)malloc( nMemSize );
  pMem[0]   = diMEM_SIGN ;
  pMem[1]   = nRequiredSize;
  pEnd      = (DiInt32*)( (DiInt8*)pMem + sizeof(DiInt32)*2 + nRequiredSize);
  *pEnd     = diMEM_SIGN ;

  DIRETURN( (DiVoid*)(pMem+2) );

}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   pMem
 * @author  Vlad
 * @see     <see>
 */
DiBool    DiMemFree( DiVoid *pMem )
{
  DiInt32   *pStart;
  DiInt32   *pEnd;
  DiInt32   nSize;

  DIFUNCTION("DiMemFree");
  pStart  = (DiInt32*)( (DiInt8*)pMem - sizeof(DiInt32)*2 );
  if (pStart[0] != diMEM_SIGN)
  {
    DiErrMessage("Memory corrupt");
    DIRETURN(FALSE);
  }
  nSize   = pStart[1];
  pEnd    = (DiInt32*)( (DiInt8*)pMem + nSize );
  if (pEnd[0] != diMEM_SIGN)
  {
    DiErrMessage( "Memory corrupt" );
    DIRETURN(FALSE);
  }
  free(pStart);
  DIRETURN(TRUE);
}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   nSize
 * @author  Vlad
 * @see     <see>
 */
DiVoid    *DiMemAllocateTmp(DiInt32 nSize)
{
  DiInt32   nSizeAllocated;
  DiUInt8   *npMem;

  DIFUNCTION("DiMemAllocateTmp");
  nSizeAllocated = _npTempAvail - _npTempBuffer;
  DIASSERT(nSizeAllocated + nSize < diMEM_TEMP_SIZE);
  npMem = _npTempAvail;
  _npTempAvail += nSize;
  DIRETURN( (DiVoid*)npMem );
} // end of DiMemAllocateTmp

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   pMem
 * @author  Vlad
 * @see     <see>
 */
DiBool    DiMemFreeTmp(DiVoid  *pMem)
{
  DiUInt8   *npMem;
  DiInt32   nOffset;

  DIFUNCTION("DiMemFreeTmp");
  npMem = (DiUInt8*)pMem;
  nOffset = (npMem - _npTempBuffer);
  DIASSERT(nOffset >= 0);
  DIASSERT(nOffset < diMEM_TEMP_SIZE);
  _npTempAvail = npMem;

  DIRETURN(TRUE);
} // end of DiMemFreeTmp

// END