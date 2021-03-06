/* FILENAME: ANIM.C
 * PROGRAMMER: AS4
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"

/* ��������� �������� �������� */
static as4ANIM AS4_Anim;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AS4_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  AS4_Anim.hWnd = hWnd;
  AS4_Anim.hDC = CreateCompatibleDC(hDC);
  AS4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  AS4_Anim.W = 30;
  AS4_Anim.H = 30;
  AS4_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* �������������� ������ */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
} /* End of 'AS4_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS4_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
  {
    AS4_Anim.Units[i]->Close(AS4_Anim.Units[i], &AS4_Anim);
    free(AS4_Anim.Units[i]);
  }
  DeleteObject(AS4_Anim.hBmFrame);
  DeleteDC(AS4_Anim.hDC);
  memset(&AS4_Anim, 0, sizeof(as4ANIM));
} /* End of 'AS4_AnimInit' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AS4_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AS4_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(AS4_Anim.hBmFrame);
  AS4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AS4_Anim.hDC, AS4_Anim.hBmFrame);

  /* ���������� ������� */
  AS4_Anim.W = W;
  AS4_Anim.H = H;

  ReleaseDC(AS4_Anim.hWnd, hDC);
} /* End of 'AS4_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS4_AnimRender( VOID )
{
  static BOOL rend = 0;
  INT i;
  LARGE_INTEGER li;
  if (rend)
    return;
  rend = 1;

  /* ���������� ����� */
  GetKeyboardState(AS4_Anim.Keys);
  for (i = 0; i < 256; i++)
    AS4_Anim.Keys[i] >>= 7;

  /* ���������� ������� */
  AS4_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* ���������� ����� */
  QueryPerformanceCounter(&li);

  /* ���������� ����� */
  AS4_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AS4_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (AS4_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    AS4_Anim.DeltaTime = 0;
  }
  else
    AS4_Anim.DeltaTime = AS4_Anim.GlobalDeltaTime;

  AS4_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AS4_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  /* ������� ���� */
  SelectObject(AS4_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AS4_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AS4_Anim.hDC, RGB(255, 255, 255));
  Rectangle(AS4_Anim.hDC, 0, 0, AS4_Anim.W, AS4_Anim.H);

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
    AS4_Anim.Units[i]->Response(AS4_Anim.Units[i], &AS4_Anim);

  /* ��������� �������� */
  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
  {
    SelectObject(AS4_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AS4_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AS4_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(AS4_Anim.hDC, RGB(55, 155, 255));
    AS4_Anim.Units[i]->Render(AS4_Anim.Units[i], &AS4_Anim);
  }

  FrameCounter++;
  rend = 0;
} /* End of 'AS4_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS4_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(AS4_Anim.hWnd);
  BitBlt(hDC, 0, 0, AS4_Anim.W, AS4_Anim.H, AS4_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AS4_Anim.hWnd, hDC);
} /* End of 'AS4_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       as4UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AS4_AnimAddUnit( as4UNIT *Unit )
{
  if (AS4_Anim.NumOfUnits < AS4_MAX_UNITS)
  {
    AS4_Anim.Units[AS4_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AS4_Anim);
  }
} /* End of 'AS4_AnimAddUnit' function */

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = 0; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(AS4_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(AS4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    /* ��� ������ ��������:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AS4_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AS4_Anim.hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 201, SWP_NOOWNERZORDER);
    IsFullScreen = 1;
  } else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(AS4_Anim.hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    IsFullScreen = 0;
  }
} /* End of 'AS4_AnimFlipFullScreen' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AS4_AnimSetPause( BOOL NewPauseFlag )
{
  AS4_Anim.IsPause = NewPauseFlag;
} /* End of 'AS4_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
