################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/testbench/subdir.mk
-include src/draw_core/subdir.mk
-include src/SEGGER_RTT/subdir.mk
-include src/SCI_RTT/subdir.mk
-include src/MIPI/subdir.mk
-include src/GLCD/subdir.mk
-include src/subdir.mk
-include ra_gen/subdir.mk
-include ra/tes/dave2d/src/subdir.mk
-include ra/fsp/src/r_sci_b_uart/subdir.mk
-include ra/fsp/src/r_mipi_phy/subdir.mk
-include ra/fsp/src/r_mipi_dsi/subdir.mk
-include ra/fsp/src/r_ioport/subdir.mk
-include ra/fsp/src/r_gpt/subdir.mk
-include ra/fsp/src/r_glcdc/subdir.mk
-include ra/fsp/src/r_drw/subdir.mk
-include ra/fsp/src/bsp/mcu/all/subdir.mk
-include ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/subdir.mk
-include ra/board/ra8d1_ek/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
EK_RA8D1_GLCD_tb0.hex \

SECONDARY_SIZE += \
EK_RA8D1_GLCD_tb0.siz \


# All Target
all:
	$(MAKE) --no-print-directory main-build
	$(MAKE) --no-print-directory post-build

# Main-build Target
main-build: EK_RA8D1_GLCD_tb0.elf secondary-outputs

# Tool invocations
EK_RA8D1_GLCD_tb0.elf: $(OBJS) $(USER_OBJS) $(LINKER_SCRIPT)
	$(file > EK_RA8D1_GLCD_tb0.elf.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -T "fsp.ld" -Xlinker --gc-sections -L "C:\\Projects\\Projects (repos)\\RA8 graphics benchmark\\EK_RA8D1_GLCD_tb0/script" -Wl,-Map,"EK_RA8D1_GLCD_tb0.map" --specs=nano.specs --specs=nosys.specs -o "EK_RA8D1_GLCD_tb0.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group)
	@echo Building target: $@ && arm-none-eabi-gcc @"EK_RA8D1_GLCD_tb0.elf.in"
EK_RA8D1_GLCD_tb0.hex: EK_RA8D1_GLCD_tb0.elf
	arm-none-eabi-objcopy -O ihex "EK_RA8D1_GLCD_tb0.elf"  "EK_RA8D1_GLCD_tb0.hex"
EK_RA8D1_GLCD_tb0.siz: EK_RA8D1_GLCD_tb0.elf
	arm-none-eabi-size --format=berkeley "EK_RA8D1_GLCD_tb0.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(JMP_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(P_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(FSY_UPPER_DEPS)  $(C_DEPS)  $(SRC_DEPS)  $(JMP_UPPER_DEPS)  $(PP_DEPS)  $(ASM_UPPER_DEPS)  $(SX_DEPS)  $(SX_UPPER_DEPS)  $(OBJS)  $(SECONDARY_FLASH)  $(ASM_DEPS)  $(SREC)  $(S_UPPER_DEPS)  $(MAP) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  EK_RA8D1_GLCD_tb0.elf
	-@echo ' '

post-build:
	-sed -n '/.bss._SEGGER_RTT/{p;n;p;n;p}' EK_RA8D1_GLCD_tb0.map
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
