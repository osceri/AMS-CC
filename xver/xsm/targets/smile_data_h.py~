#/*
# * "smile_data.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
##ifndef INC_SMILE_DATA_H_
##define INC_SMILE_DATA_H_
#
##include "main.h"
#
##include "smile_data.h"
#

#struct {
for _, data_name, data_size, data_type in inputs:
    if data_size:
        if data_size < 1:
            #   $data_type$* $data_name$;
        elif data_size < 2:
            #   $data_type$ $data_name$;
        else:
            #   $data_type$ $data_name$[$data_size$];
    else:
        #   $data_type$ $data_name$;
#} $SM$_inputs;
#

#struct {
for _, data_name, data_size, data_type in outputs:
    if data_size:
        if data_size < 1:
            #   $data_type$* $data_name$;
        elif data_size < 2:
            #   $data_type$ $data_name$;
        else:
            #   $data_type$ $data_name$[$data_size$];
    else:
        #   $data_type$ $data_name$;
#} $SM$_outputs;
#


##endif
