#include "rtwtypes.h"
#include "UTMeanCovSqrt_C6QfB9Td.h"
#include "rt_nonfinite.h"
#include <math.h>
#include "xnrm2_7bm34mDQ.h"
#include "rt_hypotd_snf.h"
#include "cholUpdateFactor_HBMX5a5h.h"
#include "rotate_LZ7ctbe0.h"

void UTMeanCovSqrt_C6QfB9Td(const real_T meanWeights[2], const real_T
  covWeights[2], real_T OOM, real_T Y1, real_T Y2[6], const real_T X1[3], real_T
  X2[18], real_T *Ymean, real_T *Sy, real_T Pxy[3])
{
  real_T A[6];
  real_T X2_0[3];
  real_T covWeights_0;
  real_T covWeights_1;
  real_T signOOM;
  real_T xnorm;
  int32_T b_k;
  int32_T i;
  int32_T knt;
  *Ymean = Y1 * meanWeights[0];
  for (knt = 0; knt < 6; knt++) {
    *Ymean += Y2[knt] * meanWeights[1];
  }

  *Ymean *= OOM;
  Y1 -= *Ymean;
  for (knt = 0; knt < 6; knt++) {
    Y2[knt] -= *Ymean;
  }

  if (rtIsNaN(OOM)) {
    signOOM = OOM;
  } else if (OOM < 0.0) {
    signOOM = -1.0;
  } else {
    signOOM = (OOM > 0.0);
  }

  covWeights_0 = signOOM * covWeights[0];
  covWeights_1 = signOOM * covWeights[1];
  OOM *= signOOM;
  signOOM = sqrt(covWeights_1);
  for (knt = 0; knt < 6; knt++) {
    A[knt] = signOOM * Y2[knt];
  }

  signOOM = A[0];
  xnorm = xnrm2_7bm34mDQ(5, A, 2);
  if (xnorm != 0.0) {
    xnorm = rt_hypotd_snf(A[0], xnorm);
    if (A[0] >= 0.0) {
      xnorm = -xnorm;
    }

    if (fabs(xnorm) < 1.0020841800044864E-292) {
      knt = 0;
      do {
        knt++;
        for (b_k = 1; b_k < 6; b_k++) {
          A[b_k] *= 9.9792015476736E+291;
        }

        xnorm *= 9.9792015476736E+291;
        signOOM *= 9.9792015476736E+291;
      } while ((fabs(xnorm) < 1.0020841800044864E-292) && (knt < 20));

      xnorm = rt_hypotd_snf(signOOM, xnrm2_7bm34mDQ(5, A, 2));
      if (signOOM >= 0.0) {
        xnorm = -xnorm;
      }

      for (b_k = 0; b_k < knt; b_k++) {
        xnorm *= 1.0020841800044864E-292;
      }

      signOOM = xnorm;
    } else {
      signOOM = xnorm;
    }
  }

  if (!rtIsNaN(covWeights_0)) {
    if (covWeights_0 < 0.0) {
      covWeights_0 = -1.0;
    } else {
      covWeights_0 = (covWeights_0 > 0.0);
    }
  }

  if (covWeights_0 == 1.0) {
    rotate_LZ7ctbe0(signOOM, Y1, &signOOM, &xnorm, &covWeights_0);
    *Sy = sqrt(OOM) * covWeights_0;
  } else {
    cholUpdateFactor_HBMX5a5h(&signOOM, Y1);
    *Sy = sqrt(OOM) * signOOM;
  }

  b_k = 0;
  for (knt = 0; knt < 6; knt++) {
    X2[b_k] -= X1[0];
    X2[b_k + 1] -= X1[1];
    X2[b_k + 2] -= X1[2];
    b_k += 3;
  }

  signOOM = covWeights_1 * OOM;
  for (knt = 0; knt < 3; knt++) {
    X2_0[knt] = 0.0;
    b_k = 0;
    for (i = 0; i < 6; i++) {
      X2_0[knt] += X2[b_k + knt] * Y2[i];
      b_k += 3;
    }

    Pxy[knt] = X2_0[knt] * signOOM;
  }
}
