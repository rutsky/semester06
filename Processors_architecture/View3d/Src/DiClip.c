//  **********************************************************
//  FILE NAME   DiClip.c
//  PURPOSE     Clip etc
//  START       15 jan 2001
//  AUTHOR      Vlad
//  PREFIX      DiClip
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include "DiErr.h"
#include "DiMem.h"

#include "DiClip.h"

//  **********************************************************
//  Defines
//  **********************************************************

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data
//  **********************************************************

//  **********************************************************
//  Functions
//  **********************************************************

/*
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   vpTest
 * @param   vpBox
 * @param   npClipCode
 * @author  Vlad
 * @see     <see>
 */
static  DiBool  _diClipBuildCode( 
                                  DiV4d   *vpTest, 
                                  DiV4d   *vpBox, 
                                  DiInt32 *npClipCode
                                )
{
  DIFUNCTION("_diClipBuildCode");
  *npClipCode = 0;

  if (vpTest->x < -vpBox->x)
    *npClipCode |= 1;
  if (vpTest->x > vpBox->x)
    *npClipCode |= 2;

  if (vpTest->y < -vpBox->y)
    *npClipCode |= 4;
  if (vpTest->y > vpBox->y)
    *npClipCode |= 8;

  if (vpTest->z < -vpBox->z)
    *npClipCode |= 16;
  if (vpTest->z > vpBox->z)
    *npClipCode |= 32;

  DIRETURN(TRUE);
} // end of _diClipBuildCode

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpClipTransform
 * @param   nNumVertices
 * @param   vpVertices
 * @param   npClipCodes
 * @author  Vlad
 * @see     <see>
 */
DiBool DiClipCodesBuild(
                        DiMatrix4d  *mpClipTransform,
                        DiInt32     nNumVertices,
                        DiV4d       *vpVertices,
                        DiInt32     *npClipCodes
                       )
{
  DiV4d     *vpVerticesTransformed;
  DiInt32   nV;
  DiInt32   *npClipCode;
  DiV4d     vTest, vBox;

  DIFUNCTION("DiClipCodesBuild");
  DIASSERT(mpClipTransform);
  DIASSERT(vpVertices);
  DIASSERT(npClipCodes);

  vpVerticesTransformed = (DiV4d*)DiMemAllocateTmp(
                sizeof(DiV4d)*nNumVertices);
  DIASSERT(vpVerticesTransformed);
  npClipCode = npClipCodes;


  DiMatrixTransformPoints(
                          mpClipTransform,
                          nNumVertices,
                          vpVertices,
                          vpVerticesTransformed
                         );

  for (nV = 0; nV < nNumVertices; nV++, npClipCode++)
  {
    vTest.x = vpVerticesTransformed[nV].x;
    vTest.y = vpVerticesTransformed[nV].y;
    vTest.z = vpVerticesTransformed[nV].z;
    vTest.w = 0.0f;

    vBox.x  = vpVerticesTransformed[nV].w;
    vBox.y  = vpVerticesTransformed[nV].w;
    vBox.z  = vpVerticesTransformed[nV].w;
    vBox.w  = 0.0f;

    _diClipBuildCode( &vTest, &vBox, npClipCode);

  }

  DiMemFreeTmp(vpVerticesTransformed);
  DIRETURN(TRUE);
} // end of DiClipCodesBuild

// END
