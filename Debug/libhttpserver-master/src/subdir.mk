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
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -I/root/raspberrypi/include -I"/root/raspiworkspace/dis/libs/libusbcompat/include" -I/root/gcc-linaro-arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.8.2/include -I/root/gcc-linaro-arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.8.2/include-fixed -O0 -g -p -pg -Wall -c -std=c++0x -g -march=armv7-a  -funwind-tables -fno-optimize-sibling-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


