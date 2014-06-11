#include <stdio.h>
#include "RENDER.H"

as4CAMERA AS4_RndCam;

DBL
  AS4_RndWs = 40, AS4_RndHs = 40,
  AS4_RndWp = 4, AS4_RndHp = 3,
  AS4_ProjDist = 1;

MATR
  AS4_RndMatrWorld = AS4_UNITMATRIX,
  AS4_RndMatrView = AS4_UNITMATRIX,
  AS4_RndMatrProjection = AS4_UNITMATRIX;
static MATR
  AS4_RndMatrWorldViewProj;

VOID AS4_RndMatrSetup( VOID )
{
  AS4_RndMatrWorldViewProj =
    MatrMulMatr(MatrMulMatr(AS4_RndMatrWorld, AS4_RndMatrView),
      AS4_RndMatrProjection);
} /* End of 'AS4_RndMatrSetup' function */

POINT AS4_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  Pp = PointTransform(P, AS4_RndMatrWorldViewProj);
  
  /* проецирование */
  Ps.x = ( Pp.x + 0.5) * (AS4_RndWs - 1);
  Ps.y = (-Pp.y + 0.5) * (AS4_RndHs - 1);
  return Ps;
} /* End of 'AS4_RndWorldToScreen' function */


BOOL AS4_RndGObjLoad( as4GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  /* обнулили данные */
  memset(GObj, 0, sizeof(as4GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  /* считаем количество вершин и граней */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* выделяем память под вершины и грани как единый участок памяти
   * (memory bulk) */
  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* считываем данные */
  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return 1;
} /* End of 'AS4_RndGObjLoad' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       as4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_RndGObjFree( as4GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(as4GOBJ));
} /* End of 'AS4_RndGObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       as4GOBJ *GObj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_RndGObjDraw( as4GOBJ *GObj, HDC hDC )
{
  INT i;
  POINT *pts;

  if ((pts = (POINT *) malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  AS4_RndMatrSetup();

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = AS4_RndWorldToScreen(GObj->V[i]);
    /*Ellipse(hDC, pts[i].x - s, pts[i].y - s, pts[i].x + s, pts[i].y + s);/**/
  }
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(24, 55, 24));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 153, 24));
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT n0 = GObj->F[i][0], n1 = GObj->F[i][1], n2 = GObj->F[i][2];
    MoveToEx(hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(hDC, pts[n1].x, pts[n1].y);
    LineTo(hDC, pts[n2].x, pts[n2].y);
    LineTo(hDC, pts[n0].x, pts[n0].y);/**/
    /*Polygon(hDC, pt, 3);/**/
  }
  free(pts);
} /* End of 'AS4_RndGObjDraw' function */

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
  r.A[3][2] = -VecDotVec(Loc, Dir);
  r.A[3][3] = 1;
  return r;
}

