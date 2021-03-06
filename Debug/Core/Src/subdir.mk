################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/COM.c \
../Core/Src/FAN.c \
../Core/Src/IMD.c \
../Core/Src/LTC.c \
../Core/Src/PID.c \
../Core/Src/SEGGER_SYSVIEW_Config_FreeRTOS.c \
../Core/Src/SEGGER_SYSVIEW_FreeRTOS.c \
../Core/Src/adc.c \
../Core/Src/can.c \
../Core/Src/canlib_callbacks.c \
../Core/Src/canlib_callbacks_sim0.c \
../Core/Src/cell_balancer.c \
../Core/Src/dma.c \
../Core/Src/freertos.c \
../Core/Src/gpio.c \
../Core/Src/iwdg.c \
../Core/Src/main.c \
../Core/Src/programme_functions.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c 

OBJS += \
./Core/Src/COM.o \
./Core/Src/FAN.o \
./Core/Src/IMD.o \
./Core/Src/LTC.o \
./Core/Src/PID.o \
./Core/Src/SEGGER_SYSVIEW_Config_FreeRTOS.o \
./Core/Src/SEGGER_SYSVIEW_FreeRTOS.o \
./Core/Src/adc.o \
./Core/Src/can.o \
./Core/Src/canlib_callbacks.o \
./Core/Src/canlib_callbacks_sim0.o \
./Core/Src/cell_balancer.o \
./Core/Src/dma.o \
./Core/Src/freertos.o \
./Core/Src/gpio.o \
./Core/Src/iwdg.o \
./Core/Src/main.o \
./Core/Src/programme_functions.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o 

C_DEPS += \
./Core/Src/COM.d \
./Core/Src/FAN.d \
./Core/Src/IMD.d \
./Core/Src/LTC.d \
./Core/Src/PID.d \
./Core/Src/SEGGER_SYSVIEW_Config_FreeRTOS.d \
./Core/Src/SEGGER_SYSVIEW_FreeRTOS.d \
./Core/Src/adc.d \
./Core/Src/can.d \
./Core/Src/canlib_callbacks.d \
./Core/Src/canlib_callbacks_sim0.d \
./Core/Src/cell_balancer.d \
./Core/Src/dma.d \
./Core/Src/freertos.d \
./Core/Src/gpio.d \
./Core/Src/iwdg.d \
./Core/Src/main.d \
./Core/Src/programme_functions.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Middlewares/Third_Party/SEGGER" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Inc/CSEH" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Src/CSEC" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Inc/CANH" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Inc/SMILEH" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Src/SMILEC" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/COM.d ./Core/Src/COM.o ./Core/Src/FAN.d ./Core/Src/FAN.o ./Core/Src/IMD.d ./Core/Src/IMD.o ./Core/Src/LTC.d ./Core/Src/LTC.o ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/SEGGER_SYSVIEW_Config_FreeRTOS.d ./Core/Src/SEGGER_SYSVIEW_Config_FreeRTOS.o ./Core/Src/SEGGER_SYSVIEW_FreeRTOS.d ./Core/Src/SEGGER_SYSVIEW_FreeRTOS.o ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/canlib_callbacks.d ./Core/Src/canlib_callbacks.o ./Core/Src/canlib_callbacks_sim0.d ./Core/Src/canlib_callbacks_sim0.o ./Core/Src/cell_balancer.d ./Core/Src/cell_balancer.o ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/iwdg.d ./Core/Src/iwdg.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/programme_functions.d ./Core/Src/programme_functions.o ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/tim.d ./Core/Src/tim.o

.PHONY: clean-Core-2f-Src

