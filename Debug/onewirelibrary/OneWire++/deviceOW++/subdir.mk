################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.cpp \
../onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.cpp 

OBJS += \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.o \
./onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS1820.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2401.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2405.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2406.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2413.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devDS2890.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_devUnknown.d \
./onewirelibrary/OneWire++/deviceOW++/LOW_deviceFactory.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/deviceOW++/%.o: ../onewirelibrary/OneWire++/deviceOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


