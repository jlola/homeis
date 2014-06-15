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
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -I/root/raspberrypi/include -I"/root/raspiworkspace/dis/libs/libusbcompat/include" -O0 -g3 -Wall -c -std=c++0x -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


