################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Websocket\ Server/WebsocketServer/src/EchoService.cpp \
../Websocket\ Server/WebsocketServer/src/GroupCommunicationService.cpp \
../Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.cpp \
../Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.cpp \
../Websocket\ Server/WebsocketServer/src/WebsocketClient.cpp \
../Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.cpp \
../Websocket\ Server/WebsocketServer/src/WebsocketServer.cpp \
../Websocket\ Server/WebsocketServer/src/WebsocketService.cpp \
../Websocket\ Server/WebsocketServer/src/stdafx.cpp 

OBJS += \
./Websocket\ Server/WebsocketServer/src/EchoService.o \
./Websocket\ Server/WebsocketServer/src/GroupCommunicationService.o \
./Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.o \
./Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.o \
./Websocket\ Server/WebsocketServer/src/WebsocketClient.o \
./Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.o \
./Websocket\ Server/WebsocketServer/src/WebsocketServer.o \
./Websocket\ Server/WebsocketServer/src/WebsocketService.o \
./Websocket\ Server/WebsocketServer/src/stdafx.o 

CPP_DEPS += \
./Websocket\ Server/WebsocketServer/src/EchoService.d \
./Websocket\ Server/WebsocketServer/src/GroupCommunicationService.d \
./Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.d \
./Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.d \
./Websocket\ Server/WebsocketServer/src/WebsocketClient.d \
./Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.d \
./Websocket\ Server/WebsocketServer/src/WebsocketServer.d \
./Websocket\ Server/WebsocketServer/src/WebsocketService.d \
./Websocket\ Server/WebsocketServer/src/stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
Websocket\ Server/WebsocketServer/src/EchoService.o: ../Websocket\ Server/WebsocketServer/src/EchoService.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/EchoService.d" -MT"Websocket\ Server/WebsocketServer/src/EchoService.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/GroupCommunicationService.o: ../Websocket\ Server/WebsocketServer/src/GroupCommunicationService.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/GroupCommunicationService.d" -MT"Websocket\ Server/WebsocketServer/src/GroupCommunicationService.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.o: ../Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/RoutedCommunicationService.d" -MT"Websocket\ Server/WebsocketServer/src/RoutedCommunicationService.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.o: ../Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/StreamedCommunicationService.d" -MT"Websocket\ Server/WebsocketServer/src/StreamedCommunicationService.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/WebsocketClient.o: ../Websocket\ Server/WebsocketServer/src/WebsocketClient.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/WebsocketClient.d" -MT"Websocket\ Server/WebsocketServer/src/WebsocketClient.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.o: ../Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/WebsocketClientFactory.d" -MT"Websocket\ Server/WebsocketServer/src/WebsocketClientFactory.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/WebsocketServer.o: ../Websocket\ Server/WebsocketServer/src/WebsocketServer.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/WebsocketServer.d" -MT"Websocket\ Server/WebsocketServer/src/WebsocketServer.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/WebsocketService.o: ../Websocket\ Server/WebsocketServer/src/WebsocketService.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/WebsocketService.d" -MT"Websocket\ Server/WebsocketServer/src/WebsocketService.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Websocket\ Server/WebsocketServer/src/stdafx.o: ../Websocket\ Server/WebsocketServer/src/stdafx.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude-fixed -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/lib/gcc/arm-linux-gnueabihf/4.7.2/finclude -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/usr/include -I/root/raspberrypi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include -I"/root/raspiworkspace/dis/PushFramework/include" -I"/root/raspiworkspace/dis/PushFramework/private" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketServer/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/sha1" -I"/root/raspiworkspace/dis/Websocket Server/WebsocketProtocol/src/base64" -I"/root/raspiworkspace/dis/rapidjson" -I"/root/raspiworkspace/dis/Services" -I"/root/raspiworkspace/dis" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/threadOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/componentOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/linkOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/deviceOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/platformOW++" -I"/root/raspiworkspace/dis/onewirelibrary/OneWire++/portOW++" -I"/root/raspiworkspace/dis/onewirelibrary" -I/usr/lib -I/root/raspberrypi/libxml2-2.9.0/include/libxml2 -I"/root/raspiworkspace/dis/homeis/Devices" -I"/root/raspiworkspace/dis/homeis/Helpers" -O0 -g3 -pedantic -Wall -c -std=c++0x -g -MMD -MP -MF"Websocket Server/WebsocketServer/src/stdafx.d" -MT"Websocket\ Server/WebsocketServer/src/stdafx.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


