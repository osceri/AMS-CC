#include "Accumulator.h"
#include "rtwtypes.h"
#include <math.h>
#include <string.h>
#include "look1_binlxpw.h"

DW_Accumulator_T Accumulator_DW;
ExtU_Accumulator_T Accumulator_U;
ExtY_Accumulator_T Accumulator_Y;
static RT_MODEL_Accumulator_T Accumulator_M_;
RT_MODEL_Accumulator_T *const Accumulator_M = &Accumulator_M_;
static real_T Accumulator_rand(void);
static real_T Accumulator_rand(void)
{
  real_T r;
  int32_T i;
  int32_T kk;
  uint32_T u[2];
  switch (Accumulator_DW.method) {
   case 4U:
    {
      uint32_T c_r;
      uint32_T y;
      i = (int32_T)(Accumulator_DW.state / 127773U);
      c_r = (Accumulator_DW.state - i * 127773U) * 16807U;
      y = 2836U * i;
      if (c_r < y) {
        c_r = ~(y - c_r) & 2147483647U;
      } else {
        c_r -= y;
      }

      r = (real_T)c_r * 4.6566128752457969E-10;
      Accumulator_DW.state = c_r;
    }
    break;

   case 5U:
    {
      uint32_T c_r;
      uint32_T y;
      c_r = 69069U * Accumulator_DW.state_c[0] + 1234567U;
      y = Accumulator_DW.state_c[1] << 13 ^ Accumulator_DW.state_c[1];
      y ^= y >> 17;
      y ^= y << 5;
      Accumulator_DW.state_c[0] = c_r;
      Accumulator_DW.state_c[1] = y;
      r = (real_T)(c_r + y) * 2.328306436538696E-10;
    }
    break;

   default:
    {
      uint32_T c_r;
      if (!Accumulator_DW.state_not_empty) {
        memset(&Accumulator_DW.state_g[0], 0, 625U * sizeof(uint32_T));
        c_r = 5489U;
        Accumulator_DW.state_g[0] = 5489U;
        for (i = 0; i < 623; i++) {
          c_r = ((c_r >> 30U ^ c_r) * 1812433253U + i) + 1U;
          Accumulator_DW.state_g[i + 1] = c_r;
        }

        Accumulator_DW.state_g[624] = 624U;
        Accumulator_DW.state_not_empty = true;
      }

      /* ========================= COPYRIGHT NOTICE ============================ */
      /*  This is a uniform (0,1) pseudorandom number generator based on:        */
      /*                                                                         */
      /*  A C-program for MT19937, with initialization improved 2002/1/26.       */
      /*  Coded by Takuji Nishimura and Makoto Matsumoto.                        */
      /*                                                                         */
      /*  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,      */
      /*  All rights reserved.                                                   */
      /*                                                                         */
      /*  Redistribution and use in source and binary forms, with or without     */
      /*  modification, are permitted provided that the following conditions     */
      /*  are met:                                                               */
      /*                                                                         */
      /*    1. Redistributions of source code must retain the above copyright    */
      /*       notice, this list of conditions and the following disclaimer.     */
      /*                                                                         */
      /*    2. Redistributions in binary form must reproduce the above copyright */
      /*       notice, this list of conditions and the following disclaimer      */
      /*       in the documentation and/or other materials provided with the     */
      /*       distribution.                                                     */
      /*                                                                         */
      /*    3. The names of its contributors may not be used to endorse or       */
      /*       promote products derived from this software without specific      */
      /*       prior written permission.                                         */
      /*                                                                         */
      /*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS    */
      /*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      */
      /*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  */
      /*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT  */
      /*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  */
      /*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT       */
      /*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  */
      /*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  */
      /*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT    */
      /*  (INCLUDING  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
      /*  OF THIS  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */
      /*                                                                         */
      /* =============================   END   ================================= */
      int32_T exitg1;
      do {
        exitg1 = 0;
        for (i = 0; i < 2; i++) {
          uint32_T y;
          c_r = Accumulator_DW.state_g[624] + 1U;
          if (Accumulator_DW.state_g[624] + 1U >= 625U) {
            for (kk = 0; kk < 227; kk++) {
              c_r = (Accumulator_DW.state_g[kk + 1] & 2147483647U) |
                (Accumulator_DW.state_g[kk] & 2147483648U);
              if ((c_r & 1U) == 0U) {
                c_r >>= 1U;
              } else {
                c_r = c_r >> 1U ^ 2567483615U;
              }

              Accumulator_DW.state_g[kk] = Accumulator_DW.state_g[kk + 397] ^
                c_r;
            }

            for (kk = 0; kk < 396; kk++) {
              c_r = (Accumulator_DW.state_g[kk + 227] & 2147483648U) |
                (Accumulator_DW.state_g[kk + 228] & 2147483647U);
              if ((c_r & 1U) == 0U) {
                c_r >>= 1U;
              } else {
                c_r = c_r >> 1U ^ 2567483615U;
              }

              Accumulator_DW.state_g[kk + 227] = Accumulator_DW.state_g[kk] ^
                c_r;
            }

            c_r = (Accumulator_DW.state_g[623] & 2147483648U) |
              (Accumulator_DW.state_g[0] & 2147483647U);
            if ((c_r & 1U) == 0U) {
              c_r >>= 1U;
            } else {
              c_r = c_r >> 1U ^ 2567483615U;
            }

            Accumulator_DW.state_g[623] = Accumulator_DW.state_g[396] ^ c_r;
            c_r = 1U;
          }

          y = Accumulator_DW.state_g[(int32_T)c_r - 1];
          Accumulator_DW.state_g[624] = c_r;
          y ^= y >> 11U;
          y ^= y << 7U & 2636928640U;
          y ^= y << 15U & 4022730752U;
          u[i] = y >> 18U ^ y;
        }

        r = ((real_T)(u[0] >> 5U) * 6.7108864E+7 + (real_T)(u[1] >> 6U)) *
          1.1102230246251565E-16;
        if (r == 0.0) {
          boolean_T b_isvalid;
          b_isvalid = ((Accumulator_DW.state_g[624] >= 1U) &&
                       (Accumulator_DW.state_g[624] < 625U));
          if (b_isvalid) {
            boolean_T exitg2;
            b_isvalid = false;
            i = 1;
            exitg2 = false;
            while ((!exitg2) && (i < 625)) {
              if (Accumulator_DW.state_g[i - 1] == 0U) {
                i++;
              } else {
                b_isvalid = true;
                exitg2 = true;
              }
            }
          }

          if (!b_isvalid) {
            c_r = 5489U;
            Accumulator_DW.state_g[0] = 5489U;
            for (i = 0; i < 623; i++) {
              c_r = ((c_r >> 30U ^ c_r) * 1812433253U + i) + 1U;
              Accumulator_DW.state_g[i + 1] = c_r;
            }

            Accumulator_DW.state_g[624] = 624U;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
    break;
  }

  return r;
}

void Accumulator_step(void)
{
  real_T rtb_Divide[126];
  real_T rtb_Rpz[126];
  real_T rtb_Rsz[126];
  real_T rtb_Switch[126];
  real_T rtb_Divide_a;
  real_T rtb_Memory3;
  real_T rtb_tauz;
  int32_T i;
  rtb_Memory3 = Accumulator_DW.Memory3_PreviousInput;
  for (i = 0; i < 126; i++) {
    real_T a;
    real_T rtb_Rsz_m;
    rtb_Divide_a = Accumulator_DW.Memory_PreviousInput[i] /
      Accumulator_DW.Memory2_PreviousInput[i];
    rtb_Rsz_m = look1_binlxpw(rtb_Divide_a, Accumulator_P.soc, Accumulator_P.Rs,
      12U);
    rtb_tauz = look1_binlxpw(rtb_Divide_a, Accumulator_P.soc, Accumulator_P.tau,
      12U);
    a = rtb_Rsz_m * rtb_tauz;
    rtb_Rsz_m = (rtb_Rsz_m + look1_binlxpw(rtb_Divide_a, Accumulator_P.soc,
      Accumulator_P.Rp, 12U)) * Accumulator_P.Ts_sim;
    rtb_Rsz_m = (((2.0 * a + rtb_Rsz_m) * Accumulator_U.SimCurrent + (rtb_Rsz_m
      - 2.0 * a) * rtb_Memory3) - (Accumulator_P.Ts_sim - 2.0 * rtb_tauz) *
                 Accumulator_DW.Memory4_PreviousInput[i]) / (2.0 * rtb_tauz +
      Accumulator_P.Ts_sim);
    rtb_tauz = rtb_Rsz_m + look1_binlxpw(rtb_Divide_a, Accumulator_P.soc,
      Accumulator_P.ocv, 12U);
    Accumulator_Y.Voltages[i] = rtb_tauz;
    rtb_Divide[i] = rtb_Divide_a;
    rtb_Rsz[i] = rtb_Rsz_m;
    rtb_Switch[i] = rtb_tauz;
  }

  rtb_Memory3 = rtb_Switch[0];
  for (i = 0; i < 125; i++) {
    rtb_Memory3 += rtb_Switch[i + 1];
  }

  rtb_Memory3 = ((Accumulator_P.cell_count * 0.0034726631697176778 *
                  Accumulator_U.CV_true * (Accumulator_U.CV_level - rtb_Memory3)
                  + Accumulator_U.CC_true * Accumulator_U.CC_level) +
                 Accumulator_U.SimCurrent) * Accumulator_P.Ts_sim;
  for (i = 0; i < 126; i++) {
    rtb_Switch[i] = rtb_Switch[i] / Accumulator_P.R_short
      * Accumulator_U.Balances[i] * Accumulator_P.Gain_Gain + (rtb_Memory3 +
      Accumulator_DW.Memory_PreviousInput[i]);
  }

  rtb_Memory3 = exp(Accumulator_P.Cap_det / (Accumulator_P.C_r * 3600.0) *
                    Accumulator_DW.Memory1_PreviousInput);
  for (i = 0; i < 126; i++) {
    rtb_Rpz[i] = Accumulator_DW.Memory2_PreviousInput[i] * rtb_Memory3;
    Accumulator_Y.SOCs[i] = rtb_Divide[i];
    Accumulator_Y.Capacities[i] = Accumulator_DW.Memory2_PreviousInput[i];
  }

  memcpy(&Accumulator_Y.Temperature[0], &Accumulator_P.cTmp[0], 60U * sizeof
         (real_T));
  if (Accumulator_U.Overtemperature != 0.0) {
    Accumulator_Y.Temperature[(int32_T)Accumulator_DW.Memory_PreviousInput_b - 1]
      = Accumulator_P.cTmp[(int32_T)Accumulator_DW.Memory_PreviousInput_b - 1] +
      45.0;
    rtb_Memory3 = Accumulator_DW.Memory_PreviousInput_b;
  } else if (Accumulator_U.Undertemperature != 0.0) {
    Accumulator_Y.Temperature[(int32_T)Accumulator_DW.Memory_PreviousInput_b - 1]
      = Accumulator_P.cTmp[(int32_T)Accumulator_DW.Memory_PreviousInput_b - 1] -
      45.0;
    rtb_Memory3 = Accumulator_DW.Memory_PreviousInput_b;
  } else {
    rtb_Memory3 = Accumulator_rand();
    rtb_Memory3 = floor(rtb_Memory3 * 60.0) + 1.0;
  }

  Accumulator_Y.Current = Accumulator_U.SimCurrent;
  Accumulator_DW.Memory3_PreviousInput = Accumulator_U.SimCurrent;
  for (i = 0; i < 126; i++) {
    rtb_Divide_a = rtb_Rpz[i];
    rtb_tauz = rtb_Switch[i];
    Accumulator_DW.Memory4_PreviousInput[i] = rtb_Rsz[i];
    if (rtb_tauz > rtb_Divide_a) {
      Accumulator_DW.Memory_PreviousInput[i] = rtb_Divide_a;
    } else if (rtb_tauz < Accumulator_P.Constant1_Value) {
      Accumulator_DW.Memory_PreviousInput[i] = Accumulator_P.Constant1_Value;
    } else {
      Accumulator_DW.Memory_PreviousInput[i] = rtb_tauz;
    }

    Accumulator_DW.Memory2_PreviousInput[i] = rtb_Divide_a;
  }

  Accumulator_DW.Memory1_PreviousInput = fabs(Accumulator_U.SimCurrent +
    Accumulator_DW.Memory1_PreviousInput);
  Accumulator_DW.Memory_PreviousInput_b = rtb_Memory3;
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
  Accumulator_DW.Memory_PreviousInput_b = Accumulator_P.Memory_InitialCondition;
  Accumulator_DW.method = 7U;
  Accumulator_DW.state = 1144108930U;
  Accumulator_DW.state_c[0] = 362436069U;
  Accumulator_DW.state_c[1] = 521288629U;
}

void Accumulator_terminate(void)
{
}
