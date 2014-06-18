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
  as4GEOM Obj[NUMOFMODELS];
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
  //AS4_GeomLoad(&Unit->Obj, "Z:\\SUM2014\\models\\Cow.object");

  AS4_GeomLoad(&Unit->Obj[0], "E:\\SPR04\\Models\\x6\\x6.object");
  AS4_GeomTransform(&Unit->Obj[0], MatrRotateX(-90));
  AS4_GeomTransform(&Unit->Obj[0], MatrRotateY(90));
  Unit->Obj[0].Len.x = 1.2, Unit->Obj[0].Len.y = 0.65, Unit->Obj[0].Len.z = 0.6;

  AS4_GeomLoad(&Unit->Obj[1], "E:\\SPR04\\Models\\Avent\\avent.object");
  AS4_GeomTransform(&Unit->Obj[1], MatrRotateY(180));
  AS4_GeomTransform(&Unit->Obj[1], MatrTranslate(0, -0.5, 0));
  Unit->Obj[1].Len.x = 1.43, Unit->Obj[1].Len.y = 0.6, Unit->Obj[1].Len.z = 0.75;

  /*AS4_GeomLoad(&Unit->Obj[2], "E:\\SPR04\\Models\\mord_fustang\\Shelby7.object");
  AS4_GeomTransform(&Unit->Obj[2], MatrRotateY(90));
  AS4_GeomTransform(&Unit->Obj[2], MatrTranslate(0, -1.3, 0));
  Unit->Obj[2].Len.x = 5, Unit->Obj[2].Len.y = 1.3, Unit->Obj[2].Len.z = 0.9;*/
  AS4_GeomLoad(&Unit->Obj[2], "E:\\SPR04\\Models\\Mrc\\sls_amg.object");
  AS4_GeomTransform(&Unit->Obj[2], MatrRotateZ(90));
  AS4_GeomTransform(&Unit->Obj[2], MatrRotateX(-90));
  AS4_GeomTransform(&Unit->Obj[2], MatrTranslate(0.65, 0, 0));
  Unit->Obj[2].Len.x = 1.35, Unit->Obj[2].Len.y = 0.45, Unit->Obj[2].Len.z = 0.6;
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

UINT AS4_ShaderProg;

static VOID GObjUnitRender0( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
  MATR WVP;
  /*Ani->RndMatrWorld = MatrRotateY(-Ani->cl.x / 20.0);
  Ani->RndMatrWorld = MatrMulMatr(MatrRotateZ(Ani->cl.y / 20.0), Ani->RndMatrWorld);
  Ani->RndMatrWorld = MatrMulMatr(MatrRotateX(Ani->clz / 20.0), Ani->RndMatrWorld);*/
  Ani->RndMatrWorld = MatrScale(0.5, 0.5, 0.5);

  Ani->RndMatrView = MatrViewLookAt(VecSet(Ani->viewfrom.x, Ani->viewfrom.y, Ani->viewfrom.z), VecSet(Ani->viewto.x, Ani->viewto.y, Ani->viewto.z), VecSet(0, 1, 0));
  Ani->RndMatrView = MatrMulMatr(MatrRotateY(0), Ani->RndMatrView);
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

static VOID GObjUnitRender( as4UNIT_GOBJ *Unit, as4ANIM *Ani )
{
  INT i;
  MATR WVP;
  static DBL time;

  /* оси и позиция наблюдателя */
  /*Ani->RndMatrView = MatrViewLookAt(
      PointTransform(VecSet(Ani->viewfrom.x, Ani->viewfrom.y, Ani->viewfrom.z), MatrRotateX(Ani->cl.z)),
      PointTransform(VecSet(Ani->viewto.x, Ani->viewto.y, Ani->viewto.z), MatrRotateY(Ani->cl.x)),
      PointTransform(VecSet(0, 1, 0), MatrRotateZ(Ani->cl.y)));*/

  Ani->RndMatrWorld = MatrScale(0.5, 0.5, 0.5);

  Ani->RndMatrView = MatrViewLookAt(VecSet(Ani->viewfrom.x, Ani->viewfrom.y, Ani->viewfrom.z), VecSet(Ani->viewto.x, Ani->viewto.y, Ani->viewto.z), VecSet(0, 1, 0));
  Ani->RndMatrView = MatrMulMatr(MatrRotateY(-Ani->cl.x), Ani->RndMatrView);
  Ani->RndMatrView = MatrMulMatr(MatrRotateX(Ani->cl.z), Ani->RndMatrView);
  Ani->RndMatrView = MatrMulMatr(MatrRotateZ(-Ani->cl.y), Ani->RndMatrView);
  Ani->RndMatrProjection = MatrProjection(-Ani->RndWp / 2, Ani->RndWp / 2, -Ani->RndHp / 2, Ani->RndHp / 2, Ani->ProjDist, 1000);

  WVP = MatrMulMatr(Ani->RndMatrWorld, MatrMulMatr(Ani->RndMatrView, Ani->RndMatrProjection));
  glLoadMatrixf(WVP.A[0]);

  glLineWidth(1);
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
  glColorMask(1, 1, 1, 0);
  for (i = -3; i < 30; i++)
  {
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3d(-0.1, -0.1, i);
    glVertex3d(-0.1,  0.1, i);
    glVertex3d( 0.1, -0.1, i);
    glVertex3d( 0.1,  0.1, i);
    glEnd();
  }

  /* Рисуем примитивы */
  time += Ani->GlobalDeltaTime;
  if (time > 1)
  {
    time = 0;
    AS4_ShadProgClose(AS4_ShaderProg);
    AS4_ShaderProg = AS4_ShadProgInit("a.vert", "a.frag");
  }


  glLineWidth(1);
  if (Ani->PolMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //Ani->RndMatrWorld = MatrMulMatr(MatrRotateX(-90), MatrTranslate(0, 0, 0.30 * sin(Ani->Time)));
  AS4_GeomDraw(&Unit->Obj[Ani->CarModel]);
}

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
  INT i;
  for (i = 0; i < NUMOFMODELS; i++)
    AS4_RndGeomFree(&Unit->Obj[i]);
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
  Unit->Response = (VOID *)GObjUnitResponse;
  Unit->Render = (VOID *)GObjUnitRender;
  return (as4UNIT *)Unit;
} /* End of 'AS4_GObjUnitCreate' function */

/* END OF 'GOBJ.C' FILE */
