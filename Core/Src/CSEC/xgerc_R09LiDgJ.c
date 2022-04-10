#include "rtwtypes.h"
#include "xgerc_R09LiDgJ.h"

void xgerc_R09LiDgJ(int32_T m, int32_T n, real_T alpha1, int32_T ix0, const
                    real_T y[3], real_T A[18], int32_T ia0)
{
  int32_T j;
  if (!(alpha1 == 0.0)) {
    int32_T jA;
    int32_T jy;
    jA = ia0 - 1;
    jy = 0;
    for (j = 0; j < n; j++) {
      if (y[jy] != 0.0) {
        real_T temp;
        int32_T b;
        int32_T ijA;
        int32_T ix;
        temp = y[jy] * alpha1;
        ix = ix0;
        ijA = jA;
        b = m + jA;
        while (ijA + 1 <= b) {
          A[ijA] += A[ix - 1] * temp;
          ix++;
          ijA++;
        }
      }

      jy++;
      jA += 6;
    }
  }
}
