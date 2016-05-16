################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_Factory.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex_POSIX.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_runable.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock_POSIX.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_thread.cpp \
../onewirelibrary/OneWire++/threadOW++/LOW_thread_thread_POSIX.cpp 

OBJS += \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_Factory.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex_POSIX.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_runable.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock_POSIX.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_thread.o \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_thread_POSIX.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_Factory.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_mutex_POSIX.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_runable.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_rwlock_POSIX.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_thread.d \
./onewirelibrary/OneWire++/threadOW++/LOW_thread_thread_POSIX.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/threadOW++/%.o: ../onewirelibrary/OneWire++/threadOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


