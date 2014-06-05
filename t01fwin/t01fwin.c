/* AS4 02.06.14 */

#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500

#define crand (rand() % 255)
#define r0 ((int)((float) rand() / RAND_MAX))
#define sqr(a) ((a) * (a))

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


#ifdef RANDOMIZE
#include <time.h>
#endif

#pragma warning(disable: 4244)

typedef char bool;

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT W, H;


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

void DrawEye( HWND hWnd, HBITMAP hBm, POINT e, POINT pt, INT R, INT r, bool k )
{
  POINT tmp;
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;

  /*hDC = GetDC(hWnd);*/
  hDC = BeginPaint(hWnd, &ps);
  hMemDC = CreateCompatibleDC(hDC);

  SelectObject(hMemDC, hBm);

  if (k)
  {
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
  }
  
  SelectObject(hMemDC, GetStockObject(DC_PEN));
  SetDCPenColor(hMemDC, RGB(0, 0, 0));
  Ellipse(hMemDC, e.x - R, e.y - R, e.x + R, e.y + R);

  SelectObject(hMemDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hMemDC, RGB(11, 40, 142));
  if (sqr(pt.x - e.x) + sqr(pt.y - e.y) < sqr(R - r))
    Ellipse(hMemDC, pt.x - r, pt.y - r, pt.x + r, pt.y + r);
  else
  {
    tmp.x = e.x + ((pt.x - e.x) / sqrt(sqr(pt.x - e.x) + sqr(pt.y - e.y))) * (R - r);
    tmp.y = e.y + ((pt.y - e.y) / sqrt(sqr(pt.x - e.x) + sqr(pt.y - e.y))) * (R - r);
    Ellipse(hMemDC, tmp.x - r, tmp.y - r, tmp.x + r, tmp.y + r);
  }

  BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
  DeleteDC(hMemDC);
  EndPaint(hWnd, &ps);

  /*ReleaseDC(hWnd, hDC);/**/
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  INT R, r;
  static HBITMAP hBm;
  HDC hDC;
  POINT pt, le, re;

  le.x = 0.25 * W, le.y = 0.5 * H;
  re.x = 0.75 * W, re.y = 0.5 * H;
  R = min(H / 2, (re.x - le.x) / 2.2);
  r = R / 5;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 4, 10, NULL);
    break;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, 0);
    break;
  case WM_ERASEBKGND:
    return 1;
  case WM_CHAR:
    if ((CHAR) wParam == 27)
      DestroyWindow(hWnd);
    break;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, 0);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    DrawEye(hWnd, hBm, re, pt, R, r, 0);
    DrawEye(hWnd, hBm, le, pt, R, r, 1);

    /*tmp.x = rc.right / 2, tmp.y = rc.bottom / 2;
    DrawEye(hWnd, tmp, pt, R, r);*/

    /*Sleep(31);*/
    break;
  case WM_DESTROY:
    DeleteObject(hBm);
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
