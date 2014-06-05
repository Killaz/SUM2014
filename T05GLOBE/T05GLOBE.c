/* AS4 03.06.14 */

#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI2 6.28318530717958647692
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

#define elif else if


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

void Globe( HDC hDC, INT W, INT H, INT R, double acc )
{
  double O, f;
  double csn, sn;
  static double k = 0;
  for (O = 0; O < M_PI2; O += M_PI / 120)
  {
    csn = cos(O);
    sn = sin(O);
    for (f = 0; f < M_PI2; f += M_PI / 120)
    {
      if (sn * sin(f + k)/*!z*/ < 0)
        continue;
      SetPixel(hDC, W / 2 + R * sn * cos(f + k), H / 2 + R * csn, RGB(31, 212, 49));
      /*MoveToEx(hDC, W / 2 + R * sn * cos(f - M_PI / 20), H / 2 + R * csn, NULL);
      LineTo(hDC, W / 2 + R * sn * cos(f), H / 2 + R * csn);*/
    }  
  }
  k += acc;
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC, hMemDC;//, hMemDCLogo;
  PAINTSTRUCT ps;
/*  BITMAP bm;*/
  static INT W = 0, H = 0, r = 290, R = 300, xyacc = 20;
  int i;
  static double k = M_PI / 80 - M_PI / 200;
  static HBITMAP hBm, hBmLogo;
  static POINT c = {0, 0};
  unsigned char keys[256];
  /*SYSTEMTIME st;
  POINT c;*/
//  CHAR chr;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 4, 10, NULL);
    hBmLogo = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    break;
  case WM_KEYDOWN: case WM_KEYUP:
    GetKeyboardState(keys);
    for (i = 0; i < 256; i++)
      keys[i] >>= 7;
    if (keys[27])
      DestroyWindow(hWnd);
    if (keys[VK_OEM_PLUS])
      k += M_PI / 800;
    if (keys[VK_OEM_MINUS])
    {
      if (k > 0)
        k -= M_PI / 800;
      else
        k = 0;
    }
    if (keys[VK_NUMPAD0])
      k = M_PI / 80 - M_PI / 200, c.x = c.y = 0;
    if (keys['F'])
      FlipFullScreen(hWnd, 1);
    if (keys['G'])
      FlipFullScreen(hWnd, 0);
    if (keys['A'] || keys[VK_NUMPAD4] || keys[VK_LEFT])
      c.x += xyacc;
    if (keys['S'] || keys[VK_NUMPAD2] || keys[VK_DOWN])
      c.y -= xyacc;
    if (keys['D'] || keys[VK_NUMPAD6] || keys[VK_RIGHT])
      c.x -= xyacc;
    if (keys['W'] || keys[VK_NUMPAD8] || keys[VK_UP])
      c.y += xyacc;
    if (keys['Q'] || keys[VK_NUMPAD7])
      xyacc--;
    if (keys['E'] || keys[VK_NUMPAD9])
      xyacc++;
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
  case WM_MOUSEWHEEL:
    R += ((short) HIWORD(wParam)) / 2;
    break;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Rectangle(hMemDC, 0, 0, W, H);
    /* PAINTING */

    Globe(hMemDC, W + c.x, H + c.y, R, k);
    Globe(hMemDC, W - c.x, H + c.y, R, k);
    Globe(hMemDC, W + c.x, H - c.y, R, k);
    Globe(hMemDC, W - c.x, H - c.y, R, k);/**/

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
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
