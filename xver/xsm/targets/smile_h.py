##include "main.h"
#/*
# * "smile.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
##ifndef INC_SMILE_H_
##define INC_SMILE_H_
#
#

first = True
for state in state_machines:
    if state.states:
        #typedef enum {
        for substate in state.states:
            #  $substate.name.upper()$_STATE = $substate.id$,
        #} $state.name$_t;
        #
#

#uint8_t after(uint32_t* timer, float boundary);
#
#uint8_t before(uint32_t* timer, float boundary);
#

for state in state_machines:
    if state.name == SM:
        #void $state.name$();
    else:
        #void $state.name$(uint32_t* super_timer, uint8_t* super_state);

##endif
