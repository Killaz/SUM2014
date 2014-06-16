 /* FILENAME: GOBJ.C
 * PROGRAMMER: AS4
 * PURPOSE: GObj Unit
 * LAST UPDATE: 13.06.2014
 */

#include "anim.h"
#include "render.h"

/* Структура описания объекта анимации */
typedef struct tagas4UNIT_GOBJ
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
  as4GOBJ Gobj;
} as4UNIT_GOBJ;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitInit( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
  AS4_RndGObjLoad(&Unit->Gobj, "Z:\\SUM2014\\models\\Cow.object");
} /* End of 'GObjUnitInit' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitResponse( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
} /* End of 'GObjUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID GObjUnitRender( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
  MATR WVP;
  /*Ani->RndMatrWorld = MatrRotateY(-Ani->cl.x / 20.0);
  Ani->RndMatrWorld = MatrMulMatr(MatrRotateZ(Ani->cl.y / 20.0), Ani->RndMatrWorld);
  Ani->RndMatrWorld = MatrMulMatr(MatrRotateX(Ani->clz / 20.0), Ani->RndMatrWorld);*/
  Ani->RndMatrWorld = MatrScale(0.5, 0.5, 0.5);

  Ani->RndMatrView = MatrViewLookAt(VecSet(Ani->viewfrom.x, Ani->viewfrom.y, Ani->viewfrom.z), VecSet(Ani->viewto.x, Ani->viewto.y, Ani->viewto.z), VecSet(0, 1, 0));
  Ani->RndMatrView = MatrMulMatr(MatrRotateY(-Ani->cl.x), Ani->RndMatrView);
  Ani->RndMatrView = MatrMulMatr(MatrRotateX(Ani->cl.z), Ani->RndMatrView);
  Ani->RndMatrView = MatrMulMatr(MatrRotateZ(-Ani->cl.y), Ani->RndMatrView);
  Ani->RndMatrProjection = MatrProjection(-Ani->RndWp / 2, Ani->RndWp / 2, -Ani->RndHp / 2, Ani->RndHp / 2, Ani->ProjDist, 1000);
  WVP = MatrMulMatr(Ani->RndMatrWorld, MatrMulMatr(Ani->RndMatrView, Ani->RndMatrProjection));
  glLoadMatrixf(WVP.A[0]);
  glBegin(GL_LINES);
    glColor3d(1, 0.5, 0.5);
    glVertex3d(-3, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0.5, 1, 0.5);
    glVertex3d(0, -3, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0.5, 0.5, 1);
    glVertex3d(0, 0, -3);
    glVertex4d(0, 0, 1, 0);
  glEnd();
  AS4_RndGObjDraw(&Unit->Gobj);
} /* End of 'GObjUnitRender' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_GOBJ *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID GObjUnitClose( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
  AS4_RndGObjFree(&Unit->Gobj);
} /* End of 'GObjUnitClose' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */

as4UNIT * AS4_GObjUnitCreate( VOID )
{
  as4UNIT_GOBJ *Unit;

  if ((Unit = (as4UNIT_GOBJ *)AS4_AnimUnitCreate(sizeof(as4UNIT_GOBJ))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)GObjUnitInit;
  Unit->Close = (VOID *)GObjUnitClose;
  Unit->Response = (VOID *)AS4_AnimUnitResponse;
  Unit->Render = (VOID *)GObjUnitRender;
  return (as4UNIT *)Unit;
} /* End of 'AS4_GObjUnitCreate' function */

/* END OF 'GOBJ.C' FILE */
