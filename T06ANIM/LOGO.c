/* FILENAME: SAMPLE.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "anim.h"
#include <mmsystem.h>

#define AS4_LOGO_COUNT 7

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#pragma comment(lib, "winmm")

/* Структура описания объекта анимации */
typedef struct tagas4UNIT_LOGO
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
  HBITMAP hBm_AND[AS4_LOGO_COUNT], hBm_XOR[AS4_LOGO_COUNT];
} as4UNIT_LOGO;

typedef struct tagas4UNIT_INFO
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
} as4UNIT_INFO;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

struct
{
  BYTE
    Keys[256], KeysClick[256], KeysOld[256];
  DBL
    JX, JY, JZ, JR, JU;
  BYTE
    JButs[32], JButsOld[32], JButsClick[32];
  INT JPOV;
} AS4_Anim;

POINT lg, cl;
static INT type = 0, lgspeed;

static VOID LogoUnitInit( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID LogoUnitClose( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID LogoUnitResponse( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
} /* End of 'LogoUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

INT Aver( INT c, INT a, INT b )
{
  if (c < a)
    c += (b - a) * ((a - c) / (b - a) + 1);
  else if (c > b)
    c -= (b - a) * ((c - b) / (b - a) + 1);
  return c;
}

static VOID LogoUnitRender( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
  /*SYSTEMTIME st;*/
  INT LGx, LGy; 
  BITMAP bm;
  HDC hScrDC, hMemDC1;
  //INT type = time(0) % AS4_LOGO_COUNT;

  hScrDC = GetDC(NULL);
  hMemDC1 = CreateCompatibleDC(hScrDC);
  GetObject(Unit->hBm_AND[type], sizeof(bm), &bm);/**/

  LGx = Aver(Ani->W - bm.bmWidth + lg.x, -256, Ani->W);
  LGy = Aver(lg.y, -256, Ani->H);

  SelectObject(hMemDC1, Unit->hBm_AND[type]);
  BitBlt(Ani->hDC, LGx, LGy, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCAND);
  SelectObject(hMemDC1, Unit->hBm_XOR[type]);
  BitBlt(Ani->hDC, LGx, LGy, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCINVERT);

  ReleaseDC(NULL, hScrDC);
  DeleteDC(hMemDC1);
} /* End of 'AS4_LogoUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */
as4UNIT * AS4_LogoUnitCreate( VOID )
{
  as4UNIT_LOGO *Unit;
  INT i;
  char name[50];

  if ((Unit = (as4UNIT_LOGO *)AS4_AnimUnitCreate(sizeof(as4UNIT_LOGO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)LogoUnitInit;
  Unit->Close = (VOID *)LogoUnitClose;
  Unit->Response = (VOID *)LogoUnitResponse;                                                  
  Unit->Render = (VOID *)LogoUnitRender;
  lgspeed = 10;
  for (i = 0; i < AS4_LOGO_COUNT; i++)
  {
    sprintf(name, "img\\YOBA_%d_and.bmp", i);
    Unit->hBm_AND[i] = LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    sprintf(name, "img\\YOBA_%d_xor.bmp", i);
    Unit->hBm_XOR[i] = LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  }
  return (as4UNIT *)Unit;
} /* End of 'AS4_LogoUnitCreate' function */

static VOID InfoUnitRender( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
  static CHAR Buf[15];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(95, 30, 155));
  TextOut(Ani->hDC, Ani->W - 85, 256, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'AS4_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */

static VOID InfoUnitResponse( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
  INT i;
  GetKeyboardState(AS4_Anim.Keys);
  for (i = 0; i < 256; i++)
    AS4_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AS4_Anim.KeysClick[i] = AS4_Anim.Keys[i] && !AS4_Anim.KeysOld[i];
  if (AS4_Anim.KeysClick[VK_ESCAPE])
  {
    DestroyWindow(Ani->hWnd);
    return;
  }
  if (AS4_Anim.KeysClick['F'])
    AS4_AnimFlipFullScreen();
  if (AS4_Anim.KeysClick['P'])
    AS4_AnimSetPause(!Ani->IsPause);
  if (AS4_Anim.KeysClick[VK_NUMPAD0])
    cl.x = cl.y = lg.x = lg.y = 0;
  for (i = 0; i < 256; i++)
    AS4_Anim.KeysOld[i] = AS4_Anim.Keys[i];

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
        memcpy(AS4_Anim.JButsOld, AS4_Anim.JButs, sizeof(AS4_Anim.JButs));
        for (i = 0; i < 32; i++)
          AS4_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AS4_Anim.JButsClick[i] = AS4_Anim.JButs[i] && !AS4_Anim.JButsOld[i];
        if (AS4_Anim.JButsClick[4])
          type = ((type - 1) + AS4_LOGO_COUNT) % AS4_LOGO_COUNT;
        if (AS4_Anim.JButsClick[6])
          type = (type + 1) % AS4_LOGO_COUNT;
        if (AS4_Anim.JButsClick[0])
          if (lgspeed > 0)
            lgspeed -= 5;
        if (AS4_Anim.JButsClick[3])
            lgspeed += 5;
        /* Оси */
        AS4_Anim.JX = AS4_GET_AXIS_VALUE(X);/*2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;*/
        AS4_Anim.JY = AS4_GET_AXIS_VALUE(Y);/*2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASZ)
          AS4_Anim.JZ = AS4_GET_AXIS_VALUE(Z);/*2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASR)
          AS4_Anim.JR = AS4_GET_AXIS_VALUE(R);/*2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;*/
        if (jc.wCaps & JOYCAPS_HASU)
          AS4_Anim.JU = AS4_GET_AXIS_VALUE(U);/*2.0 * (ji.dwUpos - jc.wUmin) / (jc.wUmax - jc.wUmin - 1) - 1;*/

        /* Point-Of-View */
        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AS4_Anim.JPOV = 0;
          else
            AS4_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
  cl.x += AS4_Anim.JX * 10;
  cl.y += AS4_Anim.JY * 10;
  lg.x += AS4_Anim.JR * lgspeed;
  lg.y += AS4_Anim.JZ * lgspeed;
} /* End of 'LogoUnitResponse' function */

as4UNIT * AS4_InfoUnitCreate( VOID )
{
  as4UNIT *Unit;

  if ((Unit = AS4_AnimUnitCreate(sizeof(as4UNIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)InfoUnitResponse;
  return Unit;
} /* End of 'AS4_InfoUnitCreate' function */

/***********************************************************/

/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

/* END OF 'SAMPLE.C' FILE */

