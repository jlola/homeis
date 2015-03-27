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
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


