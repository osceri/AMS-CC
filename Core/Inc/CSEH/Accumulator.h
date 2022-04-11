#ifndef RTW_HEADER_Accumulator_h_
#define RTW_HEADER_Accumulator_h_
#ifndef Accumulator_COMMON_INCLUDES_
#define Accumulator_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif

#include "Accumulator_types.h"

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
  real_T Memory4_PreviousInput[126];
  real_T Memory3_PreviousInput;
  real_T Memory_PreviousInput[126];
  real_T Memory2_PreviousInput[126];
  real_T Memory1_PreviousInput;
} DW_Accumulator_T;

typedef struct {
  real_T Current;
  real_T Balances[126];
} ExtU_Accumulator_T;

typedef struct {
  real_T VoltageSum;
  real_T Voltages[126];
  real_T SOCs[126];
  real_T Capacities[126];
  real_T DisplayCurrent;
  real_T Temperatures[60];
} ExtY_Accumulator_T;

struct P_Accumulator_T_ {
  real_T C_r;
  real_T Cap_det;
  real_T R_short;
  real_T Rp[13];
  real_T Rs[13];
  real_T Ts_sim;
  real_T cCap[126];
  real_T cQur[126];
  real_T cTmp[60];
  real_T ocv[13];
  real_T soc[13];
  real_T tau[13];
  real_T Memory4_InitialCondition[126];
  real_T Memory3_InitialCondition;
  real_T Gain_Gain;
  real_T Memory1_InitialCondition;
  real_T Constant1_Value;
};

struct tag_RTM_Accumulator_T {
  const char_T * volatile errorStatus;
};

extern P_Accumulator_T Accumulator_P;
extern DW_Accumulator_T Accumulator_DW;
extern ExtU_Accumulator_T Accumulator_U;
extern ExtY_Accumulator_T Accumulator_Y;
extern void Accumulator_initialize(void);
extern void Accumulator_step(void);
extern void Accumulator_terminate(void);
extern RT_MODEL_Accumulator_T *const Accumulator_M;

#endif

