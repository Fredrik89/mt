################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../linux/cgroups/jobsignal/src/cg.c \
../linux/cgroups/jobsignal/src/jobsignaler.c 

OBJS += \
./linux/cgroups/jobsignal/src/cg.o \
./linux/cgroups/jobsignal/src/jobsignaler.o 

C_DEPS += \
./linux/cgroups/jobsignal/src/cg.d \
./linux/cgroups/jobsignal/src/jobsignaler.d 


# Each subdirectory must supply rules for building sources it contributes
linux/cgroups/jobsignal/src/%.o: ../linux/cgroups/jobsignal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


