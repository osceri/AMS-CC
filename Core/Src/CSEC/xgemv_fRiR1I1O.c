#include "rtwtypes.h"
#include "xgemv_fRiR1I1O.h"

void xgemv_fRiR1I1O(int32_T m, int32_T n, const real_T A[9], int32_T ia0, const
                    real_T x[9], int32_T ix0, real_T y[3])
{
  int32_T b_iy;
  int32_T ia;
  int32_T iac;
  if ((m != 0) && (n != 0)) {
    int32_T b;
    for (b_iy = 0; b_iy < n; b_iy++) {
      y[b_iy] = 0.0;
    }

    b_iy = 0;
    b = (n - 1) * 3 + ia0;
    for (iac = ia0; iac <= b; iac += 3) {
      real_T c;
      int32_T d;
      int32_T ix;
      ix = ix0;
      c = 0.0;
      d = (iac + m) - 1;
      for (ia = iac; ia <= d; ia++) {
        c += A[ia - 1] * x[ix - 1];
        ix++;
      }

      y[b_iy] += c;
      b_iy++;
    }
  }
}
