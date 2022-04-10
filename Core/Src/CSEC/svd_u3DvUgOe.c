#include "rtwtypes.h"
#include "svd_u3DvUgOe.h"
#include "xnrm2_xDOlDEtw.h"
#include <math.h>
#include "xscal_59GtRAwv.h"
#include "xnrm2_Rlk0oxWw.h"
#include "xdotc_mznw4aLl.h"
#include "xaxpy_guu0hJif.h"
#include "xscal_PLmmUlD2.h"
#include "xaxpy_OQ28XA21.h"
#include "xaxpy_Wxch84Fl.h"
#include "xscal_eaAxXsvQ.h"
#include "xrotg_XzIjz1eG.h"
#include "xrot_i7f01JoJ.h"
#include "xswap_JoAqPjUv.h"

void svd_u3DvUgOe(const real_T A[9], real_T U[9], real_T s[3], real_T V[9])
{
  real_T b_A[9];
  real_T b_s[3];
  real_T e[3];
  real_T work[3];
  real_T nrm;
  real_T rt;
  real_T smm1;
  real_T sqds;
  real_T ztest;
  int32_T d;
  int32_T kase;
  int32_T m;
  int32_T qjj;
  int32_T qq;
  boolean_T apply_transform;
  e[0] = 0.0;
  work[0] = 0.0;
  e[1] = 0.0;
  work[1] = 0.0;
  e[2] = 0.0;
  work[2] = 0.0;
  for (qq = 0; qq < 9; qq++) {
    b_A[qq] = A[qq];
    U[qq] = 0.0;
    V[qq] = 0.0;
  }

  qq = 0;
  apply_transform = false;
  nrm = xnrm2_xDOlDEtw(3, b_A, 1);
  if (nrm > 0.0) {
    apply_transform = true;
    if (b_A[0] < 0.0) {
      b_s[0] = -nrm;
    } else {
      b_s[0] = nrm;
    }

    if (fabs(b_s[0]) >= 1.0020841800044864E-292) {
      xscal_59GtRAwv(3, 1.0 / b_s[0], b_A, 1);
    } else {
      for (qjj = 0; qjj < 3; qjj++) {
        b_A[qjj] /= b_s[0];
      }
    }

    b_A[0]++;
    b_s[0] = -b_s[0];
  } else {
    b_s[0] = 0.0;
  }

  for (d = 1; d + 1 < 4; d++) {
    qjj = 3 * d;
    if (apply_transform) {
      xaxpy_guu0hJif(3, -(xdotc_mznw4aLl(3, b_A, 1, b_A, qjj + 1) / b_A[0]), 1,
                     b_A, qjj + 1);
    }

    e[d] = b_A[qjj];
  }

  while (qq + 1 < 4) {
    U[qq] = b_A[qq];
    qq++;
  }

  nrm = xnrm2_Rlk0oxWw(2, e, 2);
  if (nrm == 0.0) {
    e[0] = 0.0;
  } else {
    if (e[1] < 0.0) {
      e[0] = -nrm;
    } else {
      e[0] = nrm;
    }

    nrm = e[0];
    if (fabs(e[0]) >= 1.0020841800044864E-292) {
      xscal_PLmmUlD2(2, 1.0 / e[0], e, 2);
    } else {
      for (qq = 1; qq < 3; qq++) {
        e[qq] /= nrm;
      }
    }

    e[1]++;
    e[0] = -e[0];
    for (qq = 2; qq < 4; qq++) {
      work[qq - 1] = 0.0;
    }

    for (qq = 1; qq + 1 < 4; qq++) {
      xaxpy_OQ28XA21(2, e[qq], b_A, 3 * qq + 2, work, 2);
    }

    for (qq = 1; qq + 1 < 4; qq++) {
      xaxpy_Wxch84Fl(2, -e[qq] / e[1], work, 2, b_A, 3 * qq + 2);
    }
  }

  for (qq = 1; qq + 1 < 4; qq++) {
    V[qq] = e[qq];
  }

  apply_transform = false;
  nrm = xnrm2_xDOlDEtw(2, b_A, 5);
  if (nrm > 0.0) {
    apply_transform = true;
    if (b_A[4] < 0.0) {
      b_s[1] = -nrm;
    } else {
      b_s[1] = nrm;
    }

    if (fabs(b_s[1]) >= 1.0020841800044864E-292) {
      xscal_59GtRAwv(2, 1.0 / b_s[1], b_A, 5);
    } else {
      for (qjj = 4; qjj < 6; qjj++) {
        b_A[qjj] /= b_s[1];
      }
    }

    b_A[4]++;
    b_s[1] = -b_s[1];
  } else {
    b_s[1] = 0.0;
  }

  for (d = 2; d + 1 < 4; d++) {
    qjj = 3 * d + 1;
    if (apply_transform) {
      xaxpy_guu0hJif(2, -(xdotc_mznw4aLl(2, b_A, 5, b_A, qjj + 1) / b_A[4]), 5,
                     b_A, qjj + 1);
    }

    e[d] = b_A[qjj];
  }

  for (qq = 1; qq + 1 < 4; qq++) {
    U[qq + 3] = b_A[qq + 3];
  }

  m = 1;
  b_s[2] = b_A[8];
  U[6] = 0.0;
  U[7] = 0.0;
  U[8] = 1.0;
  for (d = 1; d >= 0; d--) {
    qq = 3 * d + d;
    if (b_s[d] != 0.0) {
      for (kase = d + 1; kase + 1 < 4; kase++) {
        qjj = (3 * kase + d) + 1;
        xaxpy_guu0hJif(3 - d, -(xdotc_mznw4aLl(3 - d, U, qq + 1, U, qjj) / U[qq]),
                       qq + 1, U, qjj);
      }

      for (qjj = d; qjj + 1 < 4; qjj++) {
        kase = 3 * d + qjj;
        U[kase] = -U[kase];
      }

      U[qq]++;
      if (d - 1 >= 0) {
        U[3 * d] = 0.0;
      }
    } else {
      U[3 * d] = 0.0;
      U[3 * d + 1] = 0.0;
      U[3 * d + 2] = 0.0;
      U[qq] = 1.0;
    }
  }

  for (qq = 2; qq >= 0; qq--) {
    if ((qq + 1 <= 1) && (e[0] != 0.0)) {
      for (d = 2; d < 4; d++) {
        qjj = (d - 1) * 3 + 2;
        xaxpy_guu0hJif(2, -(xdotc_mznw4aLl(2, V, 2, V, qjj) / V[1]), 2, V, qjj);
      }
    }

    V[3 * qq] = 0.0;
    V[3 * qq + 1] = 0.0;
    V[3 * qq + 2] = 0.0;
    V[qq + 3 * qq] = 1.0;
  }

  ztest = e[0];
  if (b_s[0] != 0.0) {
    rt = fabs(b_s[0]);
    nrm = b_s[0] / rt;
    b_s[0] = rt;
    ztest = e[0] / nrm;
    xscal_eaAxXsvQ(nrm, U, 1);
  }

  if (ztest != 0.0) {
    rt = fabs(ztest);
    nrm = rt / ztest;
    ztest = rt;
    b_s[1] *= nrm;
    xscal_eaAxXsvQ(nrm, V, 4);
  }

  e[0] = ztest;
  ztest = b_A[7];
  if (b_s[1] != 0.0) {
    rt = fabs(b_s[1]);
    nrm = b_s[1] / rt;
    b_s[1] = rt;
    ztest = b_A[7] / nrm;
    xscal_eaAxXsvQ(nrm, U, 4);
  }

  if (ztest != 0.0) {
    rt = fabs(ztest);
    nrm = rt / ztest;
    ztest = rt;
    b_s[2] = b_A[8] * nrm;
    xscal_eaAxXsvQ(nrm, V, 7);
  }

  e[1] = ztest;
  if (b_s[2] != 0.0) {
    rt = fabs(b_s[2]);
    nrm = b_s[2] / rt;
    b_s[2] = rt;
    xscal_eaAxXsvQ(nrm, U, 7);
  }

  e[2] = 0.0;
  qq = 0;
  nrm = fmax(fmax(fmax(b_s[0], e[0]), fmax(b_s[1], ztest)), fmax(b_s[2], 0.0));
  while ((m + 2 > 0) && (qq < 75)) {
    kase = m + 1;
    int32_T exitg1;
    do {
      exitg1 = 0;
      d = kase;
      if (kase == 0) {
        exitg1 = 1;
      } else {
        rt = fabs(e[kase - 1]);
        if (rt <= (fabs(b_s[kase - 1]) + fabs(b_s[kase])) *
            2.2204460492503131E-16) {
          e[kase - 1] = 0.0;
          exitg1 = 1;
        } else if ((rt <= 1.0020841800044864E-292) || ((qq > 20) && (rt <=
                     2.2204460492503131E-16 * nrm))) {
          e[kase - 1] = 0.0;
          exitg1 = 1;
        } else {
          kase--;
        }
      }
    } while (exitg1 == 0);

    if (m + 1 == kase) {
      kase = 4;
    } else {
      int32_T k_ii;
      boolean_T exitg2;
      qjj = m + 2;
      k_ii = m + 2;
      exitg2 = false;
      while ((!exitg2) && (k_ii >= kase)) {
        qjj = k_ii;
        if (k_ii == kase) {
          exitg2 = true;
        } else {
          rt = 0.0;
          if (k_ii < m + 2) {
            rt = fabs(e[k_ii - 1]);
          }

          if (k_ii > kase + 1) {
            rt += fabs(e[k_ii - 2]);
          }

          ztest = fabs(b_s[k_ii - 1]);
          if ((ztest <= 2.2204460492503131E-16 * rt) || (ztest <=
               1.0020841800044864E-292)) {
            b_s[k_ii - 1] = 0.0;
            exitg2 = true;
          } else {
            k_ii--;
          }
        }
      }

      if (qjj == kase) {
        kase = 3;
      } else if (m + 2 == qjj) {
        kase = 1;
      } else {
        kase = 2;
        d = qjj;
      }
    }

    switch (kase) {
     case 1:
      rt = e[m];
      e[m] = 0.0;
      for (qjj = m; qjj + 1 >= d + 1; qjj--) {
        xrotg_XzIjz1eG(&b_s[qjj], &rt, &ztest, &sqds);
        if (qjj + 1 > d + 1) {
          rt = -sqds * e[0];
          e[0] *= ztest;
        }

        xrot_i7f01JoJ(V, 3 * qjj + 1, 3 * (m + 1) + 1, ztest, sqds);
      }
      break;

     case 2:
      rt = e[d - 1];
      e[d - 1] = 0.0;
      for (qjj = d; qjj < m + 2; qjj++) {
        xrotg_XzIjz1eG(&b_s[qjj], &rt, &ztest, &sqds);
        rt = -sqds * e[qjj];
        e[qjj] *= ztest;
        xrot_i7f01JoJ(U, 3 * qjj + 1, 3 * (d - 1) + 1, ztest, sqds);
      }
      break;

     case 3:
      {
        real_T emm1;
        real_T shift;
        sqds = b_s[m + 1];
        ztest = fmax(fmax(fmax(fmax(fabs(sqds), fabs(b_s[m])), fabs(e[m])), fabs
                          (b_s[d])), fabs(e[d]));
        rt = sqds / ztest;
        smm1 = b_s[m] / ztest;
        emm1 = e[m] / ztest;
        sqds = b_s[d] / ztest;
        smm1 = ((smm1 + rt) * (smm1 - rt) + emm1 * emm1) / 2.0;
        emm1 *= rt;
        emm1 *= emm1;
        if ((smm1 != 0.0) || (emm1 != 0.0)) {
          shift = sqrt(smm1 * smm1 + emm1);
          if (smm1 < 0.0) {
            shift = -shift;
          }

          shift = emm1 / (smm1 + shift);
        } else {
          shift = 0.0;
        }

        rt = (sqds + rt) * (sqds - rt) + shift;
        ztest = e[d] / ztest * sqds;
        for (qjj = d + 1; qjj <= m + 1; qjj++) {
          xrotg_XzIjz1eG(&rt, &ztest, &sqds, &smm1);
          if (qjj > d + 1) {
            e[0] = rt;
          }

          rt = e[qjj - 1];
          emm1 = b_s[qjj - 1];
          e[qjj - 1] = rt * sqds - emm1 * smm1;
          ztest = smm1 * b_s[qjj];
          b_s[qjj] *= sqds;
          xrot_i7f01JoJ(V, 3 * (qjj - 1) + 1, 3 * qjj + 1, sqds, smm1);
          b_s[qjj - 1] = emm1 * sqds + rt * smm1;
          xrotg_XzIjz1eG(&b_s[qjj - 1], &ztest, &sqds, &smm1);
          rt = e[qjj - 1] * sqds + smm1 * b_s[qjj];
          b_s[qjj] = e[qjj - 1] * -smm1 + sqds * b_s[qjj];
          ztest = smm1 * e[qjj];
          e[qjj] *= sqds;
          xrot_i7f01JoJ(U, 3 * (qjj - 1) + 1, 3 * qjj + 1, sqds, smm1);
        }

        e[m] = rt;
        qq++;
      }
      break;

     default:
      if (b_s[d] < 0.0) {
        b_s[d] = -b_s[d];
        xscal_eaAxXsvQ(-1.0, V, 3 * d + 1);
      }

      qq = d + 1;
      while ((d + 1 < 3) && (b_s[d] < b_s[qq])) {
        rt = b_s[d];
        b_s[d] = b_s[qq];
        b_s[qq] = rt;
        xswap_JoAqPjUv(V, 3 * d + 1, 3 * (d + 1) + 1);
        xswap_JoAqPjUv(U, 3 * d + 1, 3 * (d + 1) + 1);
        d = qq;
        qq++;
      }

      qq = 0;
      m--;
      break;
    }
  }

  s[0] = b_s[0];
  s[1] = b_s[1];
  s[2] = b_s[2];
}
