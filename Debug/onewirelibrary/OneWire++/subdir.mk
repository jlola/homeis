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
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


