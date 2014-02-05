################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../homeis/Devices/HisDallas.cpp \
../homeis/Devices/HisDevBase.cpp \
../homeis/Devices/HisDevFactory.cpp \
../homeis/Devices/HisDevIO2406.cpp \
../homeis/Devices/HisDevIO2413.cpp \
../homeis/Devices/HisDevRunTime.cpp \
../homeis/Devices/HisDevValue.cpp \
../homeis/Devices/HisDevices.cpp \
../homeis/Devices/ValueEventArgs.cpp 

OBJS += \
./homeis/Devices/HisDallas.o \
./homeis/Devices/HisDevBase.o \
./homeis/Devices/HisDevFactory.o \
./homeis/Devices/HisDevIO2406.o \
./homeis/Devices/HisDevIO2413.o \
./homeis/Devices/HisDevRunTime.o \
./homeis/Devices/HisDevValue.o \
./homeis/Devices/HisDevices.o \
./homeis/Devices/ValueEventArgs.o 

CPP_DEPS += \
./homeis/Devices/HisDallas.d \
./homeis/Devices/HisDevBase.d \
./homeis/Devices/HisDevFactory.d \
./homeis/Devices/HisDevIO2406.d \
./homeis/Devices/HisDevIO2413.d \
./homeis/Devices/HisDevRunTime.d \
./homeis/Devices/HisDevValue.d \
./homeis/Devices/HisDevices.d \
./homeis/Devices/ValueEventArgs.d 


# Each subdirectory must supply rules for building sources it contributes
homeis/Devices/%.o: ../homeis/Devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -O0 -g3 -Wall -c -std=c++0x -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


