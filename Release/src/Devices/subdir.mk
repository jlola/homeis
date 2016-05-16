################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Devices/HisDallas.cpp \
../src/Devices/HisDevBase.cpp \
../src/Devices/HisDevFactory.cpp \
../src/Devices/HisDevIO2406.cpp \
../src/Devices/HisDevIO2408.cpp \
../src/Devices/HisDevIO2413.cpp \
../src/Devices/HisDevLCD.cpp \
../src/Devices/HisDevRunTime.cpp \
../src/Devices/HisDevValue.cpp \
../src/Devices/HisDevices.cpp \
../src/Devices/ValueEventArgs.cpp 

OBJS += \
./src/Devices/HisDallas.o \
./src/Devices/HisDevBase.o \
./src/Devices/HisDevFactory.o \
./src/Devices/HisDevIO2406.o \
./src/Devices/HisDevIO2408.o \
./src/Devices/HisDevIO2413.o \
./src/Devices/HisDevLCD.o \
./src/Devices/HisDevRunTime.o \
./src/Devices/HisDevValue.o \
./src/Devices/HisDevices.o \
./src/Devices/ValueEventArgs.o 

CPP_DEPS += \
./src/Devices/HisDallas.d \
./src/Devices/HisDevBase.d \
./src/Devices/HisDevFactory.d \
./src/Devices/HisDevIO2406.d \
./src/Devices/HisDevIO2408.d \
./src/Devices/HisDevIO2413.d \
./src/Devices/HisDevLCD.d \
./src/Devices/HisDevRunTime.d \
./src/Devices/HisDevValue.d \
./src/Devices/HisDevices.d \
./src/Devices/ValueEventArgs.d 


# Each subdirectory must supply rules for building sources it contributes
src/Devices/%.o: ../src/Devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


