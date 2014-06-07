/* FILENAME: UNIT.C
 * PROGRAMMER: AS4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 07.06.2014
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS4_AnimUnitInit( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS4_AnimUnitClose( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as4UNIT *Unit;
 *   - указатель на контекст анимации:
 *       as4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS4_AnimUnitRender( as4UNIT *Unit, as4ANIM *Ani )
{
} /* End of 'AS4_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as4UNIT *) указатель на созданный объект анимации.
 */
as4UNIT * AS4_AnimUnitCreate( INT Size )
{
  as4UNIT *Unit;

  if (Size < sizeof(as4UNIT) ||
      (Unit = malloc(Size)) == NULL)
    return NULL;
  memset(Unit, 0, Size);
  /* заполняем поля по-умолчанию */
  Unit->Size = Size;
  Unit->Init = AS4_AnimUnitInit;
  Unit->Close = AS4_AnimUnitClose;
  Unit->Response = AS4_AnimUnitResponse;
  Unit->Render = AS4_AnimUnitRender;
  return Unit;
} /* End of 'AS4_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

