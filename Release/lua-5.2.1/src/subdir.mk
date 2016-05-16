################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lua-5.2.1/src/lapi.c \
../lua-5.2.1/src/lauxlib.c \
../lua-5.2.1/src/lbaselib.c \
../lua-5.2.1/src/lbitlib.c \
../lua-5.2.1/src/lcode.c \
../lua-5.2.1/src/lcorolib.c \
../lua-5.2.1/src/lctype.c \
../lua-5.2.1/src/ldblib.c \
../lua-5.2.1/src/ldebug.c \
../lua-5.2.1/src/ldo.c \
../lua-5.2.1/src/ldump.c \
../lua-5.2.1/src/lfunc.c \
../lua-5.2.1/src/lgc.c \
../lua-5.2.1/src/linit.c \
../lua-5.2.1/src/liolib.c \
../lua-5.2.1/src/llex.c \
../lua-5.2.1/src/lmathlib.c \
../lua-5.2.1/src/lmem.c \
../lua-5.2.1/src/loadlib.c \
../lua-5.2.1/src/lobject.c \
../lua-5.2.1/src/lopcodes.c \
../lua-5.2.1/src/loslib.c \
../lua-5.2.1/src/lparser.c \
../lua-5.2.1/src/lstate.c \
../lua-5.2.1/src/lstring.c \
../lua-5.2.1/src/lstrlib.c \
../lua-5.2.1/src/ltable.c \
../lua-5.2.1/src/ltablib.c \
../lua-5.2.1/src/ltm.c \
../lua-5.2.1/src/lua.c \
../lua-5.2.1/src/luac.c \
../lua-5.2.1/src/lundump.c \
../lua-5.2.1/src/lvm.c \
../lua-5.2.1/src/lzio.c 

OBJS += \
./lua-5.2.1/src/lapi.o \
./lua-5.2.1/src/lauxlib.o \
./lua-5.2.1/src/lbaselib.o \
./lua-5.2.1/src/lbitlib.o \
./lua-5.2.1/src/lcode.o \
./lua-5.2.1/src/lcorolib.o \
./lua-5.2.1/src/lctype.o \
./lua-5.2.1/src/ldblib.o \
./lua-5.2.1/src/ldebug.o \
./lua-5.2.1/src/ldo.o \
./lua-5.2.1/src/ldump.o \
./lua-5.2.1/src/lfunc.o \
./lua-5.2.1/src/lgc.o \
./lua-5.2.1/src/linit.o \
./lua-5.2.1/src/liolib.o \
./lua-5.2.1/src/llex.o \
./lua-5.2.1/src/lmathlib.o \
./lua-5.2.1/src/lmem.o \
./lua-5.2.1/src/loadlib.o \
./lua-5.2.1/src/lobject.o \
./lua-5.2.1/src/lopcodes.o \
./lua-5.2.1/src/loslib.o \
./lua-5.2.1/src/lparser.o \
./lua-5.2.1/src/lstate.o \
./lua-5.2.1/src/lstring.o \
./lua-5.2.1/src/lstrlib.o \
./lua-5.2.1/src/ltable.o \
./lua-5.2.1/src/ltablib.o \
./lua-5.2.1/src/ltm.o \
./lua-5.2.1/src/lua.o \
./lua-5.2.1/src/luac.o \
./lua-5.2.1/src/lundump.o \
./lua-5.2.1/src/lvm.o \
./lua-5.2.1/src/lzio.o 

C_DEPS += \
./lua-5.2.1/src/lapi.d \
./lua-5.2.1/src/lauxlib.d \
./lua-5.2.1/src/lbaselib.d \
./lua-5.2.1/src/lbitlib.d \
./lua-5.2.1/src/lcode.d \
./lua-5.2.1/src/lcorolib.d \
./lua-5.2.1/src/lctype.d \
./lua-5.2.1/src/ldblib.d \
./lua-5.2.1/src/ldebug.d \
./lua-5.2.1/src/ldo.d \
./lua-5.2.1/src/ldump.d \
./lua-5.2.1/src/lfunc.d \
./lua-5.2.1/src/lgc.d \
./lua-5.2.1/src/linit.d \
./lua-5.2.1/src/liolib.d \
./lua-5.2.1/src/llex.d \
./lua-5.2.1/src/lmathlib.d \
./lua-5.2.1/src/lmem.d \
./lua-5.2.1/src/loadlib.d \
./lua-5.2.1/src/lobject.d \
./lua-5.2.1/src/lopcodes.d \
./lua-5.2.1/src/loslib.d \
./lua-5.2.1/src/lparser.d \
./lua-5.2.1/src/lstate.d \
./lua-5.2.1/src/lstring.d \
./lua-5.2.1/src/lstrlib.d \
./lua-5.2.1/src/ltable.d \
./lua-5.2.1/src/ltablib.d \
./lua-5.2.1/src/ltm.d \
./lua-5.2.1/src/lua.d \
./lua-5.2.1/src/luac.d \
./lua-5.2.1/src/lundump.d \
./lua-5.2.1/src/lvm.d \
./lua-5.2.1/src/lzio.d 


# Each subdirectory must supply rules for building sources it contributes
lua-5.2.1/src/%.o: ../lua-5.2.1/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_HTTPSERVER_HPP_INSIDE_ -D_STDINT_H -DHAVE_INTTYPES_H -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/libxml2-2.9.0/lib -I"C:\Users\pc\git\homeis\linuxcs" -I"C:\Users\pc\git\homeis\libhttpserver-master\src\httpserver" -I/root/raspberrypi/libmicrohttpd/include -I"C:\Users\pc\git\homeis\libhttpserver-master\src" -I"C:\Users\pc\git\homeis\uuid" -I"C:\Users\pc\git\homeis" -I"C:\Users\pc\git\homeis\lua-5.2.1\src" -I/usr/include -I/root/libmicrohttpd-0.9.26/src/include -I/root/libxml2-2.8.0/include -I"C:\gcc-linaro-arm-linux-gnueabihf-4.9-2014.09\arm-linux-gnueabihf\libc\usr\include" -I"C:\Users\pc\git\homeis\libs\libsources\libmicrohttpd\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include" -I"C:\Users\pc\git\homeis\libs\libsources\libxml2-2.8.0\include\libxml2\libxml" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


