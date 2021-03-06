/* FILENAME: ANIM.H
 * PROGRAMMER: AS4
 * PURPOSE: Globe project declaration module.
 * LAST UPDATE: 07.06.2014
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* ������������ ���������� ��������� �������� */
#define AS4_MAX_UNITS 3000

#define AS4_JOYSTICK_THRESHOLD 256.0

/* ���������� ������ ������ ��������� */
#define AS4_GET_AXIS_VALUE(A) \
  ((INT)(AS4_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / \
   (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / AS4_JOYSTICK_THRESHOLD)

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

#define NUMOFMODELS 3

/* ���������� ���� �������� �������� "������" */
typedef struct tagas4UNIT as4UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagas4ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ���� */
  HGLRC hRC;           /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  as4UNIT *Units[AS4_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE
    Keys[256], KeysClick[256], KeysOld[256];
  DBL
    JX, JY, JZ, JR, JU;
  BYTE
    JButs[32], JButsOld[32], JButsClick[32];
  INT JPOV;
  BYTE mouse;
  BOOL PolMode, mute;
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
  VEC viewto, viewfrom, cl, lg;
  VEC Disc;
  INT movecoef, CarModel;
} as4ANIM;

extern as4ANIM AS4_Anim;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( as4UNIT *Unit, as4ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( as4UNIT *Unit, as4ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( as4UNIT *Unit, as4ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( as4UNIT *Unit, as4ANIM *Ani );
 */
#define AS4_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( as4UNIT *Unit, as4ANIM *Ani );       \
  VOID (*Close)( as4UNIT *Unit, as4ANIM *Ani );      \
  VOID (*Response)( as4UNIT *Unit, as4ANIM *Ani );   \
  VOID (*Render)( as4UNIT *Unit, as4ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagas4UNIT
{
  AS4_UNIT_BASE_FIELDS; /* ������� ���� */
};

/***
 * ������� ���������� ���������
 ***/

/* ������� ������������� �������� */
BOOL AS4_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID AS4_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID AS4_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID AS4_AnimRender( VOID );

VOID AS4_AnimUnitResponse( as4UNIT *Unit, as4ANIM *Ani );

/* ������� ������ ����� �������� */
VOID AS4_AnimCopyFrame( VOID );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ��������� */
VOID AS4_AnimFlipFullScreen( VOID );

/* ������� ��������� ����� �������� */
VOID AS4_AnimSetPause( BOOL NewPauseFlag );

/***
 * ������� ��������� �������� ��������
 ***/

/* ������� ���������� � ������� ������� �������� */
VOID AS4_AnimAddUnit( as4UNIT *Unit );

/* ������� �������� ������� �������� */
as4UNIT * AS4_AnimUnitCreate( INT Size );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
