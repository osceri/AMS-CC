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

for _, data_name, data_size, data_type in functions:
    if data_size < 1:
        #__weak $data_type$* $data_name$();
    elif data_size < 2:
        #__weak $data_type$ $data_name$() {
    else:
        #__weak $data_type$* $data_name$(){
    #   /* Type your actual code somewhere else */
    #}
    #
#__weak void $SM$_error() {
#   /* Type your actual code somewhere else */
#}
#
for index in range(max_depth):
    #static $SM$_state_t state_r$index$;
for index in range(max_depth):
    #static float timer_r$index$;
#
for state in state_machines:
    allcond = ''.join([ conditional for _, _, conditional, _ in state.transitions ])
    uses_timer = state.states or ('before' in allcond) or ('after' in allcond)
    #$SM$_state_t $state.name$_function() {
    if state.states:
        #   if(timer_r$state.depth$ < 0.001*$SM$_inputs.Ts) {
        #       state_r$state.depth+1$ = STATE_$state.name.upper()$_0;
        #   }
        #
        #   switch(state_r$state.depth+1$) {
        for sub_state in state.states:
            #       case STATE_$sub_state.name.upper()$:
            #           state_r$state.depth+1$ = $sub_state.name$_function();
            #           break;

        #       default:
        #           $SM$_error();
        #           break;
        #   }
        #
    for source_state, destination_state, conditional, statements in state.transitions:
        conditional = codec(conditional)
        conditional = regex.sub('before\(([^)]*)\)', f'(timer_r$state.depth$ < \\1)', conditional)
        conditional = regex.sub('after\(([^)]*)\)', f'(timer_r$state.depth$ > \\1)', conditional)

        statements = codec(statements)
        if destination_state != source_state:
            statements += f'timer_r$state.depth$ = 0;'
            statements += f'return STATE_$destination_state.upper()$;'

        if statements:
            #   if($conditional or 1$) {
            for statement in statements.split(';')[:-1]:
                #       $statement$;
            #   }
        #
    #   timer_r$state.depth$ += $SM$_inputs.Ts;
    #   return STATE_$state.name.upper()$;
    #}
    #
