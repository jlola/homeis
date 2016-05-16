################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/platformOW++/LOW_platformMisc.cpp \
../onewirelibrary/OneWire++/platformOW++/LOW_platformMiscFactory.cpp \
../onewirelibrary/OneWire++/platformOW++/LOW_platformMisc_Linux.cpp 

OBJS += \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMisc.o \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMiscFactory.o \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMisc_Linux.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMisc.d \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMiscFactory.d \
./onewirelibrary/OneWire++/platformOW++/LOW_platformMisc_Linux.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/platformOW++/%.o: ../onewirelibrary/OneWire++/platformOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


