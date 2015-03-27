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
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -std=c11 -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


