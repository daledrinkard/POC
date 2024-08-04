################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

SREC += \
EK_RA8D1_GLCD_testbench.srec 

MAP += \
EK_RA8D1_GLCD_testbench.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/src" -I"." -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc/api" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc/instances" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_gen" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_cfg/fsp_cfg/bsp" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_cfg/fsp_cfg" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/tes/dave2d/inc" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

