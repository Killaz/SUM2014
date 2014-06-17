/* SS3 06.06.14 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#pragma warning(disable: 4244 4305)

#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmsystem.h>


#define DEGREE2RADIANL 0.01745329251994329576L
#define D2R(A) ((A) * (PI / 180.0))
#ifndef sqr
#define sqr(a) ((a) * (a))
#endif

/* Common float point types */
typedef float FLT;
typedef double DBL;

typedef struct tagVEC
{
  FLT x, y, z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
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

MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz );
MATR MatrIdenity( VOID );
MATR MatrScale( FLT Sx, FLT Sy, FLT Sz );
MATR MatrRotateX( FLT AngleInDegree );
MATR MatrRotateY( FLT AngleInDegree );
MATR MatrRotateZ(FLT AngleInDegree );
MATR MatrRotate( FLT AngleInDegree, FLT X, FLT Y, FLT Z );
MATR MatrMulMatr( MATR M1, MATR M2 );
MATR MatrTranspose( MATR M );
MATR MatrInverse( MATR M );
MATR MatrProjection( FLT Left, FLT Right,
                              FLT Bottom, FLT Top,
                              FLT Near, FLT Far );

FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 );
FLT MatrDeterm( MATR M );

VEC VecSet( FLT x, FLT y, FLT z );
VEC VecAddVec( VEC V1, VEC V2 );
VEC VecSubVec( VEC V1, VEC V2 );
VEC VecMulNum( VEC V, FLT n );
VEC VecDivNum( VEC V, FLT n );
VEC VecNeg( VEC V );
FLT VecDotVec( VEC V1, VEC V2 );
VEC VecCrossVec( VEC V1, VEC V2 );
FLT VecLen2( VEC V );
FLT VecLen( VEC V );
VEC VecNormalize( VEC V );
VEC PointTransform( VEC V, MATR M );
VEC VecTransform( VEC V, MATR M );
MATR MatrViewLookAt( VEC Loc, VEC At, VEC UpApprox );

#endif
