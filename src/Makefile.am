# what flags you want to pass to the C compiler & linker
AM_CXXFLAGS=-std=c++0x
AM_LDFLAGS = -ldl -lpthread -lxml2 -lcurl -lz

bin_PROGRAMS = homeis
homeis_LDADD = ../src_3party/libsrc3party.la

homeis_LDFLAGS=\
-L../lib/libxml2-2.8.0/lib \
-L../lib/libcurl/lib 

homeis_CPPFLAGS= \
-I../lib/libxml2-2.8.0/include/libxml2 \
-I../lib/libcurl/include \
-I../lib/libssl/include \
-I../lib/libz/include \
-I../lib/nettle/include \
-I../lib/gmplib/include \
-I../lib/gnutls/include \
-I../lib/libconfig/include \
-I../lib/libmicrohttpd/include \
-I../lib/libhttpserver/include \
-I../src_3party \
-I../src_3party/linuxcs \
-I../src_3party/srutil \
-I../src_3party/uuid \
-I../src_3party/rapidjson \
-I../src_3party/lua-5.2.1/src \
-I../src_3party/libcrash-master \
-I../src_3party/gtest_src \
-I../src_3party/FakeIt \
-I./Auth \
-I./Common \
-I./Devices \
-I./Devices/Folder \
-I./Devices/Handlers \
-I./Devices/Values \
-I./Devices/VirtualDevices \
-I./Drivers \
-I./Expressions \
-I./Helpers \
-I./Modbus \
-I./Modbus/Listeners \
-I./Services \
-I./Tags

homeis_SOURCES= \
Helpers/logger.cpp \
../src_3party/death_handler.cpp \
Helpers/File.cpp \
Helpers/Directory.cpp \
Common/CUUID.cpp \
Common/Base64.cpp \
Common/DateTime.cpp \
Common/HisBase.cpp \
Common/HisLock.cpp \
Common/HisCollectionBase.hpp \
Devices/Folder/HisDevFolder.cpp \
Devices/Folder/HisDevFolderRoot.cpp \
Devices/Folder/HisDevValueId.cpp \
Devices/Folder/HisExpressionIdHolder.cpp \
Devices/Handlers/BinnaryInputHandler.cpp \
Devices/Handlers/LOW_deviceID.cpp \
Devices/Handlers/LOW_deviceIDRaw.cpp \
Devices/Handlers/LOW_helper_crc.cpp \
Devices/Handlers/ModbusHandlers.cpp \
Devices/Handlers/OneWireHandler.cpp \
Devices/Handlers/RFIDHandler.cpp \
Devices/Values/HisDevValue.cpp \
Devices/Values/HisDevValueBase.cpp \
Devices/Values/HisDevValueEmail.cpp \
Devices/VirtualDevices/HisDevVirtual.cpp \
Devices/HisDevBase.cpp \
Devices/HisDevFactory.cpp \
Devices/HisDevices.cpp \
Devices/HisDevModbus.cpp \
Devices/HisDevRunTime.cpp \
Devices/ValueEventArgs.cpp \
Drivers/EmailSender.cpp \
Expressions/ExpressionRuntime.cpp \
Expressions/LuaExpression.cpp \
Expressions/LuaExpressionManager.cpp \
Modbus/Listeners/DeviceRefreshTimer.cpp \
Modbus/Listeners/ModbusAlarmListener.cpp \
Modbus/Listeners/ModbusProcessor.cpp \
Modbus/crc.cpp \
Modbus/ModbusProvider.cpp \
Modbus/ModbusSimulator.cpp \
Modbus/ModifiedMdbus.cpp \
Modbus/SerialPort.cpp \
Modbus/Exception.hpp \
Modbus/SerialPort.hpp \
Services/ConnectorsService.cpp \
Services/DevicesService.cpp \
Services/ExpressionsService.cpp \
Services/FileController.cpp \
Services/FoldersService.cpp \
Services/HttpHeadersProvider.cpp \
Services/LogInService.cpp \
Services/LogService.cpp \
Services/ModbusDeviceService.cpp \
Services/ModbusService.cpp \
Services/ServiceBase.cpp \
Services/UsersService.cpp \
Services/VersionService.cpp \
Tags/TagBase.cpp \
HomeIsConfig.cpp \
HomeIsServer.cpp \
SignalTryCatch.cpp \
main.cpp
