################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/openblox/BaseGame.cpp \
../src/openblox/Factory.cpp \
../src/openblox/OpenBlox.cpp \
../src/openblox/OpenBloxRenderUtil.cpp \
../src/openblox/Utility.cpp \
../src/openblox/WindowUtils.cpp 

OBJS += \
./src/openblox/BaseGame.o \
./src/openblox/Factory.o \
./src/openblox/OpenBlox.o \
./src/openblox/OpenBloxRenderUtil.o \
./src/openblox/Utility.o \
./src/openblox/WindowUtils.o 

CPP_DEPS += \
./src/openblox/BaseGame.d \
./src/openblox/Factory.d \
./src/openblox/OpenBlox.d \
./src/openblox/OpenBloxRenderUtil.d \
./src/openblox/Utility.d \
./src/openblox/WindowUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/openblox/%.o: ../src/openblox/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glew\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\pthreads\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glfw\include" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-write-strings -Wno-non-virtual-dtor -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


