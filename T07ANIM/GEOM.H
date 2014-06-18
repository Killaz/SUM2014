#include "RENDER.H"

#ifndef __GEOM_H_
#define __GEOM_H_

/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagas4UV
{
  FLT U, V; /* компоненты текстурных координат */
} as4UV;

/* Функиця инициализации текстурных координат */
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

/* Теп представления вершины */
typedef struct tagas4VERTEX
{
  VEC P;    /* координаты */
  as4UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} as4VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagas4PRIM_TYPE
{
  AS4_PRIM_TRIMESH, AS4_PRIM_GRID
} as4PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagas4PRIM
{
  as4PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  as4VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
} as4PRIM;

/* Цвет по-умолчанию */
extern COLOR AS4_DefaultColor;

/* Функция создания примитива */
BOOL AS4_PrimCreate( as4PRIM *P, as4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL AS4_PrimCreateSphere( as4PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция удаления примитива */
VOID AS4_PrimFree( as4PRIM *P );

/* Функция отрисовки примитива */
VOID AS4_PrimDraw( as4PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagas4MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} as4MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagas4GEOM
{
  INT NumOfPrims, NumOfMtls; /* размерв массивов */
  as4MATERIAL *Mtls;         /* таблица материалов */
  as4PRIM *Prims;            /* массив примитивов */
  VEC Len;
} as4GEOM;

/* Функция добавления материала к объекту */
INT AS4_GeomAddMaterial( as4GEOM *G,
                         as4MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT AS4_GeomAddPrim( as4GEOM *G, as4PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID AS4_GeomFree( as4GEOM *G );

/* Функция рисования */
VOID AS4_GeomDraw( as4GEOM *G );

/* Функция загрузки геометрического объекта */
BOOL AS4_GeomLoad( as4GEOM *G, CHAR *FileName );

/* Функция преобразования вершин */
VOID AS4_GeomTransform( as4GEOM *G, MATR M );

/* Функция перевычисления нормалей у примитива */
VOID AS4_PrimAutoNormals( as4PRIM *P );

#endif
