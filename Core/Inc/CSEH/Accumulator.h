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
  real_T Memory_PreviousInput_b;
  uint32_T method;
  uint32_T state;
  uint32_T state_c[2];
  uint32_T state_g[625];
  boolean_T state_not_empty;
} DW_Accumulator_T;

typedef struct {
  real_T SimCurrent;
  real_T CC_true;
  real_T CC_level;
  real_T CV_true;
  real_T CV_level;
  real_T Balances[126];
  real_T Overtemperature;
  real_T Undertemperature;
} ExtU_Accumulator_T;

typedef struct {
  real_T Voltages[126];
  real_T SOCs[126];
  real_T Capacities[126];
  real_T Current;
  real_T Temperature[60];
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
  real_T cell_count;
  real_T ocv[13];
  real_T soc[13];
  real_T tau[13];
  real_T Memory4_InitialCondition[126];
  real_T Memory3_InitialCondition;
  real_T Gain_Gain;
  real_T Memory1_InitialCondition;
  real_T Constant1_Value;
  real_T Memory_InitialCondition;
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

