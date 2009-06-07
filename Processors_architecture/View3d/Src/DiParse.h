//  **********************************************************
//  FILE NAME   DiParse.h
//  PURPOSE     ASE file parsing
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiParse
//  NOTES
//  **********************************************************

#ifndef _DIPARSE_H_
#define _DIPARSE_H_

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

typedef enum  tagDiKeywordType
{
  diKEYWORD_TYPE_NA                     = -1,

  diKEYWORD_TYPE_HEADER                 = 0,
  diKEYWORD_TYPE_COMMENT                = 1,
  diKEYWORD_TYPE_SCENE                  = 2,
  diKEYWORD_TYPE_SCENE_FILENAME         = 3,
  diKEYWORD_TYPE_MATERIAL_LIST          = 4,
  diKEYWORD_TYPE_MATERIAL_COUNT         = 5,
  diKEYWORD_TYPE_GEOMOBJECT             = 6,
  diKEYWORD_TYPE_NODE_NAME              = 7,  
  diKEYWORD_TYPE_NODE_TM                = 8,
  diKEYWORD_TYPE_TM_ROW0                = 9,
  diKEYWORD_TYPE_TM_ROW1                = 10,
  diKEYWORD_TYPE_TM_ROW2                = 11,
  diKEYWORD_TYPE_TM_ROW3                = 12,
  diKEYWORD_TYPE_MESH                   = 13,
  diKEYWORD_TYPE_MESH_NUMVERTEX         = 14,
  diKEYWORD_TYPE_MESH_NUMFACES          = 15,
  diKEYWORD_TYPE_MESH_VERTEX_LIST       = 16,
  diKEYWORD_TYPE_MESH_VERTEX            = 17,
  diKEYWORD_TYPE_MESH_FACE_LIST         = 18,
  diKEYWORD_TYPE_MESH_FACE              = 19,
  diKEYWORD_TYPE_MESH_NUMTVERTEX        = 20,
  diKEYWORD_TYPE_MESH_TVERTLIST         = 21,
  diKEYWORD_TYPE_MESH_TVERT             = 22,
  diKEYWORD_TYPE_MESH_NUMTVFACES        = 23,
  diKEYWORD_TYPE_MESH_TFACELIST         = 24,
  diKEYWORD_TYPE_MESH_TFACE             = 25,
  diKEYWORD_TYPE_MESH_SMOOTHING         = 26,
  diKEYWORD_TYPE_MESH_MTLID             = 27,

  diKEYWORD_TYPE_CAMERAOBJECT           = 28,
  diKEYWORD_TYPE_CAMERA_SETTINGS        = 29,
  diKEYWORD_TYPE_CAMERA_NEAR            = 30,
  diKEYWORD_TYPE_CAMERA_FAR             = 31,
  diKEYWORD_TYPE_CAMERA_FOV             = 32,
  diKEYWORD_TYPE_CAMERA_TYPE            = 33,
  diKEYWORD_TYPE_CAMERA_HITHER          = 34,
  diKEYWORD_TYPE_CAMERA_YON             = 35,

  diKEYWORD_TYPE_LAST

} DiKeywordType;

//  **********************************************************
//  Functions prototypes
//  **********************************************************

#ifdef    __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern  DiBool  DiParseSystemOpen(DiVoid);
extern  DiBool  DiParseSystemClose(DiVoid);

extern  DiChar  *DiParseWaitForChar(DiChar *cpBuffer, DiChar cMatch);
extern  DiChar  *DiParseWaitForString(DiChar *cpBuffer, DiChar *cpMatch);


extern  DiChar  *DiParseReadKeyword(DiChar *cpBuffer, DiKeywordType *ktpType);

extern  DiChar  *DiParseReadQuoted(
                                    DiChar  *cpBuffer, 
                                    DiChar  *cpString,
                                    DiInt32 nMaxChars
                                  );
extern  DiChar  *DiParseSkipBlock(  DiChar  *cpBuffer);

extern  DiChar  *DiParseReadString(
                                    DiChar  *cpBuffer, 
                                    DiChar  *cpString,
                                    DiInt32 nMaxChars
                                  );
extern  DiChar  *DiParseReadFloat(  DiChar *cpBuffer, DiFloat *rpValue);
extern  DiChar  *DiParseReadInt32(  DiChar *cpBuffer, DiInt32 *npValue);


#ifdef    __cplusplus
}
#endif  /* __cplusplus */

#endif  // _DIPARSE_H_
