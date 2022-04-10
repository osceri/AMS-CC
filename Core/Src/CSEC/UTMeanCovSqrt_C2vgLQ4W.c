#include "rtwtypes.h"
#include "UTMeanCovSqrt_C2vgLQ4W.h"
#include "rt_nonfinite.h"
#include <math.h>
#include "qr_DvOBrgQi.h"
#include "rotate_LZ7ctbe0.h"
#include "svd_u3DvUgOe.h"
#include <string.h>
#include "qr_5a2P8Gh6.h"

void UTMeanCovSqrt_C2vgLQ4W(const real_T meanWeights[2], const real_T
  covWeights[2], real_T OOM, real_T Y1[3], real_T Y2[18], const real_T X1[3],
  real_T X2[18], real_T Ymean[3], real_T Sy[9], real_T Pxy[9])
{
  real_T a__1[18];
  real_T temp_0[18];
  real_T Ss[9];
  real_T Sy_0[9];
  real_T Y1_0[9];
  real_T c[3];
  real_T s[3];
  real_T x[3];
  real_T absxk;
  real_T b_t;
  real_T covWeights_0;
  real_T covWeights_1;
  real_T scale;
  real_T signOOM;
  real_T temp;
  int32_T Sy_tmp;
  int32_T i;
  int32_T iAcol;
  int32_T i_0;
  int32_T i_1;
  int32_T kk;
  Ymean[0] = Y1[0] * meanWeights[0];
  Ymean[1] = meanWeights[0] * Y1[1];
  Ymean[2] = meanWeights[0] * Y1[2];
  iAcol = 0;
  for (kk = 0; kk < 6; kk++) {
    Ymean[0] += Y2[iAcol] * meanWeights[1];
    Ymean[1] += Y2[iAcol + 1] * meanWeights[1];
    Ymean[2] += Y2[iAcol + 2] * meanWeights[1];
    iAcol += 3;
  }

  signOOM = Ymean[0] * OOM;
  scale = Y1[0] - signOOM;
  Ymean[0] = signOOM;
  Y1[0] = scale;
  signOOM = Ymean[1] * OOM;
  absxk = Y1[1] - signOOM;
  Ymean[1] = signOOM;
  Y1[1] = absxk;
  signOOM = Ymean[2] * OOM;
  b_t = Y1[2] - signOOM;
  Ymean[2] = signOOM;
  Y1[2] = b_t;
  iAcol = 0;
  for (kk = 0; kk < 6; kk++) {
    Y2[iAcol] -= Ymean[0];
    Y2[iAcol + 1] -= Ymean[1];
    Y2[iAcol + 2] -= signOOM;
    iAcol += 3;
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
  signOOM = sqrt(OOM);
  temp = sqrt(covWeights_1);
  kk = 0;
  for (iAcol = 0; iAcol < 3; iAcol++) {
    i_1 = 0;
    for (i = 0; i < 6; i++) {
      temp_0[i + kk] = Y2[i_1 + iAcol] * temp;
      i_1 += 3;
    }

    kk += 6;
  }

  qr_DvOBrgQi(temp_0, a__1, Sy);
  if (!rtIsNaN(covWeights_0)) {
    if (covWeights_0 < 0.0) {
      covWeights_0 = -1.0;
    } else {
      covWeights_0 = (covWeights_0 > 0.0);
    }
  }

  if (covWeights_0 == 1.0) {
    for (iAcol = 0; iAcol < 2; iAcol++) {
      Sy[iAcol + 1] = 0.0;
    }

    Sy[5] = 0.0;
    c[2] = 0.0;
    s[2] = 0.0;
    rotate_LZ7ctbe0(Sy[0], scale, &c[0], &s[0], &Sy[0]);
    temp = c[0] * absxk - s[0] * Sy[3];
    Sy[3] = c[0] * Sy[3] + s[0] * absxk;
    rotate_LZ7ctbe0(Sy[4], temp, &c[1], &s[1], &Sy[4]);
    temp = b_t;
    for (iAcol = 0; iAcol < 2; iAcol++) {
      scale = s[iAcol] * temp;
      absxk = Sy[iAcol + 6];
      temp = c[iAcol] * temp - absxk * s[iAcol];
      Sy[iAcol + 6] = absxk * c[iAcol] + scale;
    }

    rotate_LZ7ctbe0(Sy[8], temp, &c[2], &s[2], &Sy[8]);
    kk = 0;
    for (iAcol = 0; iAcol < 3; iAcol++) {
      Sy_0[kk] = Sy[iAcol];
      Sy_0[kk + 1] = Sy[iAcol + 3];
      Sy_0[kk + 2] = Sy[iAcol + 6];
      kk += 3;
    }

    for (kk = 0; kk < 9; kk++) {
      Sy[kk] = signOOM * Sy_0[kk];
    }
  } else {
    boolean_T errorCondition;
    boolean_T guard1 = false;
    for (iAcol = 0; iAcol < 2; iAcol++) {
      Sy[iAcol + 1] = 0.0;
    }

    Sy[5] = 0.0;
    errorCondition = (Sy[0] == 0.0);
    if (!errorCondition) {
      errorCondition = (Sy[4] == 0.0);
    }

    if (!errorCondition) {
      errorCondition = (Sy[8] == 0.0);
    }

    guard1 = false;
    if (errorCondition) {
      guard1 = true;
    } else {
      x[2] = b_t;
      x[0] = scale / Sy[0];
      x[1] = (absxk - x[0] * Sy[3]) / Sy[4];
      temp = b_t;
      for (kk = 0; kk < 2; kk++) {
        temp -= Sy[kk + 6] * x[kk];
      }

      x[2] = temp / Sy[8];
      scale = 3.3121686421112381E-170;
      absxk = fabs(x[0]);
      if (absxk > 3.3121686421112381E-170) {
        temp = 1.0;
        scale = absxk;
      } else {
        b_t = absxk / 3.3121686421112381E-170;
        temp = b_t * b_t;
      }

      absxk = fabs(x[1]);
      if (absxk > scale) {
        b_t = scale / absxk;
        temp = temp * b_t * b_t + 1.0;
        scale = absxk;
      } else {
        b_t = absxk / scale;
        temp += b_t * b_t;
      }

      absxk = fabs(x[2]);
      if (absxk > scale) {
        b_t = scale / absxk;
        temp = temp * b_t * b_t + 1.0;
        scale = absxk;
      } else {
        b_t = absxk / scale;
        temp += b_t * b_t;
      }

      temp = scale * sqrt(temp);
      if (temp >= 1.0) {
        guard1 = true;
      } else {
        rotate_LZ7ctbe0(sqrt(1.0 - temp * temp), x[2], &c[2], &s[2], &temp);
        x[2] = 0.0;
        rotate_LZ7ctbe0(temp, x[1], &c[1], &s[1], &temp);
        x[1] = 0.0;
        rotate_LZ7ctbe0(temp, x[0], &c[0], &s[0], &temp);
        x[0] = 0.0;
        for (iAcol = 0; iAcol + 1 > 0; iAcol--) {
          temp = s[iAcol] * Sy[iAcol];
          Sy[iAcol] = c[iAcol] * Sy[iAcol] - s[iAcol] * x[0];
          x[0] = c[iAcol] * x[0] + temp;
        }

        for (iAcol = 1; iAcol + 1 > 0; iAcol--) {
          absxk = Sy[iAcol + 3];
          Sy[iAcol + 3] = absxk * c[iAcol] - s[iAcol] * x[1];
          x[1] = c[iAcol] * x[1] + absxk * s[iAcol];
        }

        for (iAcol = 2; iAcol + 1 > 0; iAcol--) {
          absxk = Sy[iAcol + 6];
          Sy[iAcol + 6] = absxk * c[iAcol] - s[iAcol] * x[2];
          x[2] = c[iAcol] * x[2] + absxk * s[iAcol];
        }
      }
    }

    if (guard1) {
      boolean_T exitg2;
      kk = 0;
      for (iAcol = 0; iAcol < 3; iAcol++) {
        i_1 = 0;
        for (i = 0; i < 3; i++) {
          Sy_tmp = i_1 + iAcol;
          Sy_0[Sy_tmp] = 0.0;
          Sy_0[Sy_tmp] += Sy[kk] * Sy[i_1];
          Sy_0[Sy_tmp] += Sy[kk + 1] * Sy[i_1 + 1];
          Sy_0[Sy_tmp] += Sy[kk + 2] * Sy[i_1 + 2];
          Y1_0[i + kk] = Y1[i] * Y1[iAcol];
          i_1 += 3;
        }

        kk += 3;
      }

      errorCondition = true;
      for (iAcol = 0; iAcol < 9; iAcol++) {
        temp = Sy_0[iAcol] - Y1_0[iAcol];
        if (errorCondition && (rtIsInf(temp) || rtIsNaN(temp))) {
          errorCondition = false;
        }

        Ss[iAcol] = temp;
      }

      if (errorCondition) {
        svd_u3DvUgOe(Ss, Y1_0, s, Sy_0);
      } else {
        s[0] = (rtNaN);
        s[1] = (rtNaN);
        s[2] = (rtNaN);
        for (kk = 0; kk < 9; kk++) {
          Sy_0[kk] = (rtNaN);
        }
      }

      memset(&Ss[0], 0, 9U * sizeof(real_T));
      Ss[0] = s[0];
      Ss[4] = s[1];
      Ss[8] = s[2];
      for (kk = 0; kk < 9; kk++) {
        Ss[kk] = sqrt(Ss[kk]);
      }

      kk = 0;
      for (iAcol = 0; iAcol < 3; iAcol++) {
        i_1 = 0;
        for (i = 0; i < 3; i++) {
          Sy_tmp = i_1 + iAcol;
          Sy[Sy_tmp] = 0.0;
          Sy[Sy_tmp] += Ss[kk] * Sy_0[i];
          Sy[Sy_tmp] += Ss[kk + 1] * Sy_0[i + 3];
          Sy[Sy_tmp] += Ss[kk + 2] * Sy_0[i + 6];
          i_1 += 3;
        }

        kk += 3;
      }

      errorCondition = true;
      kk = 0;
      exitg2 = false;
      while ((!exitg2) && (kk < 3)) {
        int32_T exitg1;
        iAcol = kk + 1;
        do {
          exitg1 = 0;
          if (iAcol + 1 < 4) {
            if (!(Sy[3 * kk + iAcol] == 0.0)) {
              errorCondition = false;
              exitg1 = 1;
            } else {
              iAcol++;
            }
          } else {
            kk++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }

      if (!errorCondition) {
        memcpy(&Sy_0[0], &Sy[0], 9U * sizeof(real_T));
        qr_5a2P8Gh6(Sy_0, Y1_0, Sy);
      }
    }

    kk = 0;
    for (iAcol = 0; iAcol < 3; iAcol++) {
      Sy_0[kk] = Sy[iAcol];
      Sy_0[kk + 1] = Sy[iAcol + 3];
      Sy_0[kk + 2] = Sy[iAcol + 6];
      kk += 3;
    }

    for (kk = 0; kk < 9; kk++) {
      Sy[kk] = signOOM * Sy_0[kk];
    }
  }

  iAcol = 0;
  for (kk = 0; kk < 6; kk++) {
    X2[iAcol] -= X1[0];
    X2[iAcol + 1] -= X1[1];
    X2[iAcol + 2] -= X1[2];
    iAcol += 3;
  }

  signOOM = covWeights_1 * OOM;
  for (kk = 0; kk < 3; kk++) {
    iAcol = 0;
    for (i_1 = 0; i_1 < 3; i_1++) {
      Sy_tmp = iAcol + kk;
      Ss[Sy_tmp] = 0.0;
      i = 0;
      for (i_0 = 0; i_0 < 6; i_0++) {
        Ss[Sy_tmp] += X2[i + kk] * Y2[i + i_1];
        i += 3;
      }

      iAcol += 3;
    }
  }

  for (kk = 0; kk < 9; kk++) {
    Pxy[kk] = Ss[kk] * signOOM;
  }
}
