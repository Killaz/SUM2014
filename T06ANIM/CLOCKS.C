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

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/* ��������� �������� ������� �������� */
typedef struct tagas4UNIT_CLOCK
{
  AS4_UNIT_BASE_FIELDS; /* ��������� ������� ����� */
  INT r;
  HBITMAP hBm;
} as4UNIT_CLOCK;

extern POINT cl;

void Arrow( HDC hDC, POINT c, double a, INT r, INT w, COLORREF col )
{
  POINT pt[3];
  double sn = sin(a), csn = cos(a);
  pt[0].x = c.x - w * sn; pt[0].y = c.y + w * csn;
  pt[1].x = c.x + w * sn; pt[1].y = c.y - w * csn;
  pt[2].x = csn * r + c.x;
  pt[2].y = sn * r + c.y;
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(hDC, col);
  SetDCPenColor(hDC, col);
  Polygon(hDC, pt, 3);
}

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitInit( as4UNIT_CLOCK *Unit, as4ANIM *Ani )
{
} /* End of 'ClockUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitClose( as4UNIT_CLOCK *Unit, as4ANIM *Ani )
{
} /* End of 'ClockUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID ClockUnitResponse( as4UNIT_CLOCK *Unit, as4ANIM *Ani )
{
} /* End of 'ClockUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as4UNIT_COW *Unit;
 *   - ��������� �� �������� ��������:
 *       as4ANIM *Ani;
 * ������������ ��������: ���.
 */

static VOID ClockUnitRender( as4UNIT_CLOCK *Unit, as4ANIM *Ani )
{
  SYSTEMTIME st;
  BITMAP bm;
  HDC hDCLogo;
  HDC hScrDC;
  POINT c;

  hScrDC = GetDC(NULL);
  
  GetObject(Unit->hBm, sizeof(bm), &bm);
  Unit->r = bm.bmWidth / 2.5;
  c.x = max(Ani->W / 2, bm.bmWidth / 2) + cl.x;
  c.y = max(Ani->H / 2, bm.bmHeight / 2) + cl.y;

  hDCLogo = CreateCompatibleDC(Ani->hDC);
  SelectObject(hDCLogo, Unit->hBm);
  BitBlt(Ani->hDC, max(Ani->W / 2 - bm.bmWidth / 2, 0) + cl.x, max(Ani->H / 2 - bm.bmHeight / 2, 0) + cl.y, Ani->W, Ani->H, hDCLogo, 0, 0, SRCCOPY);
  DeleteDC(hDCLogo);
  DeleteDC(hScrDC);

  GetLocalTime(&st);
  Arrow(Ani->hDC, c, 2 * M_PI * (st.wHour/* % 12*/ + st.wMinute / 60.0 + st.wSecond / 3600.0 + st.wMilliseconds / 3600000.0) / 12 - M_PI / 2, Unit->r, 7, RGB(255, 0, 0));
  Arrow(Ani->hDC, c, 2 * M_PI * (st.wMinute + st.wSecond / 60.0 + st.wMilliseconds / 60000.0) / 60 - M_PI / 2, Unit->r, 4, RGB(0, 255, 0));
  Arrow(Ani->hDC, c, 2 * M_PI * (st.wSecond + st.wMilliseconds / 1000.0) / 60 - M_PI / 2, Unit->r, 2, RGB(0, 0, 0));
} /* End of 'AS4_ClockUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������: ���.
 * ������������ ��������:
 *   (as4UNIT *) ��������� �� ��������� ������ ��������.
 */
as4UNIT * AS4_ClockUnitCreate( VOID )
{
  as4UNIT_CLOCK *Unit;

  if ((Unit = (as4UNIT_CLOCK *)AS4_AnimUnitCreate(sizeof(as4UNIT_CLOCK))) == NULL)
    return NULL;
  /* ��������� ���� ��-��������� */
  Unit->Init = (VOID *)ClockUnitInit;
  Unit->Close = (VOID *)ClockUnitClose;
  Unit->Response = (VOID *)ClockUnitResponse;                                                  
  Unit->Render = (VOID *)ClockUnitRender;
  Unit->r = 290;
  Unit->hBm = LoadImage(NULL, "img/clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  return (as4UNIT *)Unit;
} /* End of 'AS4_ClockUnitCreate' function */

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

