/* AS4 03.06.14 */

#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"
#define _WIN32_WINNT 0x0500
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#pragma warning(disable: 4244)

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

  hWnd = CreateWindow(WND_CLASS_NAME, "Clocks", WS_OVERLAPPEDWINDOW, 1920,0,  648, 674, NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  HDC hDC, hMemDC, hMemDCLogo;
  PAINTSTRUCT ps;
  BITMAP bm;
  static INT W = 0, H = 0, k = 1;
  static HBITMAP hBm, hBmLogo;
  SYSTEMTIME st;
  POINT pS[3], pM[3], pH[3];
  double a;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 4, 10, NULL);
    hBmLogo = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    break;
  case WM_CHAR:
    if ((CHAR)wParam == 27)
      DestroyWindow(hWnd);
    else if ((CHAR)wParam == '+')
      k++;
    else if ((CHAR)wParam == '-')
      k--;
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);

    /*SelectObject(hMemDC, DC_BRUSH);/**/
    /*SelectObject(hMemDC, hBm);/**/
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
    
    GetObject(hBmLogo, sizeof(bm), &bm);/**/

    hMemDCLogo = CreateCompatibleDC(hMemDC);
    SelectObject(hMemDCLogo, hBmLogo);
    BitBlt(hMemDC, 0, 0, W, H, hMemDCLogo, 0, 0, SRCCOPY);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));

    GetLocalTime(&st);
    a = k * 2 * M_PI * (st.wHour + st.wMinute / 60.0 + st.wSecond / 3600.0 + st.wMilliseconds / 3600000.0) / 12 - M_PI / 2;
    pH[0].x = 320 - 6 * sin(a); pH[0].y = 320 + 6 * cos(a);
    pH[1].x = 320 + 6 * sin(a); pH[1].y = 320 - 6 * cos(a);
    pH[2].x = cos(a) * 300 + 320;
    pH[2].y = sin(a) * 300 + 320;
    SetDCBrushColor(hMemDC, RGB(255, 0, 0));
    Polygon(hMemDC, pH, 3);

    a = k * 2 * M_PI * (st.wMinute + st.wSecond / 60.0 + st.wMilliseconds / 60000.0) / 60 - M_PI / 2;
    pM[0].x = 320 - 4 * sin(a); pM[0].y = 320 + 4 * cos(a);
    pM[1].x = 320 + 4 * sin(a); pM[1].y = 320 - 4 * cos(a);
    pM[2].x = cos(a) * 300 + 320;
    pM[2].y = sin(a) * 300 + 320;
    SetDCBrushColor(hMemDC, RGB(0, 255, 0));
    Polygon(hMemDC, pM, 3);

    a = k * 2 * M_PI * (st.wSecond + st.wMilliseconds / 1000.0) / 60 - M_PI / 2;
    pS[0].x = 320 - 2 * sin(a); pS[0].y = 320 + 2 * cos(a);
    pS[1].x = 320 + 2 * sin(a); pS[1].y = 320 - 2 * cos(a);
    pS[2].x = cos(a) * 300 + 320;
    pS[2].y = sin(a) * 300 + 320;
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Polygon(hMemDC, pS, 3);
                           
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCLogo);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, 0);
    break;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteObject(hBmLogo);
    PostQuitMessage(0);
    KillTimer(hWnd, 4);
    return 0;
  }

  return DefWindowProc(hWnd, Msg ,wParam, lParam);
}
