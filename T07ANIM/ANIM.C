/* FILENAME: ANIM.C
 * PROGRAMMER: AS4
 * PURPOSE: Base animation module
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "VEC.H"


#define JXCoord JX
#define JYCoord JY

#ifndef SAITEK
#define JZCoord JZ
#define JRCoord JR
#define JBUTTON1 0
#define JBUTTON2 1
#define JBUTTON3 2
#define JBUTTON4 3
#define JBUTTON5 4
#define JBUTTON6 5
#define JBUTTON7 6
#define JBUTTON8 7
#define JBUTTON9 8
#define JBUTTON10 9
#define JBUTTON11 10
#define JBUTTON12 11
#else
#define JZCoord JR
#define JRCoord JZ
#define JBUTTON1 2
#define JBUTTON2 3
#define JBUTTON3 0
#define JBUTTON4 1
#define JBUTTON5 4
#define JBUTTON6 6
#define JBUTTON7 5
#define JBUTTON8 7
#define JBUTTON9 8
#define JBUTTON10 9
#define JBUTTON11 10
#define JBUTTON12 11
#endif

/* Системный контекст анимации */
static as4ANIM AS4_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  i = ChoosePixelFormat(AS4_Anim.hDC, &pfd);
  DescribePixelFormat(AS4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AS4_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  AS4_Anim.hRC = wglCreateContext(AS4_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(AS4_Anim.hDC, AS4_Anim.hRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AS4_Anim.hRC);
    ReleaseDC(AS4_Anim.hWnd, AS4_Anim.hDC);
    memset(&AS4_Anim, 0, sizeof(as4ANIM));
    return FALSE;
  }

  /* инициализируем таймер */
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

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
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

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_AnimResize( INT W, INT H )
{
  DBL Size = 1, ratio_x = 1, ratio_y = 1;
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

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       as4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_AnimAddUnit( as4UNIT *Unit )
{
  if (AS4_Anim.NumOfUnits < AS4_MAX_UNITS)
  {
    AS4_Anim.Units[AS4_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AS4_Anim);
  }
} /* End of 'AS4_AnimAddUnit' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 *ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS4_AnimRender( VOID )
{
  static BOOL rend = 0;
  INT i;
  LARGE_INTEGER li;
  if (rend)
    return;
  rend = 1;

  /* Обновление ввода */
  GetKeyboardState(AS4_Anim.Keys);
  for (i = 0; i < 256; i++)
    AS4_Anim.Keys[i] >>= 7;

  /* Обновление таймера */
  AS4_Anim.Time = (DBL)clock() / CLOCKS_PER_SEC;

  /* Обновление кадра */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  AS4_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AS4_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (AS4_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    AS4_Anim.DeltaTime = 0;
  }
  else
    AS4_Anim.DeltaTime = AS4_Anim.GlobalDeltaTime;

  AS4_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AS4_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
    AS4_Anim.Units[i]->Response(AS4_Anim.Units[i], &AS4_Anim);

  /* очистка фона */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* рисование объектов */
  glColor3d(1, 0, 0);
  glRectd(0, 0, 1, 1);
  glColor3d(0, 1, 0);
  glRectd(0.1, 0.1, 0.9, 0.9);

  for (i = 0; i < AS4_Anim.NumOfUnits; i++)
  {
    AS4_Anim.Units[i]->Render(AS4_Anim.Units[i], &AS4_Anim);
  }

  FrameCounter++;
  rend = 0;
} /* End of 'AS4_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

VOID AS4_AnimCopyFrame( VOID )
{
  /* вывод кадра */
  SwapBuffers(AS4_Anim.hDC);
} /* End of 'AS4_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       as4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani )
{
  INT i;
  GetKeyboardState(Ani->Keys);
  for (i = 0; i < 256; i++)
    Ani->Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    Ani->KeysClick[i] = Ani->Keys[i] && !Ani->KeysOld[i];
  if (Ani->KeysClick[VK_ESCAPE])
  {
    DestroyWindow(Ani->hWnd);
    return;
  }
  for (i = 0; i < 256; i++)
    Ani->KeysOld[i] = Ani->Keys[i];

  /* Мышь */
  if (Ani->mouse)
  {
    POINT pt;
    Ani->MsWheel = Ani->MsGlobalWheel;
    Ani->MsGlobalWheel = 0;
    /* абсолютная позиция */
    pt.x = Ani->MsGlobalX;
    pt.y = Ani->MsGlobalY;
    ScreenToClient(Ani->hWnd, &pt);
    Ani->MsX = pt.x;
    Ani->MsY = pt.y;
    /* относительное перемещение */
    Ani->MsDeltaX = Ani->MsGlobalX - Ani->MsXOld;
    Ani->MsDeltaY = Ani->MsGlobalY - Ani->MsYOld;
    Ani->MsXOld = Ani->MsGlobalX;
    Ani->MsYOld = Ani->MsGlobalY;
  }

  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
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
        /* Оси */
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
          AS4_Anim.clz += 2000 * AS4_Anim.DeltaTime;
        else if (Ani->JPOV == 7)
          AS4_Anim.clz -= 2000 * AS4_Anim.DeltaTime;
      }
    }
  }
  AS4_Anim.cl.x += AS4_Anim.JXCoord * 2000 * AS4_Anim.DeltaTime;
  AS4_Anim.cl.y += AS4_Anim.JYCoord * 2000 * AS4_Anim.DeltaTime;
} /* End of 'AS4_AnimUnitResponse' function */

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

VOID AS4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = 0; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AS4_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AS4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    /* для одного монитора:
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
    /* восстанавливаем размер окна */
    SetWindowPos(AS4_Anim.hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    IsFullScreen = 0;
  }
} /* End of 'InfoFlipFullScreen' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

VOID AS4_AnimSetPause( BOOL NewPauseFlag )
{
  AS4_Anim.IsPause = NewPauseFlag;
} /* End of 'AS4_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */
