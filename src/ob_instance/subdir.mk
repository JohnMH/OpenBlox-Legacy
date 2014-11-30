################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ob_instance/Camera.cpp \
../src/ob_instance/DataModel.cpp \
../src/ob_instance/Instance.cpp \
../src/ob_instance/ServiceProvider.cpp 

OBJS += \
./src/ob_instance/Camera.o \
./src/ob_instance/DataModel.o \
./src/ob_instance/Instance.o \
./src/ob_instance/ServiceProvider.o 

CPP_DEPS += \
./src/ob_instance/Camera.d \
./src/ob_instance/DataModel.d \
./src/ob_instance/Instance.d \
./src/ob_instance/ServiceProvider.d 


# Each subdirectory must supply rules for building sources it contributes
src/ob_instance/%.o: ../src/ob_instance/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glew\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\pthreads\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glfw\include" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-write-strings -Wno-non-virtual-dtor -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


