/* FILENAME: UNITINFO.C
 * PROGRAMMER: AS
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ANIM.H"
#include "units.h"

/* ��������� �������� ������� �������� */
typedef struct tagas4UNIT_INFO
{
  AS4_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
} as4UNIT_INFO;

/* ������� ������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitInit( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
} /* End of 'InfoUnitInit' function */

/* ������� ��������������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitClose( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* ������� ���������� ����������� ���������� ���������������
 * ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_INFO *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitResponse( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
}


/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4UNIT_INFO *Ani;
 * ������������ ��������: ���.
 */
static VOID InfoUnitRender( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
  static CHAR Buf[300];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'InfoUnitRender' function */

/* ������� �������� ��������������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (as4UNIT_INFO *) ��������� �� ��������� ������ ��������.
 */
as4UNIT * AS4_InfoUnitCreate( VOID )
{
  as4UNIT_INFO *Unit;

  if ((Unit = (as4UNIT_INFO *)AS4_AnimUnitCreate(sizeof(as4UNIT_INFO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)AS4_AnimUnitResponse;
  return (as4UNIT *)Unit;
} /* End of 'AS4_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
