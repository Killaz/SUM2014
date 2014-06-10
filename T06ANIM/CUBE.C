 /* FILENAME: CUBE.C
 * PROGRAMMER: AS4
 * PURPOSE: Cube unit.
 * LAST UPDATE: 10.06.2014
 */

#include "anim.h"
#include "render.h"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/* Структура описания объекта анимации */
typedef struct tagas4UNIT_CUBE
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
  as4GOBJ Cow;
} as4UNIT_CUBE;

extern POINT cl;
extern struct {
  BYTE
    Keys[256], KeysClick[256], KeysOld[256];
  DBL
    JX, JY, JZ, JR, JU;
  BYTE
    JButs[32], JButsOld[32], JButsClick[32];
  INT JPOV;
} AS4_Anim;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID CubeUnitInit( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndGObjLoad(&Unit->Cow, "objects\\cow.object");
} /* End of 'CubeUnitInit' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID CubeUnitResponse( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndWs = Ani->W;
  AS4_RndHs = Ani->H;
  AS4_RndWp = AS4_RndHp * Ani->W / Ani->H;
} /* End of 'CubeUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID CubeUnitRender( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  INT i, s = 5;
  VEC p = {1, 0, 0};
  POINT pt;

  /*AS4_RndMatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS4_RndMatrWorld = MatrRotateY(Ani->Time * 30);*/

  AS4_RndMatrView = MatrViewLookAt(VecMulMatr(VecSet(0, 0, AS4_Anim.JX + 15), MatrRotateX(50 * AS4_Anim.JY)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS4_RndMatrWorld = MatrRotateY(Ani->GlobalTime * 20);
  AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrScale(0.30, 0.30, 0.30));

  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(Ani->hDC, RGB(0, 0, 0));
  srand(30);
  for (i = 0; i < 0; i++)
  {
    p.x = 2.0 * rand() / RAND_MAX - 1;
    p.y = 2.0 * rand() / RAND_MAX - 1;
    p.z = 2.0 * rand() / RAND_MAX - 1;
    pt = AS4_RndWorldToScreen(p);
    Ellipse(Ani->hDC, pt.x - s, pt.y - s, pt.x + s, pt.y + s);
  }
   AS4_RndGObjDraw(&Unit->Cow, Ani->hDC);
} /* End of 'CubeUnitRender' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_CUBE *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeUnitClose( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndGObjFree(&Unit->Cow);
} /* End of 'CubeUnitClose' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */

as4UNIT * AS4_CubeUnitCreate( VOID )
{
  as4UNIT_CUBE *Unit;

  if ((Unit = (as4UNIT_CUBE *)AS4_AnimUnitCreate(sizeof(as4UNIT_CUBE))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;                                                  
  Unit->Render = (VOID *)CubeUnitRender;
  return (as4UNIT *)Unit;
} /* End of 'AS4_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */

