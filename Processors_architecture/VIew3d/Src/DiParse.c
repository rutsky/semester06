//  **********************************************************
//  FILE NAME   DiParse.c
//  PURPOSE     ASE text file parsing
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiParse
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <string.h>
#include <stdlib.h>

#include "DiMem.h"
#include "DiErr.h"

#include "DiParse.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diPARSE_IS_CHAR_DELIMITER(c) ( ((c)==' ') || ((c)=='\t') || ((c)=='\r') || ((c)=='\n') )

//  **********************************************************
//  Types
//  **********************************************************
typedef struct tagDiParseKeywordSettings
{
  DiKeywordType   ktType;
  DiChar          *cpString;
  DiInt32         nLength;
} DiParseKeywordSettings;

//  **********************************************************
//  Static
//  **********************************************************

static  DiParseKeywordSettings  _kwSettings[diKEYWORD_TYPE_LAST] =
{
  { diKEYWORD_TYPE_HEADER             , "3DSMAX_ASCIIEXPORT"        , 0},
  { diKEYWORD_TYPE_COMMENT            , "COMMENT"                   , 0},
  { diKEYWORD_TYPE_SCENE              , "SCENE"                     , 0},
  { diKEYWORD_TYPE_SCENE_FILENAME     , "SCENE_FILENAME"            , 0},
  { diKEYWORD_TYPE_MATERIAL_LIST      , "MATERIAL_LIST"             , 0},
  { diKEYWORD_TYPE_MATERIAL_COUNT     , "MATERIAL_COUNT"            , 0},
  { diKEYWORD_TYPE_GEOMOBJECT         , "GEOMOBJECT"                , 0},
  { diKEYWORD_TYPE_NODE_NAME          , "NODE_NAME"                 , 0},
  { diKEYWORD_TYPE_NODE_TM            , "NODE_TM"                   , 0},
  { diKEYWORD_TYPE_TM_ROW0            , "TM_ROW0"                   , 0},
  { diKEYWORD_TYPE_TM_ROW1            , "TM_ROW1"                   , 0},
  { diKEYWORD_TYPE_TM_ROW2            , "TM_ROW2"                   , 0},
  { diKEYWORD_TYPE_TM_ROW3            , "TM_ROW3"                   , 0},
  { diKEYWORD_TYPE_MESH               , "MESH"                      , 0},
  { diKEYWORD_TYPE_MESH_NUMVERTEX     , "MESH_NUMVERTEX"            , 0},
  { diKEYWORD_TYPE_MESH_NUMFACES      , "MESH_NUMFACES"             , 0},
  { diKEYWORD_TYPE_MESH_VERTEX_LIST   , "MESH_VERTEX_LIST"          , 0},
  { diKEYWORD_TYPE_MESH_VERTEX        , "MESH_VERTEX"               , 0},
  { diKEYWORD_TYPE_MESH_FACE_LIST     , "MESH_FACE_LIST"            , 0},
  { diKEYWORD_TYPE_MESH_FACE          , "MESH_FACE"                 , 0},
  { diKEYWORD_TYPE_MESH_NUMTVERTEX    , "MESH_NUMTVERTEX"           , 0},
  { diKEYWORD_TYPE_MESH_TVERTLIST     , "MESH_TVERTLIST"            , 0},
  { diKEYWORD_TYPE_MESH_TVERT         , "MESH_TVERT"                , 0},
  { diKEYWORD_TYPE_MESH_NUMTVFACES    , "MESH_NUMTVFACES"           , 0},
  { diKEYWORD_TYPE_MESH_TFACELIST     , "MESH_TFACELIST"            , 0},
  { diKEYWORD_TYPE_MESH_TFACE         , "MESH_TFACE"                , 0},
  { diKEYWORD_TYPE_MESH_SMOOTHING     , "MESH_SMOOTHING"            , 0},
  { diKEYWORD_TYPE_MESH_MTLID         , "MESH_MTLID"                , 0},
  { diKEYWORD_TYPE_CAMERAOBJECT       , "CAMERAOBJECT"              , 0},
  { diKEYWORD_TYPE_CAMERA_SETTINGS    , "CAMERA_SETTINGS"           , 0},
  { diKEYWORD_TYPE_CAMERA_NEAR        , "CAMERA_NEAR"               , 0},
  { diKEYWORD_TYPE_CAMERA_FAR         , "CAMERA_FAR"                , 0},
  { diKEYWORD_TYPE_CAMERA_FOV         , "CAMERA_FOV"                , 0},
  { diKEYWORD_TYPE_CAMERA_TYPE        , "CAMERA_TYPE"               , 0},
  { diKEYWORD_TYPE_CAMERA_HITHER      , "CAMERA_HITHER"             , 0},
  { diKEYWORD_TYPE_CAMERA_YON         , "CAMERA_YON"                , 0}
};

//  **********************************************************
//  Functions
//  **********************************************************

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   p0
 * @param   p1
 * @author  Vlad
 * @see     <see>
 */
static  DiInt32  _diParseSortCallback(
                                        const DiVoid *p0, 
                                        const DiVoid *p1
                                     )
{
  DiParseKeywordSettings  *sp0, *sp1;
  DiInt32                 nRet;

  DIFUNCTION("_diParseSortCallback");
  sp0 = (DiParseKeywordSettings*)p0;
  sp1 = (DiParseKeywordSettings*)p1;
  if (sp0->nLength < sp1->nLength)
    nRet = 1;
  else
  {
    if (sp0->nLength > sp1->nLength)
      nRet = -1;
    else
      nRet = 0;
  }
  DIRETURN(nRet);
}

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiParseSystemOpen(DiVoid)
{
  DiInt32   nI;

  DIFUNCTION("DiParseSystemOpen");
  for (nI = 0; nI < diKEYWORD_TYPE_LAST; nI++)
  {
    _kwSettings[nI].nLength = strlen(_kwSettings[nI].cpString);
  }
  qsort( 
        (DiVoid*)_kwSettings,
        diKEYWORD_TYPE_LAST,
        sizeof(DiParseKeywordSettings),
        _diParseSortCallback
      );
  DIRETURN(TRUE);
} // end of DiParseSystemOpen

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiParseSystemClose(DiVoid)
{
  DIFUNCTION("DiParseSystemClose");

  DIRETURN(TRUE);
} // end of DiParseSystemClose


/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cMatch
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseWaitForChar(
                            DiChar        *cpBuffer,
                            DiChar        cMatch
                           )
{
  DiChar    *cpS;

  DIFUNCTION("DiParseWaitForChar");
  cpS = cpBuffer;
  while (*cpS != 0)
  {
    if (*cpS == cMatch)
    {
      DIRETURN(cpS + 1);
    }
    cpS ++;
  }

  // End reached
  DIRETURN(NULL);
} // end of DiParseWaitForChar

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   cpMatch
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseWaitForString(DiChar *cpBuffer, DiChar *cpMatch)
{
  DiChar    *cpS;

  DIFUNCTION("DiParseWaitForString");
  cpS = strstr( cpBuffer, cpMatch);
  if (!cpS)
  {
    DIRETURN(NULL);
  }
  cpS += strlen(cpMatch);

  DIRETURN(cpS);
} // end of DiParseWaitForString

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseSkipBlock(DiChar  *cpBuffer)
{
  DiChar    *cpS;
  DiInt32   nCounter;

  DIFUNCTION("DiParseSkipBlock");
  nCounter = 0;
  cpS = cpBuffer;
  while (*cpS)
  {
    if (*cpS == '{')
      nCounter++;
    if (*cpS == '}')
    {
      nCounter --;
      DIASSERT(nCounter >= 0);
      if (nCounter == 0)
        break;
    }

    cpS++;
  }
  if (!*cpS)
  {
    DIRETURN(NULL);
  }
  DIRETURN(cpS + 1);
} // end of DiParseSkipBlock

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   ktpType
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseReadKeyword(
                              DiChar        *cpBuffer,
                              DiKeywordType *ktpType
                           )
{
  DiChar    *cpS;
  DiInt32   nI;

  DIFUNCTION("DiParseReadKeyword");
  *ktpType = diKEYWORD_TYPE_NA;

  cpS = cpBuffer;
  for (nI = 0; nI < diKEYWORD_TYPE_LAST; nI++)
  {
    if (!strncmp(cpBuffer, _kwSettings[nI].cpString, _kwSettings[nI].nLength))
    {
      *ktpType = _kwSettings[nI].ktType;
      cpS += _kwSettings[nI].nLength;
      DIRETURN(cpS);
    }
  }


  DIRETURN(cpS);
} // end of DiParseReadKeyword

/**
 * Skips lead spaces
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   cpString
 * @param   nMaxChars
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseReadString(
                            DiChar  *cpBuffer, 
                            DiChar  *cpString,
                            DiInt32 nMaxChars
                          )
{
  DiChar    *cpS;
  DiChar    *cpDest;

  DIFUNCTION("DiParseReadString");
  cpS = cpBuffer;
  while ( (*cpS) != 0)
  {
    if (diPARSE_IS_CHAR_DELIMITER(*cpS) )
    {
      cpS ++;
      continue;
    }
    break;
  }
  if (! (*cpS) )
    DIRETURN(NULL);

  cpDest = cpString;
  while ( (*cpS) != 0)
  {
    *cpDest = *cpS;
    cpDest ++;
    cpS ++;
    if (cpDest - cpString > nMaxChars-1)
      break;
    // check leksem end
    if (diPARSE_IS_CHAR_DELIMITER(*cpS) )
      break;
  }
  *cpDest = 0;
  if (! (*cpS) )
  {
    DIRETURN(NULL);
  }

  DIRETURN(cpS + 1);
} // end of DiParseReadString

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   rpValue
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseReadFloat(DiChar *cpBuffer, DiFloat *rpValue)
{
  DiChar    *cpS;
  DiChar    caString[32];

  DIFUNCTION("DiParseReadFloat");

  cpS = DiParseReadString(cpBuffer, caString, 32);
  if (!cpS)
  {
    DIRETURN(NULL);
  }

  *rpValue = (DiFloat)atof(caString);

  DIRETURN(cpS);
} // end of DiParseReadFloat

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   npValue
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseReadInt32(  DiChar *cpBuffer, DiInt32 *npValue)
{
  DiChar    *cpS;
  DiChar    caString[32];

  DIFUNCTION("DiParseReadInt32");

  cpS = DiParseReadString(cpBuffer, caString, 32);
  if (!cpS)
  {
    DIRETURN(NULL);
  }

  *npValue = (DiInt32)atol(caString);

  DIRETURN(cpS);
} // end of DiParseReadInt32

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   cpBuffer
 * @param   cpString
 * @param   nMaxChars
 * @author  Vlad
 * @see     <see>
 */
DiChar  *DiParseReadQuoted(
                            DiChar  *cpBuffer, 
                            DiChar  *cpString,
                            DiInt32 nMaxChars
                          )
{
  DiChar    *cpS;
  DiChar    *cpDest;

  DIFUNCTION("DiParseReadQuoted");
  cpS = DiParseWaitForChar(cpBuffer, '\"' );
  if (!cpS)
    DIRETURN(NULL);
  cpDest = cpString;
  while ( (*cpS) != 0)
  {
    *cpDest = *cpS;
    cpDest ++;
    cpS ++;
    if (cpDest - cpString > nMaxChars-1)
      break;

    if ( (*cpS) == '\"')
      break;
  }
  *cpDest = 0;
  if (! (*cpS) )
  {
    DIRETURN(NULL);
  }

  DIRETURN(cpS + 1);
} // end of DiParseReadQuoted



// END
