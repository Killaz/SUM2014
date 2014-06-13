 /* FILENAME: MAIN.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation startup module
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* ������ ������ */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 *   - ���� ������ ���� (��. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* ����������� - �������� ������������ ������ ���� */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ������� ���� - ��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* ������������ ����� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error while registering window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd = CreateWindow(WND_CLASS_NAME, "OpenGL",
    WS_OVERLAPPEDWINDOW,
    1920, 0,                      /* ������� ���� (x, y) - �� ��������� */
    900 + 8, 900 + 34,            /* ������� ���� (w, h) - �� ��������� */
    NULL,                         /* ���������� ������������� ���� */
    NULL,                         /* ���������� ������������ ���� */
    hInstance,                    /* ���������� ���������� */
    NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** ���������� �������� ***/
  /*AS4_AnimAddUnit(AS4_ClockUnitCreate());*/
  AS4_AnimAddUnit(AS4_InfoUnitCreate());
  AS4_AnimAddUnit(AS4_GObjUnitCreate());

  /* ������ ����� ��������� ��������� */
  while (1/*GetMessage(&msg, NULL, 0, 0)*/)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else
      SendMessage(hWnd, WM_TIMER, 0, 0);/**/
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  MINMAXINFO *minmax;
  static BOOL pause;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 4, 100, NULL);
    AS4_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    AS4_AnimResize(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_TIMER:
    AS4_AnimRender();
    AS4_AnimCopyFrame();
    return 0;
  /*case WM_SYSCOMMAND:
    wParam;
    break;*/
  case WM_ERASEBKGND:
    AS4_AnimCopyFrame();
    return 1;
  case WM_DESTROY:
    AS4_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 4);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */
