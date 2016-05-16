################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/helper.cpp \
../onewirelibrary/main.cpp \
../onewirelibrary/test_ds2405.cpp \
../onewirelibrary/test_ds2406.cpp \
../onewirelibrary/test_ds2413.cpp \
../onewirelibrary/test_ds2890.cpp \
../onewirelibrary/test_jalousie.cpp \
../onewirelibrary/test_link.cpp \
../onewirelibrary/test_lock.cpp \
../onewirelibrary/test_msgLogging.cpp \
../onewirelibrary/test_net.cpp \
../onewirelibrary/test_switch.cpp \
../onewirelibrary/test_temp.cpp \
../onewirelibrary/test_thread.cpp 

OBJS += \
./onewirelibrary/helper.o \
./onewirelibrary/main.o \
./onewirelibrary/test_ds2405.o \
./onewirelibrary/test_ds2406.o \
./onewirelibrary/test_ds2413.o \
./onewirelibrary/test_ds2890.o \
./onewirelibrary/test_jalousie.o \
./onewirelibrary/test_link.o \
./onewirelibrary/test_lock.o \
./onewirelibrary/test_msgLogging.o \
./onewirelibrary/test_net.o \
./onewirelibrary/test_switch.o \
./onewirelibrary/test_temp.o \
./onewirelibrary/test_thread.o 

CPP_DEPS += \
./onewirelibrary/helper.d \
./onewirelibrary/main.d \
./onewirelibrary/test_ds2405.d \
./onewirelibrary/test_ds2406.d \
./onewirelibrary/test_ds2413.d \
./onewirelibrary/test_ds2890.d \
./onewirelibrary/test_jalousie.d \
./onewirelibrary/test_link.d \
./onewirelibrary/test_lock.d \
./onewirelibrary/test_msgLogging.d \
./onewirelibrary/test_net.d \
./onewirelibrary/test_switch.d \
./onewirelibrary/test_temp.d \
./onewirelibrary/test_thread.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/%.o: ../onewirelibrary/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


