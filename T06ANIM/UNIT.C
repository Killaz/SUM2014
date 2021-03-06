/* FILENAME: UNIT.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS4_AnimUnitInit( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS4_AnimUnitClose( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS4_AnimUnitRender( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (as4UNIT *) ��������� �� ��������� ������ ��������.
 */
as4UNIT * AS4_AnimUnitCreate( INT Size )
{
  as4UNIT *Unit;

  if (Size < sizeof(as4UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* ��������� ���� ��-��������� */
  Unit->Size = Size;
  Unit->Init = AS4_AnimUnitInit;
  Unit->Close = AS4_AnimUnitClose;
  Unit->Response = AS4_AnimUnitResponse;
  Unit->Render = AS4_AnimUnitRender;
  return Unit;
} /* End of 'AS4_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

