################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include uuid/subdir.mk
-include src/Tags/subdir.mk
-include src/Services/subdir.mk
-include src/Helpers/subdir.mk
-include src/Expressions/subdir.mk
-include src/Devices/VirtualDevices/subdir.mk
-include src/Devices/Folder/subdir.mk
-include src/Devices/subdir.mk
-include src/Common/subdir.mk
-include src/subdir.mk
-include onewirelibrary/OneWire++/threadOW++/subdir.mk
-include onewirelibrary/OneWire++/portOW++/subdir.mk
-include onewirelibrary/OneWire++/platformOW++/subdir.mk
-include onewirelibrary/OneWire++/linkOW++/subdir.mk
-include onewirelibrary/OneWire++/deviceOW++/subdir.mk
-include onewirelibrary/OneWire++/componentOW++/subdir.mk
-include onewirelibrary/OneWire++/subdir.mk
-include onewirelibrary/subdir.mk
-include lua-5.2.1/src/subdir.mk
-include linuxcs/subdir.mk
-include libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/tutorial/subdir.mk
-include libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/html/subdir.mk
-include libs/libsources/libxml2-2.8.0/share/doc/libxml2-2.8.0/examples/subdir.mk
-include libs/libsources/libmicrohttpd/src/examples/subdir.mk
-include libs/libsources/libmicrohttpd/src/daemon/.libs/subdir.mk
-include libs/libsources/libmicrohttpd/src/daemon/subdir.mk
-include libs/libsources/libmicrohttpd/doc/examples/subdir.mk
-include libhttpserver-master/test/unit/subdir.mk
-include libhttpserver-master/test/integ/subdir.mk
-include libhttpserver-master/test/subdir.mk
-include libhttpserver-master/src/details/subdir.mk
-include libhttpserver-master/src/subdir.mk
-include libhttpserver-master/examples/subdir.mk
-include libcrash-master/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
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
	g++ -L"C:\Users\pc\git\homeis\lua-5.2.1\src" -o "dis" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) dis
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
