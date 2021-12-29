################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AFIO_prog.c \
../src/EXTI_prog.c \
../src/GPIO_prog.c \
../src/IR_prog.c \
../src/LEDMRX_prog.c \
../src/NVIC_prog.c \
../src/RCC_program.c \
../src/STK_prog.c \
../src/TIMER2to5_prog.c \
../src/initialize-hardware.c \
../src/main.c \
../src/stm32f1xx_hal_msp.c \
../src/write.c 

OBJS += \
./src/AFIO_prog.o \
./src/EXTI_prog.o \
./src/GPIO_prog.o \
./src/IR_prog.o \
./src/LEDMRX_prog.o \
./src/NVIC_prog.o \
./src/RCC_program.o \
./src/STK_prog.o \
./src/TIMER2to5_prog.o \
./src/initialize-hardware.o \
./src/main.o \
./src/stm32f1xx_hal_msp.o \
./src/write.o 

C_DEPS += \
./src/AFIO_prog.d \
./src/EXTI_prog.d \
./src/GPIO_prog.d \
./src/IR_prog.d \
./src/LEDMRX_prog.d \
./src/NVIC_prog.d \
./src/RCC_program.d \
./src/STK_prog.d \
./src/TIMER2to5_prog.d \
./src/initialize-hardware.d \
./src/main.d \
./src/stm32f1xx_hal_msp.d \
./src/write.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F103xB -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/stm32f1xx_hal_msp.o: ../src/stm32f1xx_hal_msp.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F103xB -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-hal" -std=gnu11 -Wno-padded -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


