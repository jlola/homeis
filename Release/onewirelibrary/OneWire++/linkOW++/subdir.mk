################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/linkOW++/LOW_link.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.cpp 

C_SRCS += \
../onewirelibrary/OneWire++/linkOW++/ow-functions.c 

OBJS += \
./onewirelibrary/OneWire++/linkOW++/LOW_link.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.o \
./onewirelibrary/OneWire++/linkOW++/ow-functions.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/linkOW++/LOW_link.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.d 

C_DEPS += \
./onewirelibrary/OneWire++/linkOW++/ow-functions.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/linkOW++/%.o: ../onewirelibrary/OneWire++/linkOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

onewirelibrary/OneWire++/linkOW++/%.o: ../onewirelibrary/OneWire++/linkOW++/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

