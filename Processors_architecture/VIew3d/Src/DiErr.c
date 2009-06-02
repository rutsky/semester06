//  **********************************************************
//  FILE NAME   DiErr.c
//  PURPOSE     Viewer project
//  START       15 jan 2000
//  AUTHOR      Vlad
//  PREFIX      DiErr
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

// System specific includes
#include <windows.h>
#include <stdio.h>

// App specific includes
#include "DiTypes.h"
#include "DiErr.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data for this module
//  **********************************************************

static  DiBool    _bEnable = TRUE;

//  **********************************************************
//  Functions 
//  **********************************************************


/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpMsg
 * @author  Vlad
 * @see     <see>
 */
DiVoid    DiErrMessage(DiChar *cpMsg)
{
  OutputDebugString( cpMsg );
  OutputDebugString( "\r\n" );
}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpFile
 * @param   nLine
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiAssertFunc(DiChar *cpFile, DiInt32 nLine)
{
  DiChar    caMsg[128];
  DiInt32   nRet;

  DIFUNCTION("DiAssertFunc");
  if (!_bEnable)
  {
    DIRETURN(TRUE);
  }
  sprintf(caMsg, "Fail in <%s> line %ld\nYes: Stop Messages/Coninue/Break?", cpFile, nLine);
  nRet = MessageBox(NULL, caMsg, "ASSERT", MB_YESNOCANCEL);
  switch (nRet)
  {
    case IDYES:
    {
      _bEnable = FALSE;
      break;
    }
    case IDNO:
    {
      // continue
      break;
    }
    default:
    case IDCANCEL:
    {
      DebugBreak();
      break;
    }
  } // switch

  DIRETURN(TRUE);
} // end of DiAssertFunc


// END