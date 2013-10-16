################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Websocket\ Server/WebsocketProtocol/src/sha1/sha.cpp \
../Websocket\ Server/WebsocketProtocol/src/sha1/sha1.cpp \
../Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.cpp \
../Websocket\ Server/WebsocketProtocol/src/sha1/shatest.cpp 

OBJS += \
./Websocket\ Server/WebsocketProtocol/src/sha1/sha.o \
./Websocket\ Server/WebsocketProtocol/src/sha1/sha1.o \
./Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.o \
./Websocket\ Server/WebsocketProtocol/src/sha1/shatest.o 

CPP_DEPS += \
./Websocket\ Server/WebsocketProtocol/src/sha1/sha.d \
./Websocket\ Server/WebsocketProtocol/src/sha1/sha1.d \
./Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.d \
./Websocket\ Server/WebsocketProtocol/src/sha1/shatest.d 


# Each subdirectory must supply rules for building sources it contributes
Websocket\ Server/WebsocketProtocol/src/sha1/sha.o: ../Websocket\ Server/WebsocketProtocol/src/sha1/sha.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketProtocol/src/sha1/sha.d" -MT"Websocket\ Server/WebsocketProtocol/src/sha1/sha.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketProtocol/src/sha1/sha1.o: ../Websocket\ Server/WebsocketProtocol/src/sha1/sha1.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketProtocol/src/sha1/sha1.d" -MT"Websocket\ Server/WebsocketProtocol/src/sha1/sha1.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.o: ../Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketProtocol/src/sha1/shacmp.d" -MT"Websocket\ Server/WebsocketProtocol/src/sha1/shacmp.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketProtocol/src/sha1/shatest.o: ../Websocket\ Server/WebsocketProtocol/src/sha1/shatest.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketProtocol/src/sha1/shatest.d" -MT"Websocket\ Server/WebsocketProtocol/src/sha1/shatest.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


