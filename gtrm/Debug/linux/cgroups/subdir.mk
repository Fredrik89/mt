################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../linux/cgroups/gtrm.c 

OBJS += \
./linux/cgroups/gtrm.o 

C_DEPS += \
./linux/cgroups/gtrm.d 


# Each subdirectory must supply rules for building sources it contributes
linux/cgroups/%.o: ../linux/cgroups/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


