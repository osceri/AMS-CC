/*
 * SM.h
 *
 *  Created on: Apr 10, 2022
 *      Author: oscar
 */

#ifndef INC_SM_H_
#define INC_SM_H_

#include "main.h"
#include "programme_states.h"

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
void state_machine_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_error_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_entry_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_idle_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_precharge_drive_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_drive_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_precharge_charge_step(void);

/*
 * @Brief	X
 * @Param	Y
 * @Retval	Z
 */
state_t state_charge_step(void);


#endif /* INC_SM_H_ */
