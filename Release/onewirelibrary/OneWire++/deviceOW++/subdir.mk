################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2408.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOWdevLCD.cpp 

OBJS += \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2408.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.o \
./onewirelibrary/OneWire++/deviceOW++/LOWdevLCD.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2408.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.d \
./onewirelibrary/OneWire++/deviceOW++/LOWdevLCD.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/deviceOW++/%.o: ../onewirelibrary/OneWire++/deviceOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


