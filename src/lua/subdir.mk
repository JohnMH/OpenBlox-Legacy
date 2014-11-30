################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lua/lapi.c \
../src/lua/lauxlib.c \
../src/lua/lbaselib.c \
../src/lua/lcode.c \
../src/lua/ldblib.c \
../src/lua/ldebug.c \
../src/lua/ldo.c \
../src/lua/ldump.c \
../src/lua/lfunc.c \
../src/lua/lgc.c \
../src/lua/linit.c \
../src/lua/liolib.c \
../src/lua/llex.c \
../src/lua/lmathlib.c \
../src/lua/lmem.c \
../src/lua/loadlib.c \
../src/lua/lobject.c \
../src/lua/lopcodes.c \
../src/lua/loslib.c \
../src/lua/lparser.c \
../src/lua/lstate.c \
../src/lua/lstring.c \
../src/lua/lstrlib.c \
../src/lua/ltable.c \
../src/lua/ltablib.c \
../src/lua/ltm.c \
../src/lua/lundump.c \
../src/lua/lvm.c \
../src/lua/lzio.c \
../src/lua/print.c 

OBJS += \
./src/lua/lapi.o \
./src/lua/lauxlib.o \
./src/lua/lbaselib.o \
./src/lua/lcode.o \
./src/lua/ldblib.o \
./src/lua/ldebug.o \
./src/lua/ldo.o \
./src/lua/ldump.o \
./src/lua/lfunc.o \
./src/lua/lgc.o \
./src/lua/linit.o \
./src/lua/liolib.o \
./src/lua/llex.o \
./src/lua/lmathlib.o \
./src/lua/lmem.o \
./src/lua/loadlib.o \
./src/lua/lobject.o \
./src/lua/lopcodes.o \
./src/lua/loslib.o \
./src/lua/lparser.o \
./src/lua/lstate.o \
./src/lua/lstring.o \
./src/lua/lstrlib.o \
./src/lua/ltable.o \
./src/lua/ltablib.o \
./src/lua/ltm.o \
./src/lua/lundump.o \
./src/lua/lvm.o \
./src/lua/lzio.o \
./src/lua/print.o 

C_DEPS += \
./src/lua/lapi.d \
./src/lua/lauxlib.d \
./src/lua/lbaselib.d \
./src/lua/lcode.d \
./src/lua/ldblib.d \
./src/lua/ldebug.d \
./src/lua/ldo.d \
./src/lua/ldump.d \
./src/lua/lfunc.d \
./src/lua/lgc.d \
./src/lua/linit.d \
./src/lua/liolib.d \
./src/lua/llex.d \
./src/lua/lmathlib.d \
./src/lua/lmem.d \
./src/lua/loadlib.d \
./src/lua/lobject.d \
./src/lua/lopcodes.d \
./src/lua/loslib.d \
./src/lua/lparser.d \
./src/lua/lstate.d \
./src/lua/lstring.d \
./src/lua/lstrlib.d \
./src/lua/ltable.d \
./src/lua/ltablib.d \
./src/lua/ltm.d \
./src/lua/lundump.d \
./src/lua/lvm.d \
./src/lua/lzio.d \
./src/lua/print.d 


# Each subdirectory must supply rules for building sources it contributes
src/lua/%.o: ../src/lua/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glew\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\pthreads\include" -I"C:\Users\john\workspace_mwr\OpenBlox\deps\glfw\include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


