#/*
# * "canlib.h"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
##ifndef INC_CANLIB_H_
##define INC_CANLIB_H_
#
#void initialize_CAN($', '.join(['CAN_HandleTypeDef* __h' + system.name for system in systems])$);
#

for system in systems:
    for message in system.tx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_transmit();
        #
    #

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_receive();
        #
    #
        

##endif
