################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libhttpserver-master/src/http_request.cpp \
../libhttpserver-master/src/http_resource.cpp \
../libhttpserver-master/src/http_response.cpp \
../libhttpserver-master/src/http_utils.cpp \
../libhttpserver-master/src/string_utilities.cpp \
../libhttpserver-master/src/webserver.cpp 

OBJS += \
./libhttpserver-master/src/http_request.o \
./libhttpserver-master/src/http_resource.o \
./libhttpserver-master/src/http_response.o \
./libhttpserver-master/src/http_utils.o \
./libhttpserver-master/src/string_utilities.o \
./libhttpserver-master/src/webserver.o 

CPP_DEPS += \
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
	g++ -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


