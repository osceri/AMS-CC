#/*
# * "canlib.c"
# *
# *  Created on: Mar 12, 2022
# *      Author: oscar
# */
#
##include "main.h"
#
##include "canlib.h"
##include "can1.h"
##include "can2.h"
##include "canlib_callbacks.h"
##include "canlib_data.h"
#

#static uint32_t tx_mailbox;
#static uint8_t rx_data[8];
#

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Param  A pointer to the message which was received
        # * @Retval None
        # */
        #__weak void $system.name$_$message.name$_rx_callback($message.name$_t* $message.name$) {
        #
        #}
        #

for system in systems:
    for message in system.tx_messages:
        #/*
        # * @Brief  
        # * @Param  A pointer to a message which should be sent
        # * @Retval Return 1 if the data entered in $message.name$ should be sent
        # */
        #__weak uint8_t $system.name$_$message.name$_tx_callback($message.name$_t* $message.name$) {
        #
        #}
        #

for system in systems:
    for message in system.rx_messages:
        #static struct $system.name$_$message.name$_t $system.name$_$message.name$;
    #
    for message in system.tx_messages:
        #static struct $system.name$_$message.name$_t $system.name$_$message.name$;
    #

for system in systems:
    for message in system.rx_messages:
        #static uint8_t $system.name$_$message.name$_flag = 0;
    #

for system in systems:
    #static CAN_HandleTypeDef* _h$system.name$;
#

for system in systems:
    for message in system.rx_messages:
        if message.multiplexer:
            #static void $system.name$_$message.name$_array_raise(uint8_t index) {
            #    static uint8_t array[1 +  $message.multiplexer_size$ / 8];
            #    static uint8_t count = 0;
            #
            #    if(index < $message.multiplexer_size$) {
            #        uint8_t div = index / 8;
            #        uint8_t rem = index % 8;
            #        uint8_t flag = (1 << rem);
            #        
            #        if(!(array[div] & flag)) {
            #            array[div] |= flag;
            #            count = (count + 1) % $message.multiplexer_size$;
            #
            #            if(count == 0) {
            #                for(int i = 0; i < (1 + $message.multiplexer_size$ / 8); i++) {
            #                    array[i] = 0;
            #                }
            #                $system.name$_$message.name$_flag = 1;
            #            }
            #        }
            #    }
            #    return count;
            #}

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_decode() {
        #   $system.name$_$message.name$_unpack(&$system.name$_$message.name$, rx_data, $message.length$);
        if message.multiplexer:
            #   $system.name$_$message.name$_array_raise($system.name$_$message.name$.$message.multiplexer$);
        else:
            #   $system.name$_$message.name$_flag = 1;
        #}
        #

for system in systems:
    for message in system.tx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_transmit() {
        #   uint8_t tx_data[8];
        #   const CAN_TxHeaderTypeDef tx_header = {
        #       .DLC = $message.length$,
        #       .IDE = CAN_ID_STD,
        #       .StdId = $message.frame_id$,
        #       .RTR = CAN_RTR_DATA,
        #       .TransmitGlobalTime = DISABLE,
        #   };
        #   $message.name$_t $message.name$;
        #   $system.name$_$message.name$_tx_callback(&$message.name$);
        #
        for signal in message.signals:
            #   $system.name$_$message.name$.$signal.name$ = $system.name$_$message.name$_$signal.name$_encode($message.name$.$signal.name$);
        #
        if message.multiplexer:
            for multiplexer_id in message.multiplexer_ids:
                #       $system.name$_$message.name$.$message.multiplexer$ = $multiplexer_id$;
                #       $system.name$_$message.name$_pack(tx_data, &$system.name$_$message.name$, $message.length$);
                #       while(0 < HAL_CAN_GetTxMailboxesFreeLevel(_h$system.name$)) {
                #           if (HAL_CAN_AddTxMessage(_h$system.name$, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
                #           Error_Handler();
                #           }
                #       }
        else:
            #   $system.name$_$message.name$_pack(tx_data, &$system.name$_$message.name$, $message.length$);
            #   while(0 < HAL_CAN_GetTxMailboxesFreeLevel(_h$system.name$)) {
            #       if (HAL_CAN_AddTxMessage(_h$system.name$, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
            #       Error_Handler();
            #       }
            #   }

        #}
        #

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_receive() {
        #   if($system.name$_$message.name$_flag) {
        #       $message.name$_t $message.name$;
        for signal in message.signals:
            if not signal.is_multiplexer:
                #       $message.name$.$signal.name$ = $system.name$_$message.name$_$signal.name$_decode($system.name$_$message.name$.$signal.name$);
        #       $system.name$_$message.name$_rx_callback(&$message.name$);
        #       $system.name$_$message.name$_flag = 0;
        #   }
        #}
        #

#void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
#   CAN_RxHeaderTypeDef rx_header;
#   static uint8_t rx_data[8];
#
#   if (HAL_OK == HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data)) {
#       switch(rx_header.StdId) {
for system in systems:
    for message in system.rx_messages:
        #           case $message.frame_id$:
        #               $system.name$_$message.name$_decode();
        #               break;
#        }
#    }
#}
#

#void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
#   CAN_RxHeaderTypeDef rx_header;
#   static uint8_t rx_data[8];
#
#   if (HAL_OK == HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data)) {
#       switch(rx_header.StdId) {
for system in systems:
    for message in system.rx_messages:
        #           case $message.frame_id$:
        #               $system.name$_$message.name$_decode();
        #               break;
#        }
#    }
#}
#
#void simple_filter() {
#   CAN_FilterTypeDef filter_config;
#   
#   filter_config.FilterActivation = ENABLE;
#   filter_config.FilterBank = 0;
#   filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
#   filter_config.FilterIdHigh = 0x0000;
#   filter_config.FilterIdLow = 0x0000;
#   filter_config.FilterMaskIdHigh = 0x0000;
#   filter_config.FilterMaskIdLow = 0x0000;
#   filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
#
for system in systems:
    #   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    #   if (HAL_CAN_ConfigFilter(_h$system.name$, &filter_config) != HAL_OK) {
    #       Error_Handler();
    #   }
    #   filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    #   if (HAL_CAN_ConfigFilter(_h$system.name$, &filter_config) != HAL_OK) {
    #       Error_Handler();
    #   }
#
#
#}
#

#void initialize_CAN($', '.join(['CAN_HandleTypeDef* __h' + system.name for system in systems])$) {
for system in systems:
    #   _h$system.name$ = __h$system.name$;
    #
    #   if (HAL_CAN_Start(_h$system.name$) != HAL_OK) {
    #       Error_Handler();
    #   }
    #   if (HAL_CAN_WakeUp(_h$system.name$) != HAL_OK) {
    #       Error_Handler();
    #   }
    #   if (HAL_CAN_ActivateNotification(_h$system.name$, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
    #       Error_Handler();
    #   }
    #
#
#   simple_filter();
#
#}
#
