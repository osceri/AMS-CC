#include "Accumulator.h"
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "look1_binlxpw.h"

DW_Accumulator_T Accumulator_DW;
ExtU_Accumulator_T Accumulator_U;
ExtY_Accumulator_T Accumulator_Y;
static RT_MODEL_Accumulator_T Accumulator_M_;
RT_MODEL_Accumulator_T *const Accumulator_M = &Accumulator_M_;
void Accumulator_step(void)
{
  real_T rtb_Divide[126];
  real_T rtb_Rpz[126];
  real_T rtb_Rsz[126];
  real_T rtb_Switch[126];
  real_T rtb_tauz[126];
  real_T rtb_Memory3;
  real_T rtb_tauz_m;
  int32_T i;
  rtb_Memory3 = Accumulator_DW.Memory3_PreviousInput;
  for (i = 0; i < 126; i++) {
    real_T a;
    real_T rtb_Divide_a;
    real_T rtb_Rsz_m;
    rtb_Divide_a = Accumulator_DW.Memory_PreviousInput[i] /
      Accumulator_DW.Memory2_PreviousInput[i];
    rtb_Rsz_m = look1_binlxpw(rtb_Divide_a, Accumulator_P.soc, Accumulator_P.Rs,
      12U);
    rtb_tauz_m = look1_binlxpw(rtb_Divide_a, Accumulator_P.soc,
      Accumulator_P.tau, 12U);
    a = rtb_Rsz_m * rtb_tauz_m;
    rtb_Rsz_m = (rtb_Rsz_m + look1_binlxpw(rtb_Divide_a, Accumulator_P.soc,
      Accumulator_P.Rp, 12U)) * Accumulator_P.Ts_sim;
    rtb_Rsz_m = (((2.0 * a + rtb_Rsz_m) * Accumulator_U.Current + (rtb_Rsz_m -
      2.0 * a) * rtb_Memory3) - (Accumulator_P.Ts_sim - 2.0 * rtb_tauz_m) *
                 Accumulator_DW.Memory4_PreviousInput[i]) / (2.0 * rtb_tauz_m +
      Accumulator_P.Ts_sim);
    rtb_tauz_m = rtb_Rsz_m + look1_binlxpw(rtb_Divide_a, Accumulator_P.soc,
      Accumulator_P.ocv, 12U);
    Accumulator_Y.Voltages[i] = rtb_tauz_m;
    rtb_Divide[i] = rtb_Divide_a;
    rtb_Rsz[i] = rtb_Rsz_m;
    rtb_Rpz[i] = rtb_tauz_m;
  }

  rtb_Memory3 = Accumulator_P.Ts_sim * Accumulator_U.Current;
  for (i = 0; i < 126; i++) {
    rtb_Switch[i] = rtb_Rpz[i] / Accumulator_P.R_short
      * Accumulator_U.Balances[i] * Accumulator_P.Gain_Gain + (rtb_Memory3 +
      Accumulator_DW.Memory_PreviousInput[i]);
  }

  rtb_Memory3 = exp(Accumulator_P.Cap_det / (Accumulator_P.C_r * 3600.0) *
                    Accumulator_DW.Memory1_PreviousInput);
  for (i = 0; i < 126; i++) {
    rtb_tauz[i] = Accumulator_DW.Memory2_PreviousInput[i] * rtb_Memory3;
    Accumulator_Y.SOCs[i] = rtb_Divide[i];
    Accumulator_Y.Capacities[i] = Accumulator_DW.Memory2_PreviousInput[i];
  }

  memcpy(&Accumulator_Y.Temperatures[0], &Accumulator_P.cTmp[0], 60U * sizeof
         (real_T));
  rtb_Memory3 = rtb_Rpz[0];
  for (i = 0; i < 125; i++) {
    rtb_Memory3 += rtb_Rpz[i + 1];
  }

  Accumulator_Y.VoltageSum = rtb_Memory3;
  Accumulator_Y.DisplayCurrent = Accumulator_U.Current;
  Accumulator_DW.Memory3_PreviousInput = Accumulator_U.Current;
  for (i = 0; i < 126; i++) {
    rtb_tauz_m = rtb_tauz[i];
    rtb_Memory3 = rtb_Switch[i];
    Accumulator_DW.Memory4_PreviousInput[i] = rtb_Rsz[i];
    if (rtb_Memory3 > rtb_tauz_m) {
      Accumulator_DW.Memory_PreviousInput[i] = rtb_tauz_m;
    } else if (rtb_Memory3 < Accumulator_P.Constant1_Value) {
      Accumulator_DW.Memory_PreviousInput[i] = Accumulator_P.Constant1_Value;
    } else {
      Accumulator_DW.Memory_PreviousInput[i] = rtb_Memory3;
    }

    Accumulator_DW.Memory2_PreviousInput[i] = rtb_tauz_m;
  }

  Accumulator_DW.Memory1_PreviousInput = fabs(Accumulator_U.Current +
    Accumulator_DW.Memory1_PreviousInput);
}

void Accumulator_initialize(void)
{
  Accumulator_DW.Memory3_PreviousInput = Accumulator_P.Memory3_InitialCondition;
  memcpy(&Accumulator_DW.Memory4_PreviousInput[0],
         &Accumulator_P.Memory4_InitialCondition[0], 126U * sizeof(real_T));
  memcpy(&Accumulator_DW.Memory_PreviousInput[0], &Accumulator_P.cQur[0], 126U *
         sizeof(real_T));
  memcpy(&Accumulator_DW.Memory2_PreviousInput[0], &Accumulator_P.cCap[0], 126U *
         sizeof(real_T));
  Accumulator_DW.Memory1_PreviousInput = Accumulator_P.Memory1_InitialCondition;
}

void Accumulator_terminate(void)
{
}
