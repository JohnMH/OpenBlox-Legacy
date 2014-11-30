################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ob_type/Vector3.cpp 

OBJS += \
./src/ob_type/Vector3.o 

CPP_DEPS += \
./src/ob_type/Vector3.d 


# Each subdirectory must supply rules for building sources it contributes
src/ob_type/%.o: ../src/ob_type/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glew\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\pthreads\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glfw\include" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-write-strings -Wno-non-virtual-dtor -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


