#include "RENDER.H"

#ifndef __GEOM_H_
#define __GEOM_H_

/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagas4UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} as4UV;

/* ������� ������������� ���������� ��������� */
__inline as4UV AS4_UVSet( FLT U, FLT V )
{
  as4UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'AS4_UVSet' function */

typedef struct tagCOLOR
{
  FLT r, g, b, a;
} COLOR;

__inline COLOR ColorSet( FLT r, FLT g, FLT b )
{
  COLOR c;
  c.r = r;
  c.g = g;
  c.b = b;
  c.a = 1;
  return c;
}

__inline COLOR ColorSeta( FLT r, FLT g, FLT b, FLT a )
{
  COLOR c;
  c.r = r;
  c.g = g;
  c.b = b;
  c.a = a;
  return c;
}

/* ��� ������������� ������� */
typedef struct tagas4VERTEX
{
  VEC P;    /* ���������� */
  as4UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} as4VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagas4PRIM_TYPE
{
  AS4_PRIM_TRIMESH, AS4_PRIM_GRID
} as4PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagas4PRIM
{
  as4PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  as4VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} as4PRIM;

/* ���� ��-��������� */
extern COLOR AS4_DefaultColor;

/* ������� �������� ��������� */
BOOL AS4_PrimCreate( as4PRIM *P, as4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL AS4_PrimCreateSphere( as4PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID AS4_PrimFree( as4PRIM *P );

/* ������� ��������� ��������� */
VOID AS4_PrimDraw( as4PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagas4MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} as4MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagas4GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  as4MATERIAL *Mtls;         /* ������� ���������� */
  as4PRIM *Prims;            /* ������ ���������� */
  VEC Len;
} as4GEOM;

/* ������� ���������� ��������� � ������� */
INT AS4_GeomAddMaterial( as4GEOM *G,
                         as4MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT AS4_GeomAddPrim( as4GEOM *G, as4PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID AS4_GeomFree( as4GEOM *G );

/* ������� ��������� */
VOID AS4_GeomDraw( as4GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL AS4_GeomLoad( as4GEOM *G, CHAR *FileName );

/* ������� �������������� ������ */
VOID AS4_GeomTransform( as4GEOM *G, MATR M );

/* ������� �������������� �������� � ��������� */
VOID AS4_PrimAutoNormals( as4PRIM *P );

#endif
