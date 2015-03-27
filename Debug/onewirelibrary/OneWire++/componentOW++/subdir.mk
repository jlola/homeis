################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../onewirelibrary/OneWire++/componentOW++/LOW_compJalousieController.cpp \
../onewirelibrary/OneWire++/componentOW++/LOW_compTempSensor.cpp \
../onewirelibrary/OneWire++/componentOW++/LOW_compToggleSwitch.cpp \
../onewirelibrary/OneWire++/componentOW++/LOW_compTwinSwitch.cpp 

OBJS += \
./onewirelibrary/OneWire++/componentOW++/LOW_compJalousieController.o \
./onewirelibrary/OneWire++/componentOW++/LOW_compTempSensor.o \
./onewirelibrary/OneWire++/componentOW++/LOW_compToggleSwitch.o \
./onewirelibrary/OneWire++/componentOW++/LOW_compTwinSwitch.o 

CPP_DEPS += \
./onewirelibrary/OneWire++/componentOW++/LOW_compJalousieController.d \
./onewirelibrary/OneWire++/componentOW++/LOW_compTempSensor.d \
./onewirelibrary/OneWire++/componentOW++/LOW_compToggleSwitch.d \
./onewirelibrary/OneWire++/componentOW++/LOW_compTwinSwitch.d 


# Each subdirectory must supply rules for building sources it contributes
onewirelibrary/OneWire++/componentOW++/%.o: ../onewirelibrary/OneWire++/componentOW++/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


