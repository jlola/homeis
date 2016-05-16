################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uuid/clear.c \
../uuid/compare.c \
../uuid/copy.c \
../uuid/gen_uuid.c \
../uuid/gen_uuid_nt.c \
../uuid/isnull.c \
../uuid/pack.c \
../uuid/parse.c \
../uuid/tst_uuid.c \
../uuid/unpack.c \
../uuid/unparse.c \
../uuid/uuid_time.c 

OBJS += \
./uuid/clear.o \
./uuid/compare.o \
./uuid/copy.o \
./uuid/gen_uuid.o \
./uuid/gen_uuid_nt.o \
./uuid/isnull.o \
./uuid/pack.o \
./uuid/parse.o \
./uuid/tst_uuid.o \
./uuid/unpack.o \
./uuid/unparse.o \
./uuid/uuid_time.o 

C_DEPS += \
./uuid/clear.d \
./uuid/compare.d \
./uuid/copy.d \
./uuid/gen_uuid.d \
./uuid/gen_uuid_nt.d \
./uuid/isnull.d \
./uuid/pack.d \
./uuid/parse.d \
./uuid/tst_uuid.d \
./uuid/unpack.d \
./uuid/unparse.d \
./uuid/uuid_time.d 


# Each subdirectory must supply rules for building sources it contributes
uuid/%.o: ../uuid/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


