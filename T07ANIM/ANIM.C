/* FILENAME: ANIM.C
 * PROGRAMMER: AS4
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "VEC.H"

/* ��������� �������� �������� */
as4ANIM AS4_Anim;

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
BOOL AS4_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  PIXELFORMATDESCRIPTOR pfd = {0};

  AS4_Anim.hDC = GetDC(hWnd);
  AS4_Anim.hWnd = hWnd;
  AS4_Anim.W = 30;
  AS4_Anim.H = 30;
  AS4_Anim.NumOfUnits = 0;
  AS4_Anim.ProjSize = 1;
  AS4_Anim.FarClip = 1000;
  AS4_Anim.viewfrom.x = AS4_Anim.viewfrom.y = AS4_Anim.viewfrom.z = 5;
  AS4_Anim.PolMode = 1;
  AS4_Anim.movecoef = 3;
  AS4_Anim.mute = 1;
  AS4_Anim.Disc.x = AS4_Anim.Disc.y = AS4_Anim.Disc.z = 0;

  /*** ������������� OpenGL ***/

  /* ��������� ������ ����� */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(AS4_Anim.hDC, &pfd);
  DescribePixelFormat(AS4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AS4_Anim.hDC, i, &pfd);

  /* ������� �������� ���������� */
  AS4_Anim.hRC = wglCreateContext(AS4_Anim.hDC);
  /* ������ �������� ��������� */
  wglMakeCurrent(AS4_Anim.hDC, AS4_Anim.hRC);

  /* �������������� ���������� */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AS4_Anim.hRC);
    ReleaseDC(AS4_Anim.hWnd, AS4_Anim.hDC);
    memset(&AS4_Anim, 0, sizeof(as4ANIM));
    return FALSE;
  }

  /* ��������� OpenGL ��-��������� */
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);

  /* �������������� ������ */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;

  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
  AS4_Anim.RndWs = 40, AS4_Anim.RndHs = 40,
  AS4_Anim.RndWp = 4, AS4_Anim.RndHp = 3,
  AS4_Anim.ProjDist = 1;

  AS4_Anim.RndMatrWorld = AS4_Anim.RndMatrView = AS4_Anim.RndMatrProjection = MatrIdenity();
  return 1;
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
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AS4_Anim.hRC);
  ReleaseDC(AS4_Anim.hWnd, AS4_Anim.hDC);
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
  DBL ratio_x = 1, ratio_y = 1;
  AS4_Anim.W = W;
  AS4_Anim.H = H;
  glViewport(0, 0, W, H);

  if (W > H)
    ratio_x = (DBL) W / H;
  else
    ratio_y = (DBL) H / W;
  AS4_Anim.RndWp = AS4_Anim.ProjSize * ratio_x;
  AS4_Anim.RndHp = AS4_Anim.ProjSize * ratio_y;
  AS4_Anim.RndMatrProjection = MatrProjection(-AS4_Anim.RndWp / 2, AS4_Anim.RndWp / 2,
                                              -AS4_Anim.RndHp / 2, AS4_Anim.RndHp / 2,
                                               AS4_Anim.ProjDist, AS4_Anim.FarClip);
} /* End of 'AS4_AnimResize' function */

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

/* ������� ���������� ����� ��������.
 * ���������: ���.
 *������������ ��������: ���.
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

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
    AS4_Anim.Units[i]->Response(AS4_Anim.Units[i], &AS4_Anim);

  /* ������� ���� */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* ��������� �������� */

  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
  {
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
  /* ����� ����� */
  SwapBuffers(AS4_Anim.hDC);
} /* End of 'AS4_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       as4UNIT *Unit;
 * ������������ ��������: ���.
 */

VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani )
{
  INT i;
  GetKeyboardState(Ani->Keys);
  for (i = 0; i < 256; i++)
    Ani->Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    Ani->KeysClick[i] = Ani->Keys[i] && !Ani->KeysOld[i];
  for (i = 0; i < 256; i++)
    Ani->KeysOld[i] = Ani->Keys[i];

  if (Ani->KeysClick['F'])
    AS4_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AS4_AnimSetPause(!Ani->IsPause);
  if (Ani->Keys[VK_ESCAPE])
    DestroyWindow(Ani->hWnd);

  if (Ani->Keys[VK_NUMPAD2])
    Ani->viewto.x += Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.x += Ani->movecoef * Ani->DeltaTime;
  else if (Ani->Keys[VK_NUMPAD8])
    Ani->viewto.x -= Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.x -= Ani->movecoef * Ani->DeltaTime;
  if (Ani->Keys[VK_NUMPAD6])
    Ani->viewto.z -= Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.z -= Ani->movecoef * Ani->DeltaTime;
  else if (Ani->Keys[VK_NUMPAD4])
    Ani->viewto.z += Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.z += Ani->movecoef * Ani->DeltaTime;
  if (Ani->Keys[VK_NUMPAD9])
    Ani->viewto.y += Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.y += Ani->movecoef * Ani->DeltaTime;
  else if (Ani->Keys[VK_NUMPAD3])
    Ani->viewto.y -= Ani->movecoef * Ani->DeltaTime * 0.1, Ani->viewfrom.y -= Ani->movecoef * Ani->DeltaTime;
  if (Ani->KeysClick[VK_NUMPAD7])
    Ani->movecoef += 0.5;
  else if (Ani->KeysClick[VK_NUMPAD1])
    if (Ani->movecoef > 0)
      Ani->movecoef -= 0.5;
    else
      Ani->movecoef = 0;

  if (Ani->JButsClick[JBUTTON8])
  {
    Ani->CarModel = (Ani->CarModel + 1) % NUMOFMODELS;
  } else if (Ani->JButsClick[JBUTTON7])
  {
    Ani->CarModel = (Ani->CarModel - 1 + NUMOFMODELS) % NUMOFMODELS;
  }

  if (Ani->KeysClick[VK_MULTIPLY])
    Ani->viewfrom.x = Ani->viewfrom.y = Ani->viewfrom.z = 5, Ani->viewto.x = Ani->viewto.y = Ani->viewto.z = 0;
  if (Ani->KeysClick['Q'])
    Ani->PolMode = !Ani->PolMode;
  if (Ani->KeysClick[VK_DIVIDE])
    Ani->cl.x = Ani->cl.y = Ani->cl.z = Ani->lg.x = Ani->lg.y = Ani->lg.z = 0;
  if (Ani->Keys[VK_NUMPAD5])
    Ani->mute = 0;
  else
    Ani->mute = 1;

  /* ���� */
  if (Ani->mouse)
  {
    POINT pt;
    Ani->MsWheel = Ani->MsGlobalWheel;
    Ani->MsGlobalWheel = 0;
    /* ���������� ������� */
    pt.x = Ani->MsGlobalX;
    pt.y = Ani->MsGlobalY;
    ScreenToClient(Ani->hWnd, &pt);
    Ani->MsX = pt.x;
    Ani->MsY = pt.y;
    /* ������������� ����������� */
    Ani->MsDeltaX = Ani->MsGlobalX - Ani->MsXOld;
    Ani->MsDeltaY = Ani->MsGlobalY - Ani->MsYOld;
    Ani->MsXOld = Ani->MsGlobalX;
    Ani->MsYOld = Ani->MsGlobalY;
  }

  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(Ani->JButsOld, Ani->JButs, sizeof(Ani->JButs));
        for (i = 0; i < 32; i++)
          Ani->JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          Ani->JButsClick[i] = Ani->JButs[i] && !Ani->JButsOld[i];
        /*if (Ani->JButsClick[JBUTTON5])
          type = ((type - 1) + AS4_LOGO_COUNT) % AS4_LOGO_COUNT;
        if (Ani->JButsClick[JBUTTON6])
          type = (type + 1) % AS4_LOGO_COUNT;
        if (Ani->JButsClick[JBUTTON3])
          if (lgspeed > 0)
            lgspeed -= 5;
        if (Ani->JButsClick[JBUTTON2])
          lgspeed += 5;
        if (Ani->JButsClick[JBUTTON7])
          LogoRight = 0;
        if (Ani->JButsClick[JBUTTON8])
          LogoRight = 1;*/
        if (Ani->JButsClick[JBUTTON4])
          Ani->PolMode = !Ani->PolMode;
        /* ��� */
        Ani->JX = AS4_GET_AXIS_VALUE(X);/*2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;*/
        Ani->JY = AS4_GET_AXIS_VALUE(Y);/*2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASZ)
          Ani->JZ = AS4_GET_AXIS_VALUE(Z);/*2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASR)
          Ani->JR = AS4_GET_AXIS_VALUE(R);/*2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASU)
          Ani->JU = AS4_GET_AXIS_VALUE(U);/*2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;*/

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            Ani->JPOV = 0;
          else
            Ani->JPOV = ji.dwPOV / 4500 + 1;
        }
        if (Ani->JPOV == 3)
        {
          if (Ani->Disc.z < 5)
            Ani->Disc.z += 2 * AS4_Anim.DeltaTime * 0.1;
        } else if (Ani->JPOV == 7)
          if (Ani->Disc.z > -5)
            Ani->Disc.z -= 2 * AS4_Anim.DeltaTime * 0.1;
        if (Ani->JPOV == 1)
        {
          if (Ani->Disc.y < 5)
            Ani->Disc.y += 2 * AS4_Anim.DeltaTime * 0.1;
        } else if (Ani->JPOV == 5)
          if (Ani->Disc.y > -5)
            Ani->Disc.y -= 2 * AS4_Anim.DeltaTime * 0.1;
        if (Ani->JButs[JBUTTON3])
        {
          if (Ani->Disc.x < 5)
            Ani->Disc.x -= 2 * AS4_Anim.DeltaTime * 0.1;
        } else if (Ani->JButs[JBUTTON2])
          if (Ani->Disc.x > -5)
            Ani->Disc.x += 2 * AS4_Anim.DeltaTime * 0.1;
        if (Ani->JButsClick[JBUTTON1])
          Ani->Disc.x = Ani->Disc.y = Ani->Disc.z = 0;
        /*if (Ani->JPOV == 3)
          AS4_Anim.cl.z += 100 * AS4_Anim.DeltaTime;
        else if (Ani->JPOV == 7)
          AS4_Anim.cl.z -= 100 * AS4_Anim.DeltaTime;*/
      }
    }
  }
  AS4_Anim.cl.x += AS4_Anim.JXCoord * 100 * AS4_Anim.DeltaTime;
  AS4_Anim.cl.y += AS4_Anim.JYCoord * 100 * AS4_Anim.DeltaTime;
  AS4_Anim.cl.z += AS4_Anim.JZCoord * 100 * AS4_Anim.DeltaTime;
  if (Ani->viewfrom.x > 0)
    Ani->viewfrom.x += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime,
    Ani->viewfrom.y += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime,
    Ani->viewfrom.z += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime;
  else if (Ani->JRCoord > 0)
      Ani->viewfrom.x += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime,
      Ani->viewfrom.y += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime,
      Ani->viewfrom.z += AS4_Anim.JRCoord * 5 * AS4_Anim.DeltaTime;
    else
      Ani->viewfrom.x = Ani->viewfrom.y = Ani->viewfrom.z = 0;
} /* End of 'AS4_AnimUnitResponse' function */

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

    SetWindowPos(AS4_Anim.hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
    IsFullScreen = 1;
  } else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(AS4_Anim.hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    IsFullScreen = 0;
  }
} /* End of 'InfoFlipFullScreen' function */

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
