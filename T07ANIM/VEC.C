#include <math.h>

#include "VEC.H"

long double AS4_MultiplierDegree2Radian = DEGREE2RADIANL;

MATR AS4_UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
  }
};

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR A =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      { Dx, Dy, Dz, 1},
    }
  };
  
  return A;
}

MATR MatrIdenity( VOID )
{
  return AS4_UnitMatrix;
}

MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m = AS4_UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
}

MATR MatrRotateX( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = AS4_UNITMATRIX;

  __asm {
    fld  AngleInDegree
    fmul AS4_MultiplierDegree2Radian
    fsincos

    fstp cosine
    fstp sine
  }
  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;
}

MATR MatrRotateY( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = AS4_UNITMATRIX;

  __asm {
    fld  AngleInDegree
    fmul AS4_MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine
  }
  m.A[0][0] = cosine;
  m.A[2][2] = cosine;
  m.A[2][0] = sine;
  m.A[0][2] = -sine;
  return m;
}

MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL sine, cosine;
  MATR m = AS4_UNITMATRIX;

  __asm {
    fld  AngleInDegree
    fmul AS4_MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine
  }
  m.A[0][0] = cosine;
  m.A[1][1] = cosine;
  m.A[0][1] = sine;
  m.A[1][0] = -sine;
  return m;
}

MATR MatrRotate( DBL AngleInDegree, DBL X, DBL Y, DBL Z )
{
  DBL sine, cosine, len;
  MATR m;

  __asm {
    fld AngleInDegree
    fmul AS4_MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine + 88
  }

  len = sqr(X) + sqr(Y) + sqr(Z);
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= sine;
  Y *= sine;
  Z *= sine;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  m.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  m.A[2][1] = 2 * cosine * X + 2 * Y * Z;
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
  MATR m;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
}

MATR MatrTranspose( MATR M )
{
  MATR m;

  m.A[0][0] = M.A[0][0];
  m.A[0][1] = M.A[1][0];
  m.A[0][2] = M.A[2][0];
  m.A[0][3] = M.A[3][0];

  m.A[1][0] = M.A[0][1];
  m.A[1][1] = M.A[1][1];
  m.A[1][2] = M.A[2][1];
  m.A[1][3] = M.A[3][1];

  m.A[2][0] = M.A[0][2];
  m.A[2][1] = M.A[1][2];
  m.A[2][2] = M.A[2][2];
  m.A[2][3] = M.A[3][2];

  m.A[3][0] = M.A[0][3];
  m.A[3][1] = M.A[1][3];
  m.A[3][2] = M.A[2][3];
  m.A[3][3] = M.A[3][3];

  return m;
}

MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT i, j;
  INT perm[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return AS4_UnitMatrix;

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

MATR MatrProjection( DBL Left, DBL Right,
                              DBL Bottom, DBL Top,
                              DBL Near, DBL Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
}

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32
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


VEC VecSet( DBL x, DBL y, DBL z )
{
  VEC V;
  V.x = x;
  V.y = y;
  V.z = z;
  return V;
}

VEC VecAddVec( VEC V1, VEC V2 )
{
  V1.x += V2.x,
  V1.y += V2.y,
  V1.z += V2.z;
  return V1;
}

VEC VecSubVec( VEC V1, VEC V2 )
{
  V1.x -= V2.x,
  V1.y -= V2.y,
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
  if (n != 0)
  {
    V.x /= n;
    V.y /= n;
    V.z /= n;
  }

  return V;     
}

VEC VecNeg( VEC V )
{
  V.x = -V.x;
  V.y = -V.y;
  V.z = -V.z;
  return V;
}

DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
}

VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.y * V2.z - V1.z * V2.y,
                -(V1.x * V2.z - V1.z * V2.x),
                V1.x * V2.y - V1.y * V2.x);
}

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

VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.x * M.A[0][3] + V.y * M.A[1][3] + V.z * M.A[2][3] + M.A[3][3];

  return VecSet((V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0] + M.A[3][0]) / w,
                (V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1] + M.A[3][1]) / w,
                (V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2] + M.A[3][2]) / w);
}

VEC VecTransform( VEC V, MATR M )
{
  return VecSet(V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0],
                V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1],
                V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2]);
}

MATR MatrViewLookAt( VEC Loc, VEC At, VEC UpApprox )
{
  VEC Right, Up, Dir;
  MATR r;
  		
  Dir = VecNormalize(VecSubVec(At, Loc));
  Right = VecNormalize(VecCrossVec(Dir, UpApprox));
  Up = VecCrossVec(Right, Dir);

  r.A[0][0] = Right.x; r.A[0][1] = Up.x; r.A[0][2] = -Dir.x; r.A[0][3] = 0;
  r.A[1][0] = Right.y; r.A[1][1] = Up.y; r.A[1][2] = -Dir.y; r.A[1][3] = 0;
  r.A[2][0] = Right.z; r.A[2][1] = Up.z; r.A[2][2] = -Dir.z; r.A[2][3] = 0;
  r.A[3][0] = -VecDotVec(Loc, Right);
  r.A[3][1] = -VecDotVec(Loc, Up);
  r.A[3][2] = VecDotVec(Loc, Dir);
  r.A[3][3] = 1;
  return r;
}

