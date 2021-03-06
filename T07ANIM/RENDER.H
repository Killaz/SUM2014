#ifndef __RENDER_H_
#define __RENDER_H_

#include "VEC.H"
#include "GEOM.H"
#include "SHADER.H"

typedef struct
{
  VEC Loc, Dir, Up, Right, At;
} as4CAMERA;

typedef struct tagas4GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} as4GOBJ;

extern UINT AS4_ShaderProg; // GOBJ.c

BOOL AS4_RndGObjLoad( as4GOBJ *GObj, CHAR *FileName );
VOID AS4_RndGObjFree( as4GOBJ *GObj );
VOID AS4_RndGObjDraw( as4GOBJ *GObj );

VOID AS4_RndGeomFree( as4GEOM *Obj );

#endif
