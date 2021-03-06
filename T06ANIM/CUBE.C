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

/* ��������� �������� ������� �������� */
typedef struct tagas4UNIT_CUBE
{
  AS4_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
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

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_CUBE *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */

static VOID CubeUnitInit( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndGObjLoad(&Unit->Cow, "objects\\Car1.object");
} /* End of 'CubeUnitInit' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_CUBE *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */

static VOID CubeUnitResponse( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndWs = Ani->W;
  AS4_RndHs = Ani->H;
  AS4_RndWp = AS4_RndHp * Ani->W / Ani->H;
  AS4_RndMatrProjection = MatrProjection(-AS4_RndWp / 2, AS4_RndWp / 2, -AS4_RndHp / 2, AS4_RndHp / 2, AS4_ProjDist, 1000.0);
} /* End of 'CubeUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_CUBE *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */

static VOID CubeUnitRender( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  VEC p = {1, 0, 0};
  /*POINT pt;*/

  /*AS4_RndMatrView = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS4_RndMatrWorld = MatrRotateY(Ani->Time * 30);*/

  AS4_RndMatrView = MatrViewLookAt(VecTransform(VecSet(0, 0, 15), MatrRotateX(0)), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS4_RndMatrWorld = MatrRotateY(-cl.x / 3.0);
  AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrRotateX(-cl.y / 3.0));
  AS4_RndMatrWorld = MatrMulMatr(AS4_RndMatrWorld, MatrScale(3.5 - AS4_Anim.JPOV / 8.0, 3.5 - AS4_Anim.JPOV / 8.0, 3.5 - AS4_Anim.JPOV / 8.0));

  SelectObject(Ani->hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(Ani->hDC, RGB(0, 0, 0));
  AS4_RndGObjDraw(&Unit->Cow, Ani->hDC);
} /* End of 'CubeUnitRender' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_CUBE *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID CubeUnitClose( as4UNIT_CUBE *Unit, as4ANIM *Ani )
{
  AS4_RndGObjFree(&Unit->Cow);
} /* End of 'CubeUnitClose' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (as4UNIT *) ��������� �� ��������� ������ ��������.
 */

as4UNIT * AS4_CubeUnitCreate( VOID )
{
  as4UNIT_CUBE *Unit;

  if ((Unit = (as4UNIT_CUBE *)AS4_AnimUnitCreate(sizeof(as4UNIT_CUBE))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)CubeUnitInit;
  Unit->Close = (VOID *)CubeUnitClose;
  Unit->Response = (VOID *)CubeUnitResponse;                                                  
  Unit->Render = (VOID *)CubeUnitRender;
  return (as4UNIT *)Unit;
} /* End of 'AS4_CubeUnitCreate' function */

/* END OF 'CUBE.C' FILE */

