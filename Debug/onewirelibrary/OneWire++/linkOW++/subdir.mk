################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/linkOW++/LOW_link.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.cpp \
../onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.cpp 

OBJS += \
./onewirelibrary/OneWire++/linkOW++/LOW_link.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.o \
./onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/linkOW++/LOW_link.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2480B.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkDS2490.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkFlexibleSpeed.d \
./onewirelibrary/OneWire++/linkOW++/LOW_linkPassiveSerial.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/linkOW++/%.o: ../onewirelibrary/OneWire++/linkOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


