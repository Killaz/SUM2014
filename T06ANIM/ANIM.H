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

#define AS4_JOYSTICK_THRESHOLD 128.0

/* ���������� ������ ������ ��������� */
#define AS4_GET_AXIS_VALUE(A) \
  ((INT)(AS4_JOYSTICK_THRESHOLD * (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / \
   (jc.w ## A ## max - jc.w ## A ##min - 1) - 1) + 0.5) / AS4_JOYSTICK_THRESHOLD)


/* ���������� ���� �������� �������� "������" */
typedef struct tagas4UNIT as4UNIT;

/* ��������� �������� ��������� �������� */
typedef struct tagas4ANIM
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

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
  BYTE Keys[256];     /* �������� ������ ���������� � ���� */
} as4ANIM;

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
VOID AS4_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID AS4_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������ */
VOID AS4_AnimResize( INT W, INT H );

/* ������� ���������� ����� �������� */
VOID AS4_AnimRender( VOID );

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
