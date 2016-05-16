################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/LOW_component.cpp \
../onewirelibrary/OneWire++/LOW_device.cpp \
../onewirelibrary/OneWire++/LOW_deviceID.cpp \
../onewirelibrary/OneWire++/LOW_deviceIDRaw.cpp \
../onewirelibrary/OneWire++/LOW_exception.cpp \
../onewirelibrary/OneWire++/LOW_helper_crc.cpp \
../onewirelibrary/OneWire++/LOW_helper_msglog.cpp \
../onewirelibrary/OneWire++/LOW_netSegment.cpp \
../onewirelibrary/OneWire++/LOW_network.cpp \
../onewirelibrary/OneWire++/LOW_objectIDFactory.cpp \
../onewirelibrary/OneWire++/LOW_objectSynchronizer.cpp 

OBJS += \
./onewirelibrary/OneWire++/LOW_component.o \
./onewirelibrary/OneWire++/LOW_device.o \
./onewirelibrary/OneWire++/LOW_deviceID.o \
./onewirelibrary/OneWire++/LOW_deviceIDRaw.o \
./onewirelibrary/OneWire++/LOW_exception.o \
./onewirelibrary/OneWire++/LOW_helper_crc.o \
./onewirelibrary/OneWire++/LOW_helper_msglog.o \
./onewirelibrary/OneWire++/LOW_netSegment.o \
./onewirelibrary/OneWire++/LOW_network.o \
./onewirelibrary/OneWire++/LOW_objectIDFactory.o \
./onewirelibrary/OneWire++/LOW_objectSynchronizer.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/LOW_component.d \
./onewirelibrary/OneWire++/LOW_device.d \
./onewirelibrary/OneWire++/LOW_deviceID.d \
./onewirelibrary/OneWire++/LOW_deviceIDRaw.d \
./onewirelibrary/OneWire++/LOW_exception.d \
./onewirelibrary/OneWire++/LOW_helper_crc.d \
./onewirelibrary/OneWire++/LOW_helper_msglog.d \
./onewirelibrary/OneWire++/LOW_netSegment.d \
./onewirelibrary/OneWire++/LOW_network.d \
./onewirelibrary/OneWire++/LOW_objectIDFactory.d \
./onewirelibrary/OneWire++/LOW_objectSynchronizer.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/%.o: ../onewirelibrary/OneWire++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


