//  **********************************************************
//  FILE NAME   DiTransf.c
//  PURPOSE     Transformations
//  START       15 jan 2000
//  AUTHOR      Vlad
//  PREFIX      DiMatrix, DiTransf
//  NOTES
//  **********************************************************

//  **********************************************************
//  Includes
//  **********************************************************

#include <memory.h>
#include <math.h>

#include "DiTypes.h"
#include "DiErr.h"

#include "DiTransf.h"

//  **********************************************************
//  Defines
//  **********************************************************

#define diTRANSF_DET(xx,xy,yx,yy) ((xx) * (yy) - (xy) * (yx))

//  **********************************************************
//  Types
//  **********************************************************

//  **********************************************************
//  Static data for this module
//  **********************************************************

//  **********************************************************
//  Functions 
//  **********************************************************


/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   vpIn
 * @param   vpOut
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiV4dNormalize(DiV4d *vpIn, DiV4d *vpOut)
{
  DiFloat   rLen;

  DIFUNCTION("DiV4dNormalize");
  rLen = DiV4dDotProduct(vpIn, vpIn);
  if (rLen != 0.0f)
  {
    rLen = 1.0f / (DiFloat)sqrt(rLen);
    DiV4dScale(vpIn, rLen, vpOut);
  }
  DIRETURN(TRUE);
} // end of DiV4dNormalize

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpMatrix
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMatrixIdentity(DiMatrix4d *mpM)
{
  DIFUNCTION("DiMatrixIdentity");

  mpM->raMatrix[0][0] = 1.0f;
  mpM->raMatrix[0][1] = 0.0f;
  mpM->raMatrix[0][2] = 0.0f;
  mpM->raMatrix[0][3] = 0.0f;

  mpM->raMatrix[1][0] = 0.0f;
  mpM->raMatrix[1][1] = 1.0f;
  mpM->raMatrix[1][2] = 0.0f;
  mpM->raMatrix[1][3] = 0.0f;

  mpM->raMatrix[2][0] = 0.0f;
  mpM->raMatrix[2][1] = 0.0f;
  mpM->raMatrix[2][2] = 1.0f;
  mpM->raMatrix[2][3] = 0.0f;

  mpM->raMatrix[3][0] = 0.0f;
  mpM->raMatrix[3][1] = 0.0f;
  mpM->raMatrix[3][2] = 0.0f;
  mpM->raMatrix[3][3] = 1.0f;

  DIRETURN(TRUE);
} // end of DiMatrixIdentity

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpSrc
 * @param   mpDst
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMatrixCopy(DiMatrix4d *mpSrc, DiMatrix4d *mpDst)
{
  DIFUNCTION("DiMatrixCopy");
  DIASSERT(mpSrc);
  DIASSERT(mpDst);
  memcpy((DiVoid*)mpDst->raMatrix, (DiVoid*)mpSrc->raMatrix, sizeof(DiMatrix4d));
  DIRETURN(TRUE);
} // end of DiMatrixCopy

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpA
 * @param   mpB
 * @param   mpDest
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMatrixMultiply(
                          DiMatrix4d *mpA,
                          DiMatrix4d *mpB,
                          DiMatrix4d *mpDst
                        )
{
  DiInt32   nI, nJ, nK;

  DIFUNCTION("DiMatrixMultiply");

  for (nJ = 0; nJ < 4; nJ++)
  {
    for (nI = 0; nI < 4; nI++)
    {
      mpDst->raMatrix[nJ][nI] = 0.0f;
      for (nK = 0; nK < 4; nK++)
      {
        mpDst->raMatrix[nJ][nI] += 
              mpA->raMatrix[nJ][nK] * 
              mpB->raMatrix[nK][nI];
      }
    }
  }

  DIRETURN(TRUE);
} // end of DiMatrixMultiply

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpIn
 * @param   mpOut
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMatrixInvert(DiMatrix4d *mpIn, DiMatrix4d *mpOut)
{
  DiFloat   rDet;

  DIFUNCTION("DiMatrixInvert");
  DIASSERT(mpOut);

  DiMatrixIdentity(mpOut);

  mpOut->raMatrix[0][0] = diTRANSF_DET(
                                        mpIn->raMatrix[1][1],
                                        mpIn->raMatrix[2][1],
                                        mpIn->raMatrix[1][2],
                                        mpIn->raMatrix[2][2]
                                      );
  mpOut->raMatrix[0][1] = diTRANSF_DET(
                                        mpIn->raMatrix[0][2],
                                        mpIn->raMatrix[2][2],
                                        mpIn->raMatrix[0][1],
                                        mpIn->raMatrix[2][1]
                                      );
  mpOut->raMatrix[0][2] = diTRANSF_DET(
                                        mpIn->raMatrix[0][1],
                                        mpIn->raMatrix[1][1],
                                        mpIn->raMatrix[0][2],
                                        mpIn->raMatrix[1][2]
                                      );

  mpOut->raMatrix[1][0] = diTRANSF_DET(
                                        mpIn->raMatrix[1][2],
                                        mpIn->raMatrix[2][2],
                                        mpIn->raMatrix[1][0],
                                        mpIn->raMatrix[2][0]
                                      );
  mpOut->raMatrix[1][1] = diTRANSF_DET(
                                        mpIn->raMatrix[0][0],
                                        mpIn->raMatrix[2][0],
                                        mpIn->raMatrix[0][2],
                                        mpIn->raMatrix[2][2]
                                      );
  mpOut->raMatrix[1][2] = diTRANSF_DET(
                                        mpIn->raMatrix[0][2],
                                        mpIn->raMatrix[1][2],
                                        mpIn->raMatrix[0][0],
                                        mpIn->raMatrix[1][0]
                                      );


  mpOut->raMatrix[2][0] = diTRANSF_DET(
                                        mpIn->raMatrix[1][0],
                                        mpIn->raMatrix[2][0],
                                        mpIn->raMatrix[1][1],
                                        mpIn->raMatrix[2][1]
                                      );
  mpOut->raMatrix[2][1] = diTRANSF_DET(
                                        mpIn->raMatrix[0][1],
                                        mpIn->raMatrix[2][1],
                                        mpIn->raMatrix[0][0],
                                        mpIn->raMatrix[2][0]
                                      );
  mpOut->raMatrix[2][2] = diTRANSF_DET(
                                        mpIn->raMatrix[0][0],
                                        mpIn->raMatrix[1][0],
                                        mpIn->raMatrix[0][1],
                                        mpIn->raMatrix[1][1]
                                      );

  rDet = 
          mpOut->raMatrix[0][0] * mpIn->raMatrix[0][0] +
          mpOut->raMatrix[0][1] * mpIn->raMatrix[1][0] +
          mpOut->raMatrix[0][2] * mpIn->raMatrix[2][0] ;

  if (rDet == 0.0f)
  {
    DIRETURN(FALSE);
  }
  rDet = 1.0f / rDet;

  mpOut->raMatrix[0][0] *= rDet;
  mpOut->raMatrix[0][1] *= rDet;
  mpOut->raMatrix[0][2] *= rDet;

  mpOut->raMatrix[1][0] *= rDet;
  mpOut->raMatrix[1][1] *= rDet;
  mpOut->raMatrix[1][2] *= rDet;

  mpOut->raMatrix[2][0] *= rDet;
  mpOut->raMatrix[2][1] *= rDet;
  mpOut->raMatrix[2][2] *= rDet;

  mpOut->raMatrix[0][3] = 
    -(mpIn->raMatrix[0][3] * mpOut->raMatrix[0][0] +
      mpIn->raMatrix[1][3] * mpOut->raMatrix[0][1] +
      mpIn->raMatrix[2][3] * mpOut->raMatrix[0][2]
     );
  mpOut->raMatrix[1][3] = 
    -(mpIn->raMatrix[0][3] * mpOut->raMatrix[1][0] +
      mpIn->raMatrix[1][3] * mpOut->raMatrix[1][1] +
      mpIn->raMatrix[2][3] * mpOut->raMatrix[1][2]
     );
  mpOut->raMatrix[2][3] = 
    -(mpIn->raMatrix[0][3] * mpOut->raMatrix[2][0] +
      mpIn->raMatrix[1][3] * mpOut->raMatrix[2][1] +
      mpIn->raMatrix[2][3] * mpOut->raMatrix[2][2]
     );

  DIRETURN(TRUE);
} // end of DiMatrixInvert

/**
 * detailed description
 *
 * @memo    <memo>
 * @return  <return>
 * @param   mpMatrix
 * @param   vpPointsIn
 * @param   vpPointsOut
 * @author  Vlad
 * @see     <see>
 */
DiBool  DiMatrixTransformPoints(
                                  DiMatrix4d  *mpMat,
                                  DiInt32     nNumPoints,
                                  DiV4d       *vpPointsIn,
                                  DiV4d       *vpPointsOut
                               )
{
  DiInt32     nV;
  DiFloat     *vpIn, *vpOut;

  DIFUNCTION("DiMatrixTransformPoints");
  vpIn  = (DiFloat*)vpPointsIn;
  vpOut = (DiFloat*)vpPointsOut;
  for (nV = 0; nV < nNumPoints; nV++)
  {
    /*
    for (nI =0; nI < 4; nI++)
    {
      vaRes[nI] = 0.0f;
      for (nJ = 0; nJ < 4; nJ++)
      {
        vaRes[nI] += mpMat->raMatrix[nI][nJ] * vpSrc[nJ];
      }  
    }
    */
    vpOut[0] =
                mpMat->raMatrix[0][0] * vpIn[0] +
                mpMat->raMatrix[0][1] * vpIn[1] +
                mpMat->raMatrix[0][2] * vpIn[2] +
                mpMat->raMatrix[0][3] ;
    vpOut[1] =
                mpMat->raMatrix[1][0] * vpIn[0] +
                mpMat->raMatrix[1][1] * vpIn[1] +
                mpMat->raMatrix[1][2] * vpIn[2] +
                mpMat->raMatrix[1][3] ;
    vpOut[2] =
                mpMat->raMatrix[2][0] * vpIn[0] +
                mpMat->raMatrix[2][1] * vpIn[1] +
                mpMat->raMatrix[2][2] * vpIn[2] +
                mpMat->raMatrix[2][3] ;
    vpOut[3] =
                mpMat->raMatrix[3][0] * vpIn[0] +
                mpMat->raMatrix[3][1] * vpIn[1] +
                mpMat->raMatrix[3][2] * vpIn[2] +
                mpMat->raMatrix[3][3] ;
    vpIn  += 4;
    vpOut += 4;
  } // for all points
  DIRETURN(TRUE);
} // end of DiMatrixTransformPoints

