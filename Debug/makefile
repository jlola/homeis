################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include uuid/subdir.mk
-include onewirelibrary/subdir.mk
-include onewirelibrary/OneWire++/threadOW++/subdir.mk
-include onewirelibrary/OneWire++/portOW++/subdir.mk
-include onewirelibrary/OneWire++/platformOW++/subdir.mk
-include onewirelibrary/OneWire++/linkOW++/subdir.mk
-include onewirelibrary/OneWire++/deviceOW++/subdir.mk
-include onewirelibrary/OneWire++/componentOW++/subdir.mk
-include onewirelibrary/OneWire++/subdir.mk
-include lua-5.2.1/src/subdir.mk
-include linuxcs/subdir.mk
-include libhttpserver-master/src/subdir.mk
-include libhttpserver-master/examples/subdir.mk
-include libcrash-master/subdir.mk
-include homeis/subdir.mk
-include homeis/Tags/subdir.mk
-include homeis/Services/subdir.mk
-include homeis/Helpers/subdir.mk
-include homeis/Expressions/subdir.mk
-include homeis/Devices/VirtualDevices/subdir.mk
-include homeis/Devices/subdir.mk
-include homeis/Devices/Folder/subdir.mk
-include homeis/Common/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: dis

# Tool invocations
dis: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	arm-linux-gnueabihf-g++ -L"/root/raspiworkspace/dis/libs/libusbcompat/lib" -L"/root/raspiworkspace/dis/libs/libusb/lib" -L/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf -L/root/raspberrypi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/arm-bcm2708hardfp-linux-gnueabi/lib -L/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/lib -L/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2 -L/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/libexec/gcc/arm-linux-gnueabihf/4.7.2 -L/root/raspberrypi/libmicrohttpd/lib -L/root/raspberrypi/libxml2-2.8.0/lib -march=armv7-a -rdynamic -lrt -Xlinker -lrt -p -pg -o"dis" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) dis
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
