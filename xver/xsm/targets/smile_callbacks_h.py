#/*
# * "smile_callbacks.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
##ifndef INC_SMILE_CALLBACKS_H_
##define INC_SMILE_CALLBACKS_H_
#
##include "smile_data.h"
#

for _, data_name, data_size, data_type in functions:
    if data_size < 1:
        #$data_type$* $data_name$();
    elif data_size < 2:
        #$data_type$ $data_name$();
    else:
        #$data_type$* $data_name$();

##endif
