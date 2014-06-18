/* AS4 03.06.14 */

#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FIRSTSCREENH 1080
#define FIRSTSCREENW 1920
#define SECONDSCREENH 1024
#define SECONDSCREENW 1280

#define PIC_H 640
#define PIC_W 640
#define PIC_CX 320
#define PIC_CY 320

#pragma warning(disable: 4244)

#define sqr(a) ((a) * (a))
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  
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

  hWnd = CreateWindow(WND_CLASS_NAME, "Clocks", WS_OVERLAPPEDWINDOW, 1920, 0, PIC_W + 8, PIC_H + 34, NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

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

VOID FlipFullScreen( HWND hWnd, INT k )
{
  static BOOL FullScreen = 0;
  static RECT SaveRC;

  if (!FullScreen)
  {
    RECT rc;

    GetWindowRect(hWnd, &SaveRC);

    rc.left = rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST, rc.left + k * FIRSTSCREENW, rc.top, rc.right - rc.left - k * (FIRSTSCREENW - SECONDSCREENW) /* + 1280*/, rc.bottom - rc.top, SWP_NOOWNERZORDER);
    FullScreen = 1;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    FullScreen = 0;
  }
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC, hMemDC, hMemDCLogo;
  PAINTSTRUCT ps;
  BITMAP bm;
  static INT W = 0, H = 0, k = 1, r = 290;
  static HBITMAP hBm, hBmLogo;
  SYSTEMTIME st;
  POINT c;
  CHAR chr;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 4, 10, NULL);
    hBmLogo = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    break;
  case WM_CHAR:
    chr = (CHAR) wParam;

    if (chr == 27)
      DestroyWindow(hWnd);
    else if (chr == '+')
      k++;
    else if (chr == '-')
      k--;
    else if (chr == 'f' || chr == 'F')
      FlipFullScreen(hWnd, 1);
    else if (chr == 'g' || chr == 'G')
      FlipFullScreen(hWnd, 0);
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

    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
    
    GetObject(hBmLogo, sizeof(bm), &bm);/**/

    hMemDCLogo = CreateCompatibleDC(hMemDC);
    SelectObject(hMemDCLogo, hBmLogo);
    BitBlt(hMemDC, max(W / 2 - PIC_CX, 0), max(H / 2 - PIC_CY, 0), W, H, hMemDCLogo, 0, 0, SRCCOPY);
    c.x = PIC_CX + max(W / 2 - PIC_CX, 0);
    c.y = PIC_CY + max(H / 2 - PIC_CY, 0);

    GetLocalTime(&st);
    Arrow(hMemDC, c, k * 2 * M_PI * (st.wHour/* % 12*/ + st.wMinute / 60.0 + st.wSecond / 3600.0 + st.wMilliseconds / 3600000.0) / 12 - M_PI / 2, r, 7, RGB(255, 0, 0));
    Arrow(hMemDC, c, k * 2 * M_PI * (st.wMinute + st.wSecond / 60.0 + st.wMilliseconds / 60000.0) / 60 - M_PI / 2, r, 4, RGB(0, 255, 0));
    Arrow(hMemDC, c, k * 2 * M_PI * (st.wSecond + st.wMilliseconds / 1000.0) / 60 - M_PI / 2, r, 2, RGB(0, 0, 0));
                           
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
