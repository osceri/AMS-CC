#include "Charger.h"

ExtU_Charger_T Charger_U;
ExtY_Charger_T Charger_Y;
static RT_MODEL_Charger_T Charger_M_;
RT_MODEL_Charger_T *const Charger_M = &Charger_M_;
void Charger_step(void)
{
  Charger_Y.Current = Charger_P.cell_count * 0.0034726631697176778 *
    Charger_U.CV_true * (Charger_U.CV_level - Charger_U.y) + Charger_U.CC_true *
    Charger_U.CC_level;
}

void Charger_initialize(void)
{
}

void Charger_terminate(void)
{
}
