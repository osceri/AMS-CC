#/*
# * "smile.c"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
#
##include "main.h"
#
##include "smile_callbacks.h"
##include "smile_data.h"
##include "smile.h"
#
#

#uint8_t after(uint32_t* timer, float boundary) {
#   return !(timer[0]*$SM$_parameters.Ts_f32 < boundary);
#}
#
#uint8_t before(uint32_t* timer, float boundary) {
#   return (timer[0]*$SM$_parameters.Ts_f32 < boundary);
#}
#

for data_name, data_size, data_type in functions:
    if data_size < 1:
        #__weak $data_type$* $data_name$();
    elif data_size < 2:
        #__weak $data_type$ $data_name$() {
    else:
        #__weak $data_type$* $data_name$(){
    #   /* Type your actual code somewhere else */
    #}
        

for state in state_machines:
    allcond = ''.join([ conditional for _, _, conditional, _ in state.transitions ])
    uses_timer = state.states or ('before' in allcond) or ('after' in allcond)
    if state.name == SM:
        #void $state.name$() {
        #   static $state.name$_t state = $state.name.upper()$_0_STATE;
        #   static uint32_t timer = 0;
        #   timer++;
    else:
        #void $state.name$(uint32_t* super_timer, uint8_t* super_state) {
        if state.states:
            #   static $state.name$_t state = $state.name.upper()$_0_STATE;
        if uses_timer:
            #   static uint32_t timer = 0;
            #   timer++;
            #   if (super_timer[0] == 0) {
            #       timer = 0;
            if state.states:
                #       state = $state.name.upper()$_0_STATE;
            #   }
            #
    if state.states:
        #   switch(state) {
        for sub_state in state.states:
            #       case $sub_state.name.upper()$_STATE:
            #           $sub_state.name$(&timer, &state);
            #           break;
        #   }
        #

    for source_state, destination_state, conditional, statements in state.transitions:
        conditional = codec(conditional)
        conditional.replace('before(', 'before(&timer,').replace('after(', 'after(&timer,')
        conditional = regex.sub('(before|after)\(([^)]*)\)', '\\1(&timer,\\2)', conditional)
        statements = codec(statements)

        if source_state == destination_state and not statements:
            continue

        if conditional or statements:
            #   if($conditional or 1$) {
            for statement in statements.split(';')[:-1]:
                #       $statement$;
            if source_state != destination_state:
                #       super_timer[0] = 0;
                #       super_state[0] = $destination_state.upper()$_STATE;
            #   }
    
    #
    #}
    
