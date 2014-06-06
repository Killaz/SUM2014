#ifdef Release
      DBL sine_p, cosine_p, phi = ... ;
      __asm {
        fld phi
        fsincos
        fstp cosine_p
        fstp sine_p
      }

#define DEGREE2RADIANL 0.01745329251994329576L
/* Multiplyer degree to radian constant */
long double MultiplierDegree2Radian = DEGREE2RADIANL;

  _asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fld  st(0)
    fstp m.A11 /* cos -> a11 */
    fstp m.A22 /* cos -> a22 */

    fld  st(0) /* duplicate st(0) */
    fstp m.A12 /* sin -> a12 */
    fldz       /* push 0 (st(0) = 0) */
    fsubr      /* st(0) = 0 - sin() */
    fstp m.A21 /* -sin -> a21 */
  }
#endif
