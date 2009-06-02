//  **********************************************************
//  FILE NAME   DiGeo.h
//  PURPOSE     Geometry structure
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiGeometry
//  NOTES
//  **********************************************************

#ifndef _DIGEO_H_
#define _DIGEO_H_

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiTransf.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define   diGEOMETRY_MAX_FRAME_NAME       32

//  **********************************************************
//  Types
//  **********************************************************


// Camera
typedef struct tagDiCameraValues
{
  DiFloat       rNear;
  DiFloat       rFar;
  DiFloat       rFieldOfView;
} DiCameraValues;

// Geometry

typedef struct tagDiUV
{
  DiFloat   u,v;
} DiUV;

typedef struct tagDiTriangle
{
  DiInt32       naVertexIndices[3];
  DiInt32       naTexIndices[3];
  DiInt32       nMaterialIndex;
} DiTriangle;

typedef struct tagDiGeometry
{
  DiInt32       nNumVertices;
  DiV4d         *vpVertices;
  DiV4d         *vpNormals;

  DiInt32       nNumTexCoords;
  DiUV          *uvpTexCoords;

  DiInt32       nNumTriangles;
  DiTriangle    *tpTriangles;
} DiGeometry;


// Hierarchy
typedef enum tagDiFrameType
{
  diFRAME_TYPE_GEOMETRY = 0,
  diFRAME_TYPE_CAMERA   = 1
} DiFrameType;

typedef struct tagDiFrameBase
{
  DiFrameType           tType;
  DiChar                caName[diGEOMETRY_MAX_FRAME_NAME];

  // Orientation
  DiMatrix4d            mGTM;

  // Hierarchy
  struct tagDiFrameBase *fpSon;
  struct tagDiFrameBase *fpNext;
} DiFrameBase;

typedef struct tagDiGeometryFrame
{
  DiFrameBase         fBase;
  DiGeometry          gGeometry;
} DiGeometryFrame;

typedef struct tagDiCameraFrame
{
  DiFrameBase         fBase;
  DiCameraValues      cCamera;
} DiCameraFrame;

typedef struct  tagDiWorld
{
  DiFrameBase         *fpFrame;
} DiWorld;

//  **********************************************************
//  Functions
//  **********************************************************

extern  DiWorld         *DiWorldCreate(DiChar *cpTextBuffer);
extern  DiBool          DiWorldDestroy(DiWorld  *wpWorld);

extern  DiBool          DiWorldForAllGeometryFrames(
                              DiWorld  *wpWorld,
                              DiBool (*fpCallback)(DiGeometryFrame *gpGeometry, DiVoid *pA),
                              DiVoid  *pA
                                                    );
extern  DiBool          DiWorldForAllFrames(
                              DiWorld  *wpWorld,
                              DiBool (*fpCallback)
                              (DiFrameBase *fpFrame, DiVoid *pA),
                              DiVoid *pA
                                                    );

extern  DiCameraFrame   *DiWorldGetCameraFrame(DiWorld  *wpWorld);
extern  DiBool          DiWorldGetCenter(
                                         DiWorld  *wpWorld, 
                                         DiV4d    *vpCenter
                                        );

extern  DiBool          DiWorldRender(DiWorld   *wpWorld);

#endif  // _DIGEO_H_
