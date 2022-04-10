#include "CSE.h"
#include "rtwtypes.h"
#include "CSE_private.h"
#include <math.h>
#include "trisolve_BC1XmAGq.h"
#include "UTMeanCovSqrt_C2vgLQ4W.h"
#include <string.h>
#include "qr_DvOBrgQi.h"
#include "svd_u3DvUgOe.h"
#include "rt_nonfinite.h"
#include "rotate_LZ7ctbe0.h"
#include "qr_5a2P8Gh6.h"
#include "UTMeanCovSqrt_C6QfB9Td.h"
#include "xnrm2_Mnmxg9YT.h"
#include "rt_hypotd_snf.h"
#include "look1_binlxpw.h"

DW_CSE_T CSE_DW;
ExtU_CSE_T CSE_U;
ExtY_CSE_T CSE_Y;
static RT_MODEL_CSE_T CSE_M_;
RT_MODEL_CSE_T *const CSE_M = &CSE_M_;
static void UKFCorrectorAdditive_getPredict(real_T Rs, const real_T X1[3], const
  real_T S[9], real_T alpha, real_T beta, real_T kappa, real_T *Ymean, real_T
  Pxy[3], real_T *Sy);
void CSE_S_x(const real_T rtu_x[3], real_T rty_x_next[3])
{
  real_T rtb_Divide;
  real_T tmp;
  rtb_Divide = rtu_x[1] / rtu_x[2];
  tmp = exp(-CSE_P.Ts / look1_binlxpw(rtb_Divide, CSE_P.soc, CSE_P.tau, 12U));
  rty_x_next[0] = (1.0 - tmp) * CSE_U.u * look1_binlxpw(rtb_Divide, CSE_P.soc,
    CSE_P.Rp, 12U) + tmp * rtu_x[0];
  rty_x_next[1] = CSE_U.u * CSE_P.Ts + rtu_x[1];
  rty_x_next[2] = rtu_x[2];
}

void CSE_M_x(const real_T rtu_x[3], real_T *rty_y)
{
  real_T rtb_Divide;
  rtb_Divide = rtu_x[1] / rtu_x[2];
  *rty_y = (CSE_U.u * look1_binlxpw(rtb_Divide, CSE_P.soc, CSE_P.Rs, 12U) +
            look1_binlxpw(rtb_Divide, CSE_P.soc, CSE_P.ocv, 12U)) + rtu_x[0];
}

static void UKFCorrectorAdditive_getPredict(real_T Rs, const real_T X1[3], const
  real_T S[9], real_T alpha, real_T beta, real_T kappa, real_T *Ymean, real_T
  Pxy[3], real_T *Sy)
{
  real_T X2[18];
  real_T Y2[6];
  real_T Y2_0[6];
  real_T Wcov[2];
  real_T Wmean[2];
  real_T tempY;
  real_T tempY_tmp;
  int32_T knt;
  int32_T knt_0;
  tempY_tmp = alpha * alpha;
  tempY = (kappa + 3.0) * tempY_tmp;
  Wmean[0] = 1.0 - 3.0 / tempY;
  Wmean[1] = 1.0 / (2.0 * tempY);
  Wcov[0] = ((1.0 - tempY_tmp) + beta) + Wmean[0];
  Wcov[1] = Wmean[1];
  if (Wmean[0] != 0.0) {
    real_T Wcov_0;
    *Sy = Wmean[0];
    tempY_tmp = Wmean[0];
    Wmean[0] /= Wmean[0];
    Wcov[0] /= *Sy;
    Wcov_0 = Wmean[1];
    Wmean[1] /= tempY_tmp;
    Wcov[1] = Wcov_0 / *Sy;
  } else {
    *Sy = 1.0;
  }

  tempY = sqrt(tempY);
  for (knt = 0; knt < 9; knt++) {
    tempY_tmp = tempY * S[knt];
    X2[knt] = tempY_tmp;
    X2[knt + 9] = -tempY_tmp;
  }

  knt_0 = 0;
  for (knt = 0; knt < 6; knt++) {
    X2[knt_0] += X1[0];
    X2[knt_0 + 1] += X1[1];
    X2[knt_0 + 2] += X1[2];
    knt_0 += 3;
  }

  knt_0 = 0;
  for (knt = 0; knt < 6; knt++) {
    CSE_M_x(&X2[knt_0], &Y2[knt]);
    knt_0 += 3;
  }

  CSE_M_x(X1, &tempY);
  for (knt = 0; knt < 6; knt++) {
    Y2_0[knt] = (Y2[knt] - tempY) + tempY;
  }

  UTMeanCovSqrt_C6QfB9Td(Wmean, Wcov, *Sy, tempY, Y2_0, X1, X2, Ymean,
    &tempY_tmp, Pxy);
  Wmean[0] = tempY_tmp;
  Wmean[1] = Rs;
  *Sy = tempY_tmp;
  tempY = xnrm2_Mnmxg9YT(1, Wmean, 2);
  if (tempY != 0.0) {
    tempY = rt_hypotd_snf(tempY_tmp, tempY);
    if (tempY_tmp >= 0.0) {
      tempY = -tempY;
    }

    if (fabs(tempY) < 1.0020841800044864E-292) {
      knt = 0;
      do {
        knt++;
        Wmean[1] *= 9.9792015476736E+291;
        tempY *= 9.9792015476736E+291;
        *Sy *= 9.9792015476736E+291;
      } while ((fabs(tempY) < 1.0020841800044864E-292) && (knt < 20));

      tempY = rt_hypotd_snf(*Sy, xnrm2_Mnmxg9YT(1, Wmean, 2));
      if (*Sy >= 0.0) {
        tempY = -tempY;
      }

      for (knt_0 = 0; knt_0 < knt; knt_0++) {
        tempY *= 1.0020841800044864E-292;
      }

      *Sy = tempY;
    } else {
      *Sy = tempY;
    }
  }
}

void CSE_step(void)
{
  real_T X2state[18];
  real_T Y2[18];
  real_T R[9];
  real_T R_0[9];
  real_T Ss[9];
  real_T tempY_0[9];
  real_T C[3];
  real_T K[3];
  real_T s[3];
  real_T tempY[3];
  real_T s_0;
  real_T temp;
  int32_T aoffset;
  int32_T coffset;
  int32_T iAcol;
  int32_T iAcol_0;
  static const real_T Wmean[2] = { 1.0, -0.1736111111111111 };

  static const real_T Wcov[2] = { 0.87666666666666659, -0.1736111111111111 };

  if (CSE_P.Enable1_Value) {
    boolean_T errorCondition;
    boolean_T guard1 = false;
    CSE_M_x(CSE_DW.x, &s_0);
    UKFCorrectorAdditive_getPredict(CSE_P.R1_Value, CSE_DW.x, CSE_DW.P, 0.2, 2.0,
      0.0, &s_0, tempY, &temp);
    s_0 = CSE_U.y - s_0;
    iAcol_0 = 0;
    for (iAcol = 0; iAcol < 3; iAcol++) {
      R[iAcol_0] = CSE_DW.P[iAcol];
      R[iAcol_0 + 1] = CSE_DW.P[iAcol + 3];
      R[iAcol_0 + 2] = CSE_DW.P[iAcol + 6];
      C[iAcol] = tempY[iAcol];
      iAcol_0 += 3;
    }

    trisolve_BC1XmAGq(temp, C);
    K[0] = C[0];
    K[1] = C[1];
    K[2] = C[2];
    trisolve_BC1XmAGq(temp, K);
    tempY[0] = K[0] * temp;
    for (iAcol = 0; iAcol < 2; iAcol++) {
      R[iAcol + 1] = 0.0;
    }

    tempY[1] = K[1] * temp;
    R[5] = 0.0;
    tempY[2] = K[2] * temp;
    errorCondition = (R[0] == 0.0);
    if (!errorCondition) {
      errorCondition = (R[4] == 0.0);
    }

    if (!errorCondition) {
      errorCondition = (R[8] == 0.0);
    }

    guard1 = false;
    if (errorCondition) {
      guard1 = true;
    } else {
      real_T absxk;
      real_T scale;
      real_T t;
      C[2] = tempY[2];
      C[0] = tempY[0] / R[0];
      C[1] = (tempY[1] - C[0] * R[3]) / R[4];
      temp = tempY[2];
      for (iAcol_0 = 0; iAcol_0 < 2; iAcol_0++) {
        temp -= R[iAcol_0 + 6] * C[iAcol_0];
      }

      C[2] = temp / R[8];
      scale = 3.3121686421112381E-170;
      absxk = fabs(C[0]);
      if (absxk > 3.3121686421112381E-170) {
        temp = 1.0;
        scale = absxk;
      } else {
        t = absxk / 3.3121686421112381E-170;
        temp = t * t;
      }

      absxk = fabs(C[1]);
      if (absxk > scale) {
        t = scale / absxk;
        temp = temp * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        temp += t * t;
      }

      absxk = fabs(C[2]);
      if (absxk > scale) {
        t = scale / absxk;
        temp = temp * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        temp += t * t;
      }

      temp = scale * sqrt(temp);
      if (temp >= 1.0) {
        guard1 = true;
      } else {
        rotate_LZ7ctbe0(sqrt(1.0 - temp * temp), C[2], &tempY[2], &s[2], &temp);
        C[2] = 0.0;
        rotate_LZ7ctbe0(temp, C[1], &tempY[1], &s[1], &temp);
        C[1] = 0.0;
        rotate_LZ7ctbe0(temp, C[0], &tempY[0], &s[0], &temp);
        C[0] = 0.0;
        for (iAcol_0 = 0; iAcol_0 + 1 > 0; iAcol_0--) {
          temp = s[iAcol_0] * R[iAcol_0];
          R[iAcol_0] = tempY[iAcol_0] * R[iAcol_0] - s[iAcol_0] * C[0];
          C[0] = tempY[iAcol_0] * C[0] + temp;
        }

        for (iAcol_0 = 1; iAcol_0 + 1 > 0; iAcol_0--) {
          temp = R[iAcol_0 + 3];
          R[iAcol_0 + 3] = temp * tempY[iAcol_0] - s[iAcol_0] * C[1];
          C[1] = tempY[iAcol_0] * C[1] + temp * s[iAcol_0];
        }

        for (iAcol_0 = 2; iAcol_0 + 1 > 0; iAcol_0--) {
          temp = R[iAcol_0 + 6];
          R[iAcol_0 + 6] = temp * tempY[iAcol_0] - s[iAcol_0] * C[2];
          C[2] = tempY[iAcol_0] * C[2] + temp * s[iAcol_0];
        }
      }
    }

    if (guard1) {
      int32_T R_tmp;
      boolean_T exitg2;
      iAcol_0 = 0;
      for (iAcol = 0; iAcol < 3; iAcol++) {
        coffset = 0;
        for (aoffset = 0; aoffset < 3; aoffset++) {
          R_tmp = coffset + iAcol;
          R_0[R_tmp] = 0.0;
          R_0[R_tmp] += R[iAcol_0] * R[coffset];
          R_0[R_tmp] += R[iAcol_0 + 1] * R[coffset + 1];
          R_0[R_tmp] += R[iAcol_0 + 2] * R[coffset + 2];
          tempY_0[aoffset + iAcol_0] = tempY[aoffset] * tempY[iAcol];
          coffset += 3;
        }

        iAcol_0 += 3;
      }

      errorCondition = true;
      for (iAcol = 0; iAcol < 9; iAcol++) {
        temp = R_0[iAcol] - tempY_0[iAcol];
        if (errorCondition && (rtIsInf(temp) || rtIsNaN(temp))) {
          errorCondition = false;
        }

        Ss[iAcol] = temp;
      }

      if (errorCondition) {
        svd_u3DvUgOe(Ss, tempY_0, s, R_0);
      } else {
        s[0] = (rtNaN);
        s[1] = (rtNaN);
        s[2] = (rtNaN);
        for (iAcol_0 = 0; iAcol_0 < 9; iAcol_0++) {
          R_0[iAcol_0] = (rtNaN);
        }
      }

      memset(&Ss[0], 0, 9U * sizeof(real_T));
      Ss[0] = s[0];
      Ss[4] = s[1];
      Ss[8] = s[2];
      for (iAcol_0 = 0; iAcol_0 < 9; iAcol_0++) {
        Ss[iAcol_0] = sqrt(Ss[iAcol_0]);
      }

      iAcol_0 = 0;
      for (iAcol = 0; iAcol < 3; iAcol++) {
        coffset = 0;
        for (aoffset = 0; aoffset < 3; aoffset++) {
          R_tmp = coffset + iAcol;
          R[R_tmp] = 0.0;
          R[R_tmp] += Ss[iAcol_0] * R_0[aoffset];
          R[R_tmp] += Ss[iAcol_0 + 1] * R_0[aoffset + 3];
          R[R_tmp] += Ss[iAcol_0 + 2] * R_0[aoffset + 6];
          coffset += 3;
        }

        iAcol_0 += 3;
      }

      errorCondition = true;
      iAcol = 0;
      exitg2 = false;
      while ((!exitg2) && (iAcol < 3)) {
        int32_T exitg1;
        iAcol_0 = iAcol + 1;
        do {
          exitg1 = 0;
          if (iAcol_0 + 1 < 4) {
            if (!(R[3 * iAcol + iAcol_0] == 0.0)) {
              errorCondition = false;
              exitg1 = 1;
            } else {
              iAcol_0++;
            }
          } else {
            iAcol++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = true;
        }
      }

      if (!errorCondition) {
        memcpy(&R_0[0], &R[0], 9U * sizeof(real_T));
        qr_5a2P8Gh6(R_0, tempY_0, R);
      }
    }

    iAcol_0 = 0;
    for (iAcol = 0; iAcol < 3; iAcol++) {
      CSE_DW.P[iAcol_0] = R[iAcol];
      CSE_DW.P[iAcol_0 + 1] = R[iAcol + 3];
      CSE_DW.P[iAcol_0 + 2] = R[iAcol + 6];
      CSE_DW.x[iAcol] += K[iAcol] * s_0;
      iAcol_0 += 3;
    }
  }

  CSE_Y.soc = CSE_DW.x[1] / CSE_DW.x[2];
  CSE_Y.C = CSE_DW.x[2];
  CSE_S_x(CSE_DW.x, tempY);
  for (iAcol_0 = 0; iAcol_0 < 9; iAcol_0++) {
    s_0 = 0.34641016151377552 * CSE_DW.P[iAcol_0];
    X2state[iAcol_0] = s_0;
    X2state[iAcol_0 + 9] = -s_0;
  }

  iAcol_0 = 0;
  for (iAcol = 0; iAcol < 6; iAcol++) {
    X2state[iAcol_0] += CSE_DW.x[0];
    X2state[iAcol_0 + 1] += CSE_DW.x[1];
    X2state[iAcol_0 + 2] += CSE_DW.x[2];
    iAcol_0 += 3;
  }

  iAcol_0 = 0;
  for (iAcol = 0; iAcol < 6; iAcol++) {
    CSE_S_x(&X2state[iAcol_0], &Y2[iAcol_0]);
    iAcol_0 += 3;
  }

  CSE_S_x(CSE_DW.x, tempY);
  for (iAcol_0 = 0; iAcol_0 < 3; iAcol_0++) {
    C[iAcol_0] = CSE_DW.x[iAcol_0];
  }

  UTMeanCovSqrt_C2vgLQ4W(Wmean, Wcov, -23.999999999999996, tempY, Y2, C, X2state,
    CSE_DW.x, Ss, R);
  memset(&R[0], 0, 9U * sizeof(real_T));
  R[0] = 1.0;
  R[4] = 1.0;
  R[8] = 1.0;
  for (iAcol_0 = 0; iAcol_0 < 3; iAcol_0++) {
    coffset = iAcol_0 * 3;
    for (iAcol = 0; iAcol < 3; iAcol++) {
      aoffset = iAcol * 3;
      R_0[coffset + iAcol] = (Ss[aoffset + 1] * R[iAcol_0 + 3] + Ss[aoffset] *
        R[iAcol_0]) + Ss[aoffset + 2] * R[iAcol_0 + 6];
    }
  }

  iAcol_0 = 0;
  iAcol = 0;
  for (coffset = 0; coffset < 3; coffset++) {
    Y2[iAcol_0] = R_0[iAcol];
    Y2[iAcol_0 + 3] = CSE_P.Q_Value[coffset];
    Y2[iAcol_0 + 1] = R_0[iAcol + 1];
    Y2[iAcol_0 + 4] = CSE_P.Q_Value[coffset + 3];
    Y2[iAcol_0 + 2] = R_0[iAcol + 2];
    Y2[iAcol_0 + 5] = CSE_P.Q_Value[coffset + 6];
    iAcol_0 += 6;
    iAcol += 3;
  }

  qr_DvOBrgQi(Y2, X2state, R);
  iAcol_0 = 0;
  for (iAcol = 0; iAcol < 3; iAcol++) {
    CSE_DW.P[iAcol_0] = R[iAcol];
    CSE_DW.P[iAcol_0 + 1] = R[iAcol + 3];
    CSE_DW.P[iAcol_0 + 2] = R[iAcol + 6];
    iAcol_0 += 3;
  }
}

void CSE_initialize(void)
{
  rt_InitInfAndNaN(sizeof(real_T));
  memcpy(&CSE_DW.P[0], &CSE_P.DataStoreMemoryP_InitialValue[0], 9U * sizeof
         (real_T));
  CSE_DW.x[0] = CSE_P.DataStoreMemoryx_InitialValue[0];
  CSE_DW.x[1] = CSE_P.DataStoreMemoryx_InitialValue[1];
  CSE_DW.x[2] = CSE_P.DataStoreMemoryx_InitialValue[2];
}

void CSE_terminate(void)
{
}
