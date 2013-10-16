################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uuid/clear.c \
../uuid/compare.c \
../uuid/copy.c \
../uuid/gen_uuid.c \
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
	arm-linux-gnueabihf-gcc -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/onewirelibrary" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -I"/root/raspiworkspace/dis/linuxcs" -I"/root/raspiworkspace/dis/libhttpserver-master/src/httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"/root/raspiworkspace/dis/libhttpserver-master/src" -I"/root/raspiworkspace/dis/uuid" -I/root/raspberrypi/libxml2-2.8.0/include/libxml2 -I"/root/raspiworkspace/dis/lua-5.2.1/src" -O0 -g3 -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


