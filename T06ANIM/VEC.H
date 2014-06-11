/* SS3 06.06.14 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include "def.h"

#define DEGREE2RADIANL 0.01745329251994329576L
#define D2R(A) ((A) * (PI / 180.0))

typedef struct tagVEC
{
  DBL x, y, z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

extern long double AS4_MultiplierDegree2Radian;

/* Единичная матрица */
extern MATR AS4_UnitMatrix;

#define AS4_UNITMATRIX \
{                      \
  {                    \
    {1, 0, 0, 0},      \
    {0, 1, 0, 0},      \
    {0, 0, 1, 0},      \
    {0, 0, 0, 1},      \
  }                    \
}

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz );
MATR MatrIdenity( VOID );
MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
MATR MatrRotateX( DBL AngleInDegree );
MATR MatrRotateY( DBL AngleInDegree );
MATR MatrRotateZ(DBL AngleInDegree );
MATR MatrRotate( DBL AngleInDegree, DBL X, DBL Y, DBL Z );
MATR MatrMulMatr( MATR M1, MATR M2 );
MATR MatrTranspose( MATR M );
MATR MatrInverse( MATR M );
MATR MatrProjection( DBL Left, DBL Right,
                              DBL Bottom, DBL Top,
                              DBL Near, DBL Far );

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 );
DBL MatrDeterm( MATR M );

VEC VecSet( DBL x, DBL y, DBL z );
VEC VecAddVec( VEC V1, VEC V2 );
VEC VecSubVec( VEC V1, VEC V2 );
VEC VecMulNum( VEC V, DBL n );
VEC VecDivNum( VEC V, DBL n );
VEC VecNeg( VEC V );
DBL VecDotVec( VEC V1, VEC V2 );
VEC VecCrossVec( VEC V1, VEC V2 );
DBL VecLen2( VEC V );
DBL VecLen( VEC V );
VEC VecNormalize( VEC V );
VEC PointTransform( VEC V, MATR M );
VEC VecTransform( VEC V, MATR M );

#endif
