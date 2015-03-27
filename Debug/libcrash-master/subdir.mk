################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libcrash-master/auto_crash.c \
../libcrash-master/auto_test.c \
../libcrash-master/crash.c \
../libcrash-master/crashd.c \
../libcrash-master/naive.c \
../libcrash-master/test.c 

OBJS += \
./libcrash-master/auto_crash.o \
./libcrash-master/auto_test.o \
./libcrash-master/crash.o \
./libcrash-master/crashd.o \
./libcrash-master/naive.o \
./libcrash-master/test.o 

C_DEPS += \
./libcrash-master/auto_crash.d \
./libcrash-master/auto_test.d \
./libcrash-master/crash.d \
./libcrash-master/crashd.d \
./libcrash-master/naive.d \
./libcrash-master/test.d 


# Each subdirectory must supply rules for building sources it contributes
libcrash-master/%.o: ../libcrash-master/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -std=c11 -D_HTTPSERVER_HPP_INSIDE_ -DHTTPSERVER_COMPILATION -DUSE_THREADS -I"D:\Elektronika\homeis\homeisworkspace\dis\onewirelibrary" -I"D:\Elektronika\homeis\homeisworkspace\dis\rapidjson" -I"D:\Elektronika\homeis\homeisworkspace\dis" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Devices" -I"D:\Elektronika\homeis\homeisworkspace\dis\homeis\Helpers" -I"D:\Elektronika\homeis\homeisworkspace\dis\linuxcs" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src\httpserver" -I"D:\Elektronika\homeis\homeisworkspace\dis\libhttpserver-master\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\uuid" -I"D:\Elektronika\homeis\homeisworkspace\dis\lua-5.2.1\src" -I"D:\Elektronika\homeis\homeisworkspace\dis\libs\libusbcompat\include" -I"D:\Elektronika\homeis\homeisworkspace\libmicrohttpd\src\include" -I"D:\Elektronika\homeis\homeisworkspace\libxml2-2.8.0\include\libxml2" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


