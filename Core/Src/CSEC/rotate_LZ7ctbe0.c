#include "rtwtypes.h"
#include "rotate_LZ7ctbe0.h"
#include <math.h>
#include "rt_hypotd_snf.h"

void rotate_LZ7ctbe0(real_T x, real_T y, real_T *c, real_T *s, real_T *r)
{
  real_T absx;
  real_T absy;
  absx = fabs(x);
  absy = fabs(y);
  if (absy == 0.0) {
    *c = 1.0;
    *s = 0.0;
    *r = x;
  } else if (absx == 0.0) {
    *c = 0.0;
    *s = 1.0;
    *r = y;
  } else {
    real_T rho;
    real_T xx;
    real_T yy;
    absy += absx;
    xx = x / absy;
    yy = y / absy;
    absx = fabs(xx);
    rho = rt_hypotd_snf(absx, fabs(yy));
    *c = absx / rho;
    xx /= absx;
    *s = xx * yy / rho;
    *r = rho * absy * xx;
  }
}
