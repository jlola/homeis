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
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -I/root/raspberrypi/include -I"/root/raspiworkspace/dis/libs/libusbcompat/include" -O0 -g3 -Wall -c -std=c++0x -g -march=armv7-a -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


