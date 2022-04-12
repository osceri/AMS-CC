#include "SIM0.h"
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "look1_binlxpw.h"

DW_SIM0_T SIM0_DW;
ExtU_SIM0_T SIM0_U;
ExtY_SIM0_T SIM0_Y;
static RT_MODEL_SIM0_T SIM0_M_;
RT_MODEL_SIM0_T *const SIM0_M = &SIM0_M_;
void SIM0_step(void)
{
  real_T rtb_Memory15[126];
  real_T rtb_Memory21[126];
  real_T rtb_Rpz[126];
  real_T rtb_Rsz[126];
  real_T rtb_Switch[126];
  real_T rtb_Current;
  real_T rtb_Exp;
  real_T rtb_Memory20;
  real_T rtb_Memory22;
  real_T rtb_Product1_n;
  real_T rtb_Product9;
  real_T rtb_current;
  real_T rtb_is_drive;
  real_T rtb_y;
  int32_T i;
  int32_T iU;
  int32_T rtb_is_charge;
  boolean_T rtb_Logic_idx_0;
  boolean_T rtb_Logic_p_idx_0;
  rtb_Memory20 = SIM0_DW.Memory20_PreviousInput;
  rtb_Exp = exp(SIM0_P.Cap_det / (SIM0_P.C_r * 3600.0) *
                SIM0_DW.Memory9_PreviousInput);
  for (i = 0; i < 126; i++) {
    rtb_Memory15[i] = SIM0_DW.Memory15_PreviousInput[i];
    rtb_Memory22 = SIM0_DW.Memory_PreviousInput[i] /
      SIM0_DW.Memory10_PreviousInput[i];
    rtb_Product1_n = look1_binlxpw(rtb_Memory22, SIM0_P.soc, SIM0_P.Rs, 12U);
    rtb_is_drive = look1_binlxpw(rtb_Memory22, SIM0_P.soc, SIM0_P.tau, 12U);
    rtb_Product9 = rtb_Product1_n * rtb_is_drive;
    rtb_Product1_n = (rtb_Product1_n + look1_binlxpw(rtb_Memory22, SIM0_P.soc,
      SIM0_P.Rp, 12U)) * SIM0_P.Ts;
    rtb_Product1_n = (((2.0 * rtb_Product9 + rtb_Product1_n) * rtb_Memory20 +
                       (rtb_Product1_n - 2.0 * rtb_Product9) *
                       SIM0_DW.Memory11_PreviousInput) - (SIM0_P.Ts - 2.0 *
      rtb_is_drive) * SIM0_DW.Memory12_PreviousInput[i]) / (2.0 * rtb_is_drive +
      SIM0_P.Ts);
    rtb_is_drive = rtb_Product1_n + look1_binlxpw(rtb_Memory22, SIM0_P.soc,
      SIM0_P.ocv, 12U);
    SIM0_Y.cell_voltages[i] = rtb_is_drive;
    SIM0_Y.cell_socs[i] = rtb_Memory22;
    SIM0_Y.cell_capacities[i] = SIM0_DW.Memory10_PreviousInput[i];
    rtb_Memory21[i] = SIM0_DW.Memory10_PreviousInput[i] * rtb_Exp;
    rtb_Rsz[i] = rtb_Product1_n;
    rtb_Rpz[i] = rtb_is_drive;
  }

  rtb_Exp = SIM0_P.Ts * rtb_Memory20;
  for (i = 0; i < 126; i++) {
    rtb_Switch[i] = SIM0_DW.Memory21_PreviousInput[i] / SIM0_P.R_short
      * SIM0_DW.Memory19_PreviousInput[i] * SIM0_P.Gain_Gain + (rtb_Exp +
      SIM0_DW.Memory_PreviousInput[i]);
  }

  boolean_T rtb_Logic_idx_1;
  i = (int32_T)(((((uint32_T)(SIM0_DW.Memory1_PreviousInput != 0.0) << 1) +
                  (SIM0_DW.Memory2_PreviousInput != 0.0)) << 1) +
                SIM0_DW.Memory_PreviousInput_m);
  rtb_Logic_idx_0 = SIM0_P.Logic_table[(uint32_T)i];
  rtb_Logic_idx_1 = SIM0_P.Logic_table[i + 8U];
  SIM0_Y.AMS_error_latched = rtb_Logic_idx_0;
  i = (int32_T)(((((uint32_T)(SIM0_DW.Memory3_PreviousInput != 0.0) << 1) +
                  (SIM0_DW.Memory4_PreviousInput != 0.0)) << 1) +
                SIM0_DW.Memory_PreviousInput_c);
  rtb_Logic_p_idx_0 = SIM0_P.Logic_table_p[(uint32_T)i];
  SIM0_Y.IMD_error_latched = rtb_Logic_p_idx_0;
  rtb_Memory22 = (rtb_Logic_idx_1 ? (real_T)SIM0_P.Logic_table_p[i + 8U] : 0.0) *
    SIM0_DW.Memory5_PreviousInput;
  SIM0_Y.SC_probe = rtb_Memory22;
  rtb_Exp = rtb_Memory22 * SIM0_DW.Delay1_DSTATE;
  SIM0_Y.precharge_closed = rtb_Exp;
  rtb_Product1_n = rtb_Memory22 * SIM0_DW.Delay2_DSTATE[0];
  rtb_Memory22 *= SIM0_DW.Delay3_DSTATE[0];
  i = 1;
  rtb_is_charge = 0;
  rtb_is_drive = 0.0;
  if (SIM0_DW.Memory6_PreviousInput != 0.0) {
    i = 2;
    rtb_is_charge = 1;
  } else if (SIM0_DW.Memory7_PreviousInput != 0.0) {
    i = 3;
    rtb_is_drive = 1.0;
  }

  rtb_Product9 = rtb_Memory22 * rtb_Product1_n;
  rtb_is_drive = rtb_is_drive * SIM0_DW.Memory22_PreviousInput * rtb_Product9;
  rtb_y = rtb_Memory15[0];
  for (iU = 0; iU < 125; iU++) {
    rtb_y += rtb_Memory15[iU + 1];
  }

  if (SIM0_DW.Memory16_PreviousInput != 0.0) {
    rtb_Current = SIM0_DW.Memory17_PreviousInput - rtb_y;
    if (rtb_Current < 0.0) {
      rtb_Current = 0.0;
    }

    if (SIM0_DW.Memory18_PreviousInput < rtb_Current) {
      rtb_Current = SIM0_DW.Memory18_PreviousInput;
    }
  } else {
    rtb_Current = 0.0;
  }

  rtb_current = 0.0;
  if (i == 1) {
    rtb_Exp = -1.0;
  } else if (i == 2) {
    if (rtb_Memory22 != 0.0) {
      if ((rtb_Product1_n != 0.0) || (rtb_Exp != 0.0)) {
        rtb_Exp = (rtb_y - SIM0_DW.Memory_PreviousInput_a) +
          SIM0_DW.Memory_PreviousInput_a;
        rtb_current = (real_T)rtb_is_charge * rtb_Current * rtb_Product9;
      } else {
        rtb_Exp = (SIM0_DW.Memory17_PreviousInput -
                   SIM0_DW.Memory_PreviousInput_a) +
          SIM0_DW.Memory_PreviousInput_a;
      }
    } else {
      rtb_Exp = -1.0;
    }
  } else if (rtb_Memory22 != 0.0) {
    if (rtb_Product1_n != 0.0) {
      rtb_Exp = (rtb_y - SIM0_DW.Memory_PreviousInput_a) * 0.8 +
        SIM0_DW.Memory_PreviousInput_a;
      rtb_current = rtb_is_drive - (rtb_y - rtb_Exp) / 0.5;
    } else if (rtb_Exp != 0.0) {
      rtb_Exp = (rtb_y - SIM0_DW.Memory_PreviousInput_a) / 50.0 +
        SIM0_DW.Memory_PreviousInput_a;
      rtb_current = rtb_is_drive - (rtb_y - rtb_Exp) / 100.0;
    } else {
      rtb_Exp = (0.0 - SIM0_DW.Memory_PreviousInput_a) / 5.0 +
        SIM0_DW.Memory_PreviousInput_a;
      rtb_current = -(0.0 - rtb_Exp) / 10.0;
    }
  } else {
    rtb_Exp = -1.0;
  }

  SIM0_Y.vehicle_voltage = rtb_Exp;
  SIM0_Y.current = rtb_current;
  SIM0_Y.accumulator_voltage = rtb_y;
  SIM0_Y.AIR_minus_closed = rtb_Memory22;
  SIM0_Y.AIR_plus_closed = rtb_Product1_n;
  memcpy(&SIM0_Y.cell_temperatures[0], &SIM0_P.cTmp[0], 60U * sizeof(real_T));
  SIM0_Y.IMD_ok = SIM0_P.IMD_ok;
  SIM0_DW.Memory20_PreviousInput = rtb_current;
  SIM0_DW.Memory11_PreviousInput = rtb_Memory20;
  SIM0_DW.Memory9_PreviousInput = fabs(rtb_Memory20 +
    SIM0_DW.Memory9_PreviousInput);
  for (i = 0; i < 126; i++) {
    rtb_Memory22 = rtb_Memory21[i];
    rtb_Memory20 = rtb_Switch[i];
    rtb_is_drive = rtb_Rpz[i];
    SIM0_DW.Memory15_PreviousInput[i] = rtb_is_drive;
    SIM0_DW.Memory12_PreviousInput[i] = rtb_Rsz[i];
    if (rtb_Memory20 > rtb_Memory22) {
      SIM0_DW.Memory_PreviousInput[i] = rtb_Memory22;
    } else if (rtb_Memory20 < SIM0_P.Constant1_Value) {
      SIM0_DW.Memory_PreviousInput[i] = SIM0_P.Constant1_Value;
    } else {
      SIM0_DW.Memory_PreviousInput[i] = rtb_Memory20;
    }

    SIM0_DW.Memory10_PreviousInput[i] = rtb_Memory22;
    SIM0_DW.Memory19_PreviousInput[i] = SIM0_U.BalanceOut[i];
    SIM0_DW.Memory21_PreviousInput[i] = rtb_is_drive;
  }

  SIM0_DW.Memory1_PreviousInput = SIM0_U.AMS_error;
  SIM0_DW.Memory2_PreviousInput = SIM0_U.AMS_error_reset;
  SIM0_DW.Memory_PreviousInput_m = rtb_Logic_idx_0;
  SIM0_DW.Memory3_PreviousInput = SIM0_U.IMD_error;
  SIM0_DW.Memory4_PreviousInput = SIM0_U.IMD_error_reset;
  SIM0_DW.Memory_PreviousInput_c = rtb_Logic_p_idx_0;
  SIM0_DW.Memory5_PreviousInput = SIM0_U.SC;
  SIM0_DW.Delay1_DSTATE = SIM0_DW.Memory14_PreviousInput;
  SIM0_DW.Delay3_DSTATE[0] = SIM0_DW.Delay3_DSTATE[1];
  SIM0_DW.Delay3_DSTATE[1] = SIM0_DW.Memory8_PreviousInput;
  SIM0_DW.Delay2_DSTATE[0] = SIM0_DW.Delay2_DSTATE[1];
  SIM0_DW.Delay2_DSTATE[1] = SIM0_DW.Memory13_PreviousInput;
  SIM0_DW.Memory6_PreviousInput = SIM0_U.charge;
  SIM0_DW.Memory7_PreviousInput = SIM0_U.drive;
  SIM0_DW.Memory22_PreviousInput = SIM0_U.drive_current;
  SIM0_DW.Memory17_PreviousInput = SIM0_U.VoltageLimit;
  SIM0_DW.Memory16_PreviousInput = SIM0_U.EnableCharge;
  SIM0_DW.Memory18_PreviousInput = SIM0_U.CurrentLimit;
  SIM0_DW.Memory_PreviousInput_a = rtb_Exp;
  SIM0_DW.Memory13_PreviousInput = SIM0_U.EnableAirPlus;
  SIM0_DW.Memory14_PreviousInput = SIM0_U.EnablePrecharge;
  SIM0_DW.Memory8_PreviousInput = SIM0_U.EnableAirMinus;
}

void SIM0_initialize(void)
{
  {
    int32_T i;
    SIM0_DW.Memory20_PreviousInput = SIM0_P.Memory20_InitialCondition;
    SIM0_DW.Memory11_PreviousInput = SIM0_P.Memory11_InitialCondition;
    SIM0_DW.Memory9_PreviousInput = SIM0_P.Memory9_InitialCondition;
    for (i = 0; i < 126; i++) {
      SIM0_DW.Memory15_PreviousInput[i] = SIM0_P.Memory15_InitialCondition;
      SIM0_DW.Memory12_PreviousInput[i] = SIM0_P.Memory12_InitialCondition[i];
      SIM0_DW.Memory_PreviousInput[i] = SIM0_P.cQur[i];
      SIM0_DW.Memory10_PreviousInput[i] = SIM0_P.cCap[i];
      SIM0_DW.Memory19_PreviousInput[i] = SIM0_P.Memory19_InitialCondition;
      SIM0_DW.Memory21_PreviousInput[i] = SIM0_P.Memory21_InitialCondition;
    }

    SIM0_DW.Memory1_PreviousInput = SIM0_P.Memory1_InitialCondition;
    SIM0_DW.Memory2_PreviousInput = SIM0_P.Memory2_InitialCondition;
    SIM0_DW.Memory_PreviousInput_m = SIM0_P.SRFlipFlop2_initial_condition;
    SIM0_DW.Memory3_PreviousInput = SIM0_P.Memory3_InitialCondition;
    SIM0_DW.Memory4_PreviousInput = SIM0_P.Memory4_InitialCondition;
    SIM0_DW.Memory_PreviousInput_c = SIM0_P.SRFlipFlop1_initial_condition;
    SIM0_DW.Memory5_PreviousInput = SIM0_P.Memory5_InitialCondition;
    SIM0_DW.Delay1_DSTATE = SIM0_P.Delay1_InitialCondition;
    SIM0_DW.Delay3_DSTATE[0] = SIM0_P.Delay3_InitialCondition;
    SIM0_DW.Delay2_DSTATE[0] = SIM0_P.Delay2_InitialCondition;
    SIM0_DW.Delay3_DSTATE[1] = SIM0_P.Delay3_InitialCondition;
    SIM0_DW.Delay2_DSTATE[1] = SIM0_P.Delay2_InitialCondition;
    SIM0_DW.Memory6_PreviousInput = SIM0_P.Memory6_InitialCondition;
    SIM0_DW.Memory7_PreviousInput = SIM0_P.Memory7_InitialCondition;
    SIM0_DW.Memory22_PreviousInput = SIM0_P.Memory22_InitialCondition;
    SIM0_DW.Memory17_PreviousInput = SIM0_P.Memory17_InitialCondition;
    SIM0_DW.Memory16_PreviousInput = SIM0_P.Memory16_InitialCondition;
    SIM0_DW.Memory18_PreviousInput = SIM0_P.Memory18_InitialCondition;
    SIM0_DW.Memory_PreviousInput_a = SIM0_P.Memory_InitialCondition;
    SIM0_DW.Memory13_PreviousInput = SIM0_P.Memory13_InitialCondition;
    SIM0_DW.Memory14_PreviousInput = SIM0_P.Memory14_InitialCondition;
    SIM0_DW.Memory8_PreviousInput = SIM0_P.Memory8_InitialCondition;
  }
}

void SIM0_terminate(void)
{
}
