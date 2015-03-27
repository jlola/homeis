################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libhttpserver-master/src/http_endpoint.cpp \
../libhttpserver-master/src/http_request.cpp \
../libhttpserver-master/src/http_resource.cpp \
../libhttpserver-master/src/http_response.cpp \
../libhttpserver-master/src/http_utils.cpp \
../libhttpserver-master/src/string_utilities.cpp \
../libhttpserver-master/src/webserver.cpp 

OBJS += \
./libhttpserver-master/src/http_endpoint.o \
./libhttpserver-master/src/http_request.o \
./libhttpserver-master/src/http_resource.o \
./libhttpserver-master/src/http_response.o \
./libhttpserver-master/src/http_utils.o \
./libhttpserver-master/src/string_utilities.o \
./libhttpserver-master/src/webserver.o 

CPP_DEPS += \
./libhttpserver-master/src/http_endpoint.d \
./libhttpserver-master/src/http_request.d \
./libhttpserver-master/src/http_resource.d \
./libhttpserver-master/src/http_response.d \
./libhttpserver-master/src/http_utils.d \
./libhttpserver-master/src/string_utilities.d \
./libhttpserver-master/src/webserver.d 


# Each subdirectory must supply rules for building sources it contributes
libhttpserver-master/src/%.o: ../libhttpserver-master/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++0x -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\threadOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\componentOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\linkOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\deviceOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\platformOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary\OneWire++\portOW++" -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -I"D:\Elektronika\homeis\homeisworkspace\dis" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -lrt -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


