################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CANC/can1.c \
../Core/Src/CANC/can2.c \
../Core/Src/CANC/canlib.c 

OBJS += \
./Core/Src/CANC/can1.o \
./Core/Src/CANC/can2.o \
./Core/Src/CANC/canlib.o 

C_DEPS += \
./Core/Src/CANC/can1.d \
./Core/Src/CANC/can2.d \
./Core/Src/CANC/canlib.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/CANC/%.o: ../Core/Src/CANC/%.c Core/Src/CANC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Middlewares/Third_Party/SEGGER" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Inc/CSEH" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Src/CSEC" -I"C:/Users/oscar/Documents/GitHub/AMS-CC/Core/Inc/CANH" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-CANC

clean-Core-2f-Src-2f-CANC:
	-$(RM) ./Core/Src/CANC/can1.d ./Core/Src/CANC/can1.o ./Core/Src/CANC/can2.d ./Core/Src/CANC/can2.o ./Core/Src/CANC/canlib.d ./Core/Src/CANC/canlib.o

.PHONY: clean-Core-2f-Src-2f-CANC

