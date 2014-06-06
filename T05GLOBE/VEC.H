#ifndef __VECT_H_
#define __VECT_H_

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef sqr
#define sqr(a) ((a) * (a))
#endif
#define D2R(A) ((A) * (M_PI / 180.0))

typedef double DBL;
#ifndef _WINDOWS_
typedef int INT;
#endif


typedef struct tagVEC
{
  DBL x, y, z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

/*__inline VEC VecDotVec( VEC V1, VEC V2 )
{
  return V1.x * V2.x + V1.y *  V2.y;
}*/

#endif
