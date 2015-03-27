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
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


