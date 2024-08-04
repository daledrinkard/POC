################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
EK_RA8D1_GLCD_testbench.srec 

MAP += \
EK_RA8D1_GLCD_testbench.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/src" -I"." -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc/api" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/fsp/inc/instances" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_gen" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_cfg/fsp_cfg/bsp" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra_cfg/fsp_cfg" -I"C:/Users/daled/e2_studio/workspace/EK_RA8D1_GLCD_testbench/ra/tes/dave2d/inc" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

