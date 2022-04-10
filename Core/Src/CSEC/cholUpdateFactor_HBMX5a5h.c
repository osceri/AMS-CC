#include "rtwtypes.h"
#include "cholUpdateFactor_HBMX5a5h.h"
#include <math.h>
#include "rotate_LZ7ctbe0.h"
#include "rt_nonfinite.h"
#include "rtGetNaN.h"

void cholUpdateFactor_HBMX5a5h(real_T *S, real_T U)
{
  real_T R;
  real_T alpha;
  real_T c;
  real_T nrmx;
  real_T s;
  int8_T p;
  R = *S;
  p = 0;
  if (*S == 0.0) {
    p = 2;
  } else {
    s = U / *S;
    nrmx = fabs(s);
    if (nrmx >= 1.0) {
      p = 1;
    } else {
      rotate_LZ7ctbe0(sqrt(1.0 - nrmx * nrmx), s, &R, &c, &alpha);
      R = R * *S - c * 0.0;
    }
  }

  *S = R;
  if (p != 0) {
    s = R * R - U * U;
    if ((!rtIsInf(s)) && (!rtIsNaN(s))) {
      nrmx = 1.0;
      if (s != 0.0) {
        s = fabs(s);
      }
    } else {
      s = (rtNaN);
      nrmx = (rtNaN);
    }

    *S = nrmx * sqrt(s);
  }
}
