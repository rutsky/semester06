//  **********************************************************
//  FILE NAME   DiTime.c
//  PURPOSE     System time
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiTime
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <windows.h>

#include "DiErr.h"
#include "DiMem.h"

#include "DiTime.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

static  DiInt32   _nTimeCur   = 0;
static  DiInt32   _nTimePrev  = 0;

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
DiInt32 DiTimeGetCurrentTime(DiVoid)
{
  DIFUNCTION("DiTimeGetCurrentTime");
  DIRETURN(_nTimeCur);
} // end of DiTimeGetCurrentTime

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiInt32 DiTimeGetDelta(DiVoid)
{
  DIFUNCTION("DiTimeGetDelta");
  DIRETURN(_nTimeCur - _nTimePrev);
} // end of DiTimeGetDelta

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool DiTimeUpdate(DiVoid)
{
  DiInt32     nTime;

  DIFUNCTION("DiTimeUpdate");
  nTime = GetTickCount();

  if (_nTimeCur)
  {
    _nTimePrev = _nTimeCur;
  }
  else
  {
    _nTimePrev = nTime;
  }
  _nTimeCur = nTime;
  DIRETURN(TRUE);
} // end of DiTimeUpdate


// END
