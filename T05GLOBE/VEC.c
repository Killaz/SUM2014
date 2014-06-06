#include "vect.h"

MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }
};

MATR MatrIdenity( VOID )
{
  return UnitMatrix;
}

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}

/*MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
MATR MatrRotateX( DBL AngleInDegree );
MATR MatrRotateY( DBL AngleInDegree );
MATR MatrRotateZ( DBL AngleInDegree );
MATR MatrTranspose( MATR M ); */

MATR MatrRotate( DBL AngleInDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR m;

  a = D2R(AngleInDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}

MATR MatrMulMatr( MATR M1, MATR M2 )
{
  int i, j, k;
  MATR m;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
}

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

MATR MatrInverse( MATR M )
{
  int i, j;
  MATR r;
  DBL det = MatrDeterm(M);
  INT perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };
  if (det == 0)
    return UnitMatrix;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        MatrDeterm3x3(
          M.A[perm[i][0]][perm[j][0]],
          M.A[perm[i][0]][perm[j][1]],
          M.A[perm[i][0]][perm[j][2]],
          M.A[perm[i][1]][perm[j][0]],
          M.A[perm[i][1]][perm[j][1]],
          M.A[perm[i][1]][perm[j][2]],
          M.A[perm[i][2]][perm[j][0]],
          M.A[perm[i][2]][perm[j][1]],
          M.A[perm[i][2]][perm[j][2]]) / det;
  return r;
}

/* Нормали:
MATR Q = MatrTranspose(MatrInverse(M));
N1 = VectorTransform(N, Q);
*/


VEC VecSet( DBL x, DBL y, DBL z )
{
  VEC v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0] + M.A[3][0],
                V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1] + M.A[3][1],
                V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2] + M.A[3][2]);
}

VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0],
                V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1],
                V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2]);
}

VEC VecAddVec( VEC V1, VEC V2 )
{
  /*return VecSet(V1.x + V2.x, V1.y + V2.y, V1.z + V2.z);*/
  V1.x += V2.x;
  V1.y += V2.y;
  V1.z += V2.z;
  return V1;
}

VEC VecSubVec( VEC V1, VEC V2 )
{
  V1.x -= V2.x;
  V1.y -= V2.y;
  V1.z -= V2.z;
  return V1;
}

VEC VecMulNum( VEC V, DBL n )
{
  V.x *= n;
  V.y *= n;
  V.z *= n;
  return V;
}

VEC VecDivNum( VEC V, DBL n )
{
  V.x /= n;
  V.y /= n;
  V.z /= n;
  return V;
}

VEC VecNeg( VEC V )
{
  return VecSet(-V.x, -V.y, -V.z);
}

/*DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.x * V2.x + V1.y * V2.y; ;
}

VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                ... );
}*/

DBL VecLen2( VEC V )
{
  return sqr(V.x) + sqr(V.y) + sqr(V.z);
}

DBL VecLen( VEC V )
{
  return sqrt(sqr(V.x) + sqr(V.y) + sqr(V.z));
}

VEC VecNormalize( VEC V )
{
  DBL len = sqr(V.x) + sqr(V.y) + sqr(V.z);
  if (len != 0 && len != 1)
  {
    len = sqrt(len);
    V.x /= len;
    V.y /= len;
    V.z /= len;
  }
  return V;
}
