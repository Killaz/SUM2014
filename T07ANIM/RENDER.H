#ifndef __RENDER_H_
#define __RENDER_H_

#include "VEC.H"

typedef struct
{
  VEC Loc, Dir, Up, Right, At;
} as4CAMERA;

typedef struct tagas4GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} as4GOBJ;

BOOL AS4_RndGObjLoad( as4GOBJ *GObj, CHAR *FileName );
VOID AS4_RndGObjFree( as4GOBJ *GObj );
VOID AS4_RndGObjDraw( as4GOBJ *GObj, HDC hDC );

#endif
