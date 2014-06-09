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

#define AS4_LOGO_COUNT 3

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

/* Структура описания объекта анимации */
typedef struct tagas4UNIT_LOGO
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
  HBITMAP hBm_AND[AS4_LOGO_COUNT], hBm_XOR[AS4_LOGO_COUNT];
} as4UNIT_LOGO;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_COW *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
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
static VOID LogoUnitRender( as4UNIT_LOGO *Unit, as4ANIM *Ani )
{
  /*SYSTEMTIME st;*/
  BITMAP bm;
  HDC hScrDC, hMemDC1;
  INT n = time(0) % AS4_LOGO_COUNT;

  hScrDC = GetDC(NULL);
  hMemDC1 = CreateCompatibleDC(hScrDC);
  GetObject(Unit->hBm_AND[n], sizeof(bm), &bm);/**/

  SelectObject(hMemDC1, Unit->hBm_AND[n]);
  BitBlt(Ani->hDC, Ani->W - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCAND);
  SelectObject(hMemDC1, Unit->hBm_XOR[n]);
  BitBlt(Ani->hDC, Ani->W - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, hMemDC1, 0, 0, SRCINVERT);

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
  for (i = 0; i < AS4_LOGO_COUNT; i++)
  {
    sprintf(name, "img/YOBA_and%d.bmp", i);
    Unit->hBm_AND[i] = LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    sprintf(name, "img/YOBA_xor%d.bmp", i);
    Unit->hBm_XOR[i] = LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  }
  return (as4UNIT *)Unit;
} /* End of 'AS4_LogoUnitCreate' function */

static VOID InfoUnitRender( as4UNIT *Unit, as4ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(95, 30, 155));
  TextOut(Ani->hDC, Ani->W - 85, 256, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
} /* End of 'AS4_AnimUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */
as4UNIT * AS4_InfoUnitCreate( VOID )
{
  as4UNIT *Unit;

  if ((Unit = AS4_AnimUnitCreate(sizeof(as4UNIT))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Render = (VOID *)InfoUnitRender;
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

