/* FILENAME: ANIM.H
 * PROGRAMMER: AS4
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Максимальное количество элементов анимации */
#define AS4_MAX_UNITS 3000

#define AS4_JOYSTICK_THRESHOLD 128.0

/* Устранение ошибок замера джойстика */
#define AS4_GET_AXIS_VALUE(A) \
  ((INT)(AS4_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / \
   (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / AS4_JOYSTICK_THRESHOLD)


/* Объявление типа объектов анимации "вперед" */
typedef struct tagas4UNIT as4UNIT;

/* Структура хранения контекста анимации */
typedef struct tagas4ANIM
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HDC hDC;            /* Контекст окна */
  HGLRC hRC;           /* Контекст построения */

  /* Массив элементов анимации и их количество */
  as4UNIT *Units[AS4_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE
    Keys[256], KeysClick[256], KeysOld[256];
  DBL
    JX, JY, JZ, JR, JU;
  BYTE
    JButs[32], JButsOld[32], JButsClick[32];
  INT JPOV;
  BYTE mouse;
  INT
    MsGlobalX, MsGlobalY,
    MsDeltaX, MsDeltaY,
    MsX, MsY,
    MsXOld, MsYOld,
    MsGlobalWheel, MsWheel;

  DBL
    RndWs, RndHs,
    RndWp, RndHp,
    ProjDist, ProjSize, FarClip;

  MATR
    RndMatrWorld,
    RndMatrView,
    RndMatrProjection;
  POINT cl, lg;
  INT clz;
} as4ANIM;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - указатель на функцию инициализации
 *     VOID (*Init)( as4UNIT *Unit, as4ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( as4UNIT *Unit, as4ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( as4UNIT *Unit, as4ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( as4UNIT *Unit, as4ANIM *Ani );
 */
#define AS4_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( as4UNIT *Unit, as4ANIM *Ani );       \
  VOID (*Close)( as4UNIT *Unit, as4ANIM *Ani );      \
  VOID (*Response)( as4UNIT *Unit, as4ANIM *Ani );   \
  VOID (*Render)( as4UNIT *Unit, as4ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagas4UNIT
{
  AS4_UNIT_BASE_FIELDS; /* Базовые поля */
};

/***
 * Функции управления анимацией
 ***/

/* Функция инициализации анимации */
BOOL AS4_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID AS4_AnimClose( VOID );

/* Функция обработки изменения размера области вывода */
VOID AS4_AnimResize( INT W, INT H );

/* Функция построения кадра анимации */
VOID AS4_AnimRender( VOID );

VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani );

/* Функция вывода кадра анимации */
VOID AS4_AnimCopyFrame( VOID );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов */
VOID AS4_AnimFlipFullScreen( VOID );

/* Функция установки паузы анимации */
VOID AS4_AnimSetPause( BOOL NewPauseFlag );

/***
 * Функции обработки объектов анимации
 ***/

/* Функция добавления в систему объекта анимации */
VOID AS4_AnimAddUnit( as4UNIT *Unit );

/* Функция создания объекта анимации */
as4UNIT * AS4_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
