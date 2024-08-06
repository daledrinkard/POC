################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/board/ra8d1_ek/board_init.c \
../ra/board/ra8d1_ek/board_leds.c \
../ra/board/ra8d1_ek/board_sdram.c 

C_DEPS += \
./ra/board/ra8d1_ek/board_init.d \
./ra/board/ra8d1_ek/board_leds.d \
./ra/board/ra8d1_ek/board_sdram.d 

OBJS += \
./ra/board/ra8d1_ek/board_init.o \
./ra/board/ra8d1_ek/board_leds.o \
./ra/board/ra8d1_ek/board_sdram.o 

SREC += \
EK_RA8D1_GLCD_tb0.srec 

MAP += \
EK_RA8D1_GLCD_tb0.map 


# Each subdirectory must supply rules for building sources it contributes
ra/board/ra8d1_ek/%.o: ../ra/board/ra8d1_ek/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM85 -D_RA_ORDINAL=1 -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/src" -I"." -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra/fsp/inc" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra/fsp/inc/api" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra/fsp/inc/instances" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra_gen" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra_cfg/fsp_cfg/bsp" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra_cfg/fsp_cfg" -I"C:/Projects/Projects (repos)/RA8 graphics benchmark/EK_RA8D1_GLCD_tb0/ra/tes/dave2d/inc" -I"C:/Projects/Projects (repos)/SCI_RTT/src/SCI_RTT" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

