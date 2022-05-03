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
    max_len = round(1.5*sum(len(msg.multiplexer_ids) for msg in system.tx_messages))
    #static $system.name$_tx_filo_max = $max_len$;
    #static $system.name$_tx_filo_index = 0;
    #static can_queue_element_t $system.name$_tx_filo[$max_len$];
    #
#

for system in systems:
    for message in system.rx_messages:
        #static struct $system.name$_$message.name$_t $system.name$_rx_$message.name$;
    #
    for message in system.tx_messages:
        #static struct $system.name$_$message.name$_t $system.name$_tx_$message.name$;
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
        msize = len(message.multiplexer_ids)
        if message.multiplexer:
            #static void $system.name$_$message.name$_array_raise(uint16_t _index) {
            #   static uint8_t array[1 +  $msize$ / 8];
            #   static uint8_t count = 0;
            #   uint8_t index;
            #
            #   switch(_index) {
            _q = 0
            for multiplexer_id in message.multiplexer_ids:
                #       case $multiplexer_id$: index = $_q$; break;
                _q = _q + 1;
            #       default: return;
            #   }
            #
            #    if(index < $msize$) {
            #        uint8_t div = index / 8;
            #        uint8_t rem = index % 8;
            #        uint8_t flag = (1 << rem);
            #        
            #        if(!(array[div] & flag)) {
            #            array[div] |= flag;
            #            count = (count + 1) % $msize$;
            #
            #            if(count == 0) {
            #                for(int i = 0; i < (1 + $msize$ / 8); i++) {
            #                    array[i] = 0;
            #                }
            #                $system.name$_$message.name$_flag = 1;
            #            }
            #        }
            #    }
            #}

for system in systems:
    for message in system.rx_messages:
        #/*
        # * @Brief  
        # * @Retval None
        # */
        #void $system.name$_$message.name$_decode() {
        #   $system.name$_$message.name$_unpack(&$system.name$_rx_$message.name$, rx_data, $message.length$);
        if message.multiplexer:
            #   $system.name$_$message.name$_array_raise($system.name$_rx_$message.name$.$message.multiplexer$);
        else:
            #   $system.name$_$message.name$_flag = 1;
        #   $system.name$_$message.name$_receive();
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
            #   $system.name$_tx_$message.name$.$signal.name$ = $system.name$_$message.name$_$signal.name$_encode($message.name$.$signal.name$);
        #
        if message.multiplexer:
            for multiplexer_id in message.multiplexer_ids:
                #       $system.name$_tx_$message.name$.$message.multiplexer$ = $multiplexer_id$;
                #       $system.name$_$message.name$_pack(tx_data, &$system.name$_tx_$message.name$, $message.length$);
                #       while(!HAL_CAN_GetTxMailboxesFreeLevel(_h$system.name$));
                #       if (HAL_CAN_AddTxMessage(_h$system.name$, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
                #           Error_Handler();
                #       }
        else:
            #   $system.name$_$message.name$_pack(tx_data, &$system.name$_tx_$message.name$, $message.length$);
            #   while(!HAL_CAN_GetTxMailboxesFreeLevel(_h$system.name$));
            #   if (HAL_CAN_AddTxMessage(_h$system.name$, &tx_header, tx_data, &tx_mailbox) != HAL_OK) {
            #       Error_Handler();
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
                #       $message.name$.$signal.name$ = $system.name$_$message.name$_$signal.name$_decode($system.name$_rx_$message.name$.$signal.name$);
        #       $system.name$_$message.name$_rx_callback(&$message.name$);
        #       $system.name$_$message.name$_flag = 0;
        #   }
        #}
        #

#void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
#   CAN_RxHeaderTypeDef rx_header;
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
