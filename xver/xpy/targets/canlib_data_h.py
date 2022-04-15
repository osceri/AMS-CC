#/*
# * "canlib_data.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
#
##ifndef INC_CANLIB_DATA_H_
##define INC_CANLIB_DATA_H_
#

for system in systems:
    for message in system.rx_messages:
        #typedef struct {
        for signal in message.signals:
            #   $signal.type$ $signal.name$;
        #} $message.name$_t;
        #
    #

for system in systems:
    for message in system.tx_messages:
        #typedef struct {
        for signal in message.signals:
            #   $signal.type$ $signal.name$;
        #} $message.name$_t;
        #
    #

##endif
