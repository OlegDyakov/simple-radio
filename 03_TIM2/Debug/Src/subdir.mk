################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio.c \
../Src/main.c \
../Src/system_init.c \
../Src/tim2.c \
../Src/vectors.c 

OBJS += \
./Src/gpio.o \
./Src/main.o \
./Src/system_init.o \
./Src/tim2.o \
./Src/vectors.o 

C_DEPS += \
./Src/gpio.d \
./Src/main.d \
./Src/system_init.d \
./Src/tim2.d \
./Src/vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Src/gpio.o: ../Src/gpio.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431KBTx -DSTM32 -DSTM32G4 -DDEBUG -c -I"D:/Development/ST/Nucleo-32/STM32G431/Article/03_TIM2/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431KBTx -DSTM32 -DSTM32G4 -DDEBUG -c -I"D:/Development/ST/Nucleo-32/STM32G431/Article/03_TIM2/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_init.o: ../Src/system_init.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431KBTx -DSTM32 -DSTM32G4 -DDEBUG -c -I"D:/Development/ST/Nucleo-32/STM32G431/Article/03_TIM2/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_init.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim2.o: ../Src/tim2.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431KBTx -DSTM32 -DSTM32G4 -DDEBUG -c -I"D:/Development/ST/Nucleo-32/STM32G431/Article/03_TIM2/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim2.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/vectors.o: ../Src/vectors.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431KBTx -DSTM32 -DSTM32G4 -DDEBUG -c -I"D:/Development/ST/Nucleo-32/STM32G431/Article/03_TIM2/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/vectors.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

