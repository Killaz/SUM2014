/* FILENAME: SAMPLE.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

#define PIC_H 640
#define PIC_W 640
#define PIC_CX 320
#define PIC_CY 320

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/* ��������� �������� ������� �������� */
typedef struct tagas4UNIT_LOGO
{
  AS4_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  HBITMAP hBm_AND, hBm_XOR;
} as4UNIT_LOGO;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID LogoUnitInit( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID LogoUnitClose( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID LogoUnitResponse( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID LogoUnitRender( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
  /*SYSTEMTIME st;*/
  BITMAP bm;
  HDC hScrDC, hMemDC1;

  hScrDC = GetDC(NULL);
  hMemDC1 = CreateCompatibleDC(hScrDC);
  GetObject(Unit->hBm_AND, sizeof(bm), &bm);/**/

  SelectObject(hMemDC1, Unit->hBm_AND);
  BitBlt(Ani->hDC, Ani->W - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCAND);
  SelectObject(hMemDC1, Unit->hBm_XOR);
  BitBlt(Ani->hDC, Ani->W - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCINVERT);

  ReleaseDC(NULL, hScrDC);
  DeleteDC(hMemDC1);
} /* End of 'AS4_LogoUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (as4UNIT *) ��������� �� ��������� ������ ��������.
 */
as4UNIT * AS4_LogoUnitCreate( VOID )
{
  as4UNIT_LOGO *Unit;

  if ((Unit = (as4UNIT_LOGO *)AS4_AnimUnitCreate(sizeof(as4UNIT_LOGO))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)LogoUnitInit;
  Unit->Close = (VOID *)LogoUnitClose;
  Unit->Response = (VOID *)LogoUnitResponse;                                                  
  Unit->Render = (VOID *)LogoUnitRender;
  Unit->hBm_AND = LoadImage(NULL, "YOBA_and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  Unit->hBm_XOR = LoadImage(NULL, "YOBA_xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  return (as4UNIT *)Unit;
} /* End of 'AS4_LogoUnitCreate' function */

/***********************************************************/

/* ������� ���������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */

/* END OF 'SAMPLE.C' FILE */

