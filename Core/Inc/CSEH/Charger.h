#ifndef RTW_HEADER_Charger_h_
#define RTW_HEADER_Charger_h_
#ifndef Charger_COMMON_INCLUDES_
#define Charger_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif

#include "Charger_types.h"

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
  real_T CC_true;
  real_T CC_level;
  real_T CV_true;
  real_T CV_level;
  real_T y;
} ExtU_Charger_T;

typedef struct {
  real_T Current;
} ExtY_Charger_T;

struct P_Charger_T_ {
  real_T cell_count;
};

struct tag_RTM_Charger_T {
  const char_T * volatile errorStatus;
};

extern P_Charger_T Charger_P;
extern ExtU_Charger_T Charger_U;
extern ExtY_Charger_T Charger_Y;
extern void Charger_initialize(void);
extern void Charger_step(void);
extern void Charger_terminate(void);
extern RT_MODEL_Charger_T *const Charger_M;

#endif

