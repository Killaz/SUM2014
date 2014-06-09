/* FILENAME: STARTUP.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation startup module
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* Ссылки вперед */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 * АРГУМЕНТЫ:
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 *   - флаг показа окна (см. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Регистрация - создание собственного класса окна */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Фоновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрируем класс */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,
    1920, 0,                      /* Позиция окна (x, y) - по умолчанию */
    900 + 8, 900 + 34, /* Размеры окна (w, h) - по умолчанию */
    NULL,                         /* Дескриптор родительского окна */
    NULL,                         /* Дескриптор загруженного меню */
    hInstance,                    /* Дескриптор приложения */
    NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** Добавление объектов ***/
  AS4_AnimAddUnit(AS4_ClockUnitCreate());
  AS4_AnimAddUnit(AS4_LogoUnitCreate());
  AS4_AnimAddUnit(AS4_InfoUnitCreate());

  /* Запуск цикла обработки сообщений */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения:
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
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
    SetTimer(hWnd, 30, 1, NULL);
    AS4_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    AS4_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    AS4_AnimRender();
    AS4_AnimCopyFrame();
    return 0;
  case WM_SYSCOMMAND:
    wParam;
    break;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AS4_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    AS4_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'STARTUP.C' FILE */
