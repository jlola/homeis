################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddattribute.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddkeyword.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeconvert.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includegetattribute.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includekeyword.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includexpath.c 

OBJS += \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddattribute.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddkeyword.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeconvert.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includegetattribute.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includekeyword.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includexpath.o 

C_DEPS += \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddattribute.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeaddkeyword.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includeconvert.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includegetattribute.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includekeyword.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/includexpath.d 


# Each subdirectory must supply rules for building sources it contributes
libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/%.o: ../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


