#include <stdio.h>
#include "RENDER.H"
#include "ANIM.H"


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

VOID AS4_RndGeomFree( as4GEOM *Obj )
{
  INT i;
  for (i = 0; i < Obj->NumOfPrims; i++)
    AS4_PrimFree(&Obj->Prims[i]);
  free(Obj->Prims);
  for (i = 0; i < Obj->NumOfMtls; i++)
    glDeleteTextures(1, &Obj->Mtls[i].TexNo);
  free(Obj->Mtls);
  memset(Obj, 0, sizeof(as4GEOM));
} /* End of 'AS4_RndGObjFree' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       as4GOBJ *GObj;
 *   - контекст устройства вывода:
 *       HDC hDC;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_RndGObjDraw( as4GOBJ *GObj )
{
  INT i;

  if (AS4_Anim.PolMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  glColor3d(1, 0.6, 0.09375);
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT n0 = GObj->F[i][0], n1 = GObj->F[i][1], n2 = GObj->F[i][2];
    glVertex3fv(&GObj->V[n0].x);
    glVertex3fv(&GObj->V[n1].x);
    glVertex3fv(&GObj->V[n2].x);
  }
  glEnd();
} /* End of 'AS4_RndGObjDraw' function */

