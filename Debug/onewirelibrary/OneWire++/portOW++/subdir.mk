################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/portOW++/LOW_portSerial.cpp \
../onewirelibrary/OneWire++/portOW++/LOW_portSerialFactory.cpp \
../onewirelibrary/OneWire++/portOW++/LOW_portSerial_Linux.cpp \
../onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice.cpp \
../onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice_Linux.cpp \
../onewirelibrary/OneWire++/portOW++/LOW_portUsb_Factory.cpp 

OBJS += \
./onewirelibrary/OneWire++/portOW++/LOW_portSerial.o \
./onewirelibrary/OneWire++/portOW++/LOW_portSerialFactory.o \
./onewirelibrary/OneWire++/portOW++/LOW_portSerial_Linux.o \
./onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice.o \
./onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice_Linux.o \
./onewirelibrary/OneWire++/portOW++/LOW_portUsb_Factory.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/portOW++/LOW_portSerial.d \
./onewirelibrary/OneWire++/portOW++/LOW_portSerialFactory.d \
./onewirelibrary/OneWire++/portOW++/LOW_portSerial_Linux.d \
./onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice.d \
./onewirelibrary/OneWire++/portOW++/LOW_portUsbDevice_Linux.d \
./onewirelibrary/OneWire++/portOW++/LOW_portUsb_Factory.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/portOW++/%.o: ../onewirelibrary/OneWire++/portOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -I/root/raspberrypi/include -I"/root/raspiworkspace/dis/libs/libusbcompat/include" -I/root/gcc-linaro-arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.8.2/include -I/root/gcc-linaro-arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.8.2/include-fixed -O0 -g -p -pg -Wall -c -std=c++0x -g -march=armv7-a  -funwind-tables -fno-optimize-sibling-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


