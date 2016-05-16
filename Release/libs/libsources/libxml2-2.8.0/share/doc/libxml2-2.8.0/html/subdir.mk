################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io1.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io2.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse1.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse2.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse3.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse4.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader1.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader2.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader3.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader4.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/testWriter.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree1.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree2.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath1.c \
../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath2.c 

OBJS += \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io1.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io2.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse1.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse2.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse3.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse4.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader1.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader2.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader3.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader4.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/testWriter.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree1.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree2.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath1.o \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath2.o 

C_DEPS += \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io1.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/io2.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse1.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse2.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse3.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/parse4.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader1.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader2.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader3.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/reader4.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/testWriter.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree1.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tree2.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath1.d \
./libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/xpath2.d 


# Each subdirectory must supply rules for building sources it contributes
libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/%.o: ../libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


