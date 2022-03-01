################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../external_device_drivers/src/1602a_lcd.c \
../external_device_drivers/src/dht11.c 

OBJS += \
./external_device_drivers/src/1602a_lcd.o \
./external_device_drivers/src/dht11.o 

C_DEPS += \
./external_device_drivers/src/1602a_lcd.d \
./external_device_drivers/src/dht11.d 


# Each subdirectory must supply rules for building sources it contributes
external_device_drivers/src/%.o: ../external_device_drivers/src/%.c external_device_drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -I"E:/Users/class/Documents/repo/blank/stm32f407xx_drivers/external_device_drivers/public" -I"E:/Users/class/Documents/repo/blank/stm32f407xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-external_device_drivers-2f-src

clean-external_device_drivers-2f-src:
	-$(RM) ./external_device_drivers/src/1602a_lcd.d ./external_device_drivers/src/1602a_lcd.o ./external_device_drivers/src/dht11.d ./external_device_drivers/src/dht11.o

.PHONY: clean-external_device_drivers-2f-src

