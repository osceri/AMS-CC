#/*
# * "canlib_callbacks.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
##ifndef INC_CANLIB_CALLBACKS_H_
##define INC_CANLIB_CALLBACKS_H_
#
##include "canlib_data.h"
#

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Param  A pointer to the message which was received
        # * @Retval None
        # */
        #void $system.name$_$message.name$_rx_callback($message.name$_t* $message.name$);
        #

for system in systems:
    for message in system.tx_messages:
        #/*
        # * @Brief  
        # * @Param  A pointer to a message which should be sent
        # * @Retval Return 1 if the data entered in $message.name$ should be sent
        # */
        #uint8_t $system.name$_$message.name$_tx_callback($message.name$_t* $message.name$);
        #

##endif
