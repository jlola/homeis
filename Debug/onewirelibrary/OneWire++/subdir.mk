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
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -I/root/raspberrypi/include -I"/root/raspiworkspace/dis/libs/libusbcompat/include" -O0 -g3 -Wall -c -std=c++0x -g -march=armv7-a -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


