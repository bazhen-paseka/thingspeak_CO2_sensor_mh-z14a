################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/gpio.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c \
../Src/usart.c 

OBJS += \
./Src/dma.o \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o \
./Src/usart.o 

C_DEPS += \
./Src/dma.d \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Inc" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Drivers/ringbuffer" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Drivers/STM32F1xx_HAL_Driver/Inc" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Drivers/CMSIS/Include" -I"E:/5 WorkSpace/WorkSpace 2016/2017-10-22-f103-CO2/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


