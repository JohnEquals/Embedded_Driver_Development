################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PeripheralClockEnable.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
PeripheralClockEnable.list \


# All Target
all: PeripheralClockEnable.elf secondary-outputs

# Tool invocations
PeripheralClockEnable.elf: $(OBJS) $(USER_OBJS) C:\Users\class\OneDrive\Desktop\Embedded_Driver_Development\Project1\PeripheralClockEnable\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "PeripheralClockEnable.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\class\OneDrive\Desktop\Embedded_Driver_Development\Project1\PeripheralClockEnable\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="PeripheralClockEnable.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

PeripheralClockEnable.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "PeripheralClockEnable.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
