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


#typedef enum {
#   STATE_NONE = -1;
for state in state_machines:
    #   STATE_$state.name.upper()$ = $state.id$,
#} $SM$_state_t;
#

for state in state_machines:
    #void $state.name$_function();

##endif
