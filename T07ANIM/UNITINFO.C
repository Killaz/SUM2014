/* FILENAME: UNITINFO.C
 * PROGRAMMER: AS
 * PURPOSE: Animation unit samples handle module.
 * LAST UPDATE: 13.06.2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ANIM.H"
#include "units.h"

/* Структура описания объекта анимации */
typedef struct tagas4UNIT_INFO
{
  AS4_UNIT_BASE_FIELDS; /* Включение базовых полей */
} as4UNIT_INFO;

/* Функция инициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitInit( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
} /* End of 'InfoUnitInit' function */

/* Функция деинициализации информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitClose( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
} /* End of 'InfoUnitClose' function */

/* Функция обновления межкадровых параметров информационного
 * объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT_INFO *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitResponse( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
}


/* Функция построения информационного объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4UNIT_INFO *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID InfoUnitRender( as4UNIT_INFO *Unit, as4ANIM *Ani )
{
  static CHAR Buf[300];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
} /* End of 'InfoUnitRender' function */

/* Функция создания информационного объекта анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT_INFO *) указатель на созданный объект анимации.
 */
as4UNIT * AS4_InfoUnitCreate( VOID )
{
  as4UNIT_INFO *Unit;

  if ((Unit = (as4UNIT_INFO *)AS4_AnimUnitCreate(sizeof(as4UNIT_INFO))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Unit->Init = (VOID *)InfoUnitInit;
  Unit->Close = (VOID *)InfoUnitClose;
  Unit->Render = (VOID *)InfoUnitRender;
  Unit->Response = (VOID *)AS4_AnimUnitResponse;
  return (as4UNIT *)Unit;
} /* End of 'AS4_InfoUnitCreate' function */

/* END OF 'UNITINFO.C' FILE */
