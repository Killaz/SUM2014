/* AS4 02.06.14 */

#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"
#define _WIN32_WINNT 0x0500

#define crand (rand() % 255)
#define r0 ((int)((float) rand() / RAND_MAX))
#define sqr(a) ((a) * (a))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


#ifdef RANDOMIZE
#include <time.h>
#endif

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  
# ifdef RANDOMIZE
  srand(time(0));
# else
  srand(13421);
# endif

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error while registering window class", "Error", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "Circles", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    /*InvalidateRect(hWnd, NULL, 1);
    Sleep(300);/**/
  }
  return msg.wParam;
}

void DrawEye( HWND hWnd, POINT e, POINT pt, INT R, INT r )
{
  POINT tmp;
  HDC hDC;

  hDC = GetDC(hWnd);
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(11, 40, 142));
  if (sqr(pt.x - e.x) + sqr(pt.y - e.y) < sqr(R - r)) /* LEFT EYE*/
    Ellipse(hDC, pt.x - r, pt.y - r, pt.x + r, pt.y + r);
  else
  {
    tmp.x = e.x + ((pt.x - e.x) / sqrt(sqr(pt.x - e.x) + sqr(pt.y - e.y))) * (R - r);
    tmp.y = e.y + ((pt.y - e.y) / sqrt(sqr(pt.x - e.x) + sqr(pt.y - e.y))) * (R - r);
    Ellipse(hDC, tmp.x - r, tmp.y - r, tmp.x + r, tmp.y + r);
  }
  ReleaseDC(hWnd, hDC);
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  char output[150];
  INT R, r;
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  POINT pt, le, re;

  GetClientRect(hWnd, &rc);
  le.x = 0.25 * rc.right, le.y = 0.5 * rc.bottom;
  re.x = 0.75 * rc.right, re.y = 0.5 * rc.bottom;
  //R = min(rc.right, min(rc.bottom, abs(le.x - re.x))) / 3;
  R = min(rc.bottom / 2, (re.x - le.x) / 2.2);
  r = R / 5;

  hDC = BeginPaint(hWnd, &ps);
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, le.x - R, le.y - R, le.x + R, le.y + R);
  Ellipse(hDC, re.x - R, re.y - R, re.x + R, re.y + R);
  EndPaint(hWnd, &ps);

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 4, 10, NULL);
    break;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, 1);
    break;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, 1);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    DrawEye(hWnd, le, pt, R, r);
    DrawEye(hWnd, re, pt, R, r);

    /*tmp.x = rc.right / 2, tmp.y = rc.bottom / 2;
    DrawEye(hWnd, tmp, pt, R, r);*/

    Sleep(31);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    KillTimer(hWnd, 4);
    return 0;
  }
  
  /*hDC = BeginPaint(hWnd, &ps);
  GetClientRect(hWnd, &rc);

  srand(14121);
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(crand, crand, crand));
  SetDCBrushColor(hDC, RGB(crand, crand, crand));

  
  Ellipse(hDC, r0 * rc.right, r0 * rc.bottom, r0 * rc.right, r0 * rc.bottom);

  EndPaint(hWnd, &ps);*/

  return DefWindowProc(hWnd, Msg ,wParam, lParam);
}
