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


def func(x):
    x = x.replace('12',chr(12))
    x = x.replace('11',chr(11))
    x = x.replace('10',chr(10))
    x = x.replace('9', chr(9))
    x = x.replace('8', chr(8))
    x = x.replace('7', chr(7))
    x = x.replace('6', chr(6))
    x = x.replace('5', chr(5))
    x = x.replace('4', chr(4))
    x = x.replace('3', chr(3))
    x = x.replace('2', chr(2))
    x = x.replace('1', chr(1))
    return x

for system in systems:
    for message in system.rx_messages:
        #typedef struct {
        for signal in sorted(message.signals, key=lambda x: func(x.name)):
            #   $signal.type$ $signal.name$;
        #} $message.name$_t;
        #
    #

for system in systems:
    for message in system.tx_messages:
        #typedef struct {
        for signal in sorted(message.signals, key=lambda x: func(x.name)):
            #   $signal.type$ $signal.name$;
        #} $message.name$_t;
        #
    #

##endif
