# what flags you want to pass to the C compiler & linker
AM_CXXFLAGS = -g -std=c++11 -Wall -fPIC

bin_PROGRAMS = homeis

homeis_LDADD = -lpthread -lgnutls -ldl -lxml2 -lrt -lconfig++ -lcurl -lz -lcrypto -lmicrohttpd -lhttpserver -lnettle -lgmp -lhogweed -lidn2 -lp11-kit -ltasn1 -lunistring -lffi



homeis_LDFLAGS=\
-L../lib/libxml2-2.8.0/lib \
-L../lib/libcurl/lib \
-L../lib/libmicrohttpd/lib \
-L../lib/libz \
-L../lib/libssl/lib \
-L../lib/libhttpserver/lib \
-L../lib/gnutls/lib \
-L../lib/nettle/lib \
-L../lib/gmplib/lib \
-L../lib/libconfig/lib

homeis_CPPFLAGS= \
-DUSE_THREADS -DLUA_USE_MKSTEMP -DMEMWATCH \
-I../src_3party/lua-5.2.1/src \
-I../src_3party/lua-5.2.1/src \
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
-I../lib/libssl \
-I../src_3party \
-I../src_3party/linuxcs \
-I../src_3party/srutil \
-I../src_3party/uuid \
-I../src_3party/rapidjson \
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
../src_3party/linuxservice.cpp \
../src_3party/linuxcs/linuxcs.cpp \
../src_3party/lua-5.2.1/src/lapi.c \
../src_3party/lua-5.2.1/src/lauxlib.c \
../src_3party/lua-5.2.1/src/lbaselib.c \
../src_3party/lua-5.2.1/src/lbitlib.c \
../src_3party/lua-5.2.1/src/lcode.c \
../src_3party/lua-5.2.1/src/lcorolib.c \
../src_3party/lua-5.2.1/src/lctype.c \
../src_3party/lua-5.2.1/src/ltm.c \
../src_3party/lua-5.2.1/src/lmathlib.c \
../src_3party/lua-5.2.1/src/ldblib.c \
../src_3party/lua-5.2.1/src/ldebug.c \
../src_3party/lua-5.2.1/src/ldo.c \
../src_3party/lua-5.2.1/src/ldump.c \
../src_3party/lua-5.2.1/src/lfunc.c \
../src_3party/lua-5.2.1/src/lgc.c \
../src_3party/lua-5.2.1/src/linit.c \
../src_3party/lua-5.2.1/src/liolib.c \
../src_3party/lua-5.2.1/src/llex.c \
../src_3party/lua-5.2.1/src/lmem.c \
../src_3party/lua-5.2.1/src/loadlib.c \
../src_3party/lua-5.2.1/src/lobject.c \
../src_3party/lua-5.2.1/src/lopcodes.c \
../src_3party/lua-5.2.1/src/loslib.c \
../src_3party/lua-5.2.1/src/lparser.c \
../src_3party/lua-5.2.1/src/lstate.c \
../src_3party/lua-5.2.1/src/lstring.c \
../src_3party/lua-5.2.1/src/lstrlib.c \
../src_3party/lua-5.2.1/src/ltable.c \
../src_3party/lua-5.2.1/src/ltablib.c \
../src_3party/lua-5.2.1/src/lua.c \
../src_3party/lua-5.2.1/src/luac.c \
../src_3party/lua-5.2.1/src/lundump.c \
../src_3party/lua-5.2.1/src/lvm.c \
../src_3party/lua-5.2.1/src/lzio.c \
../src_3party/uuid/clear.c \
../src_3party/uuid/compare.c \
../src_3party/uuid/copy.c \
../src_3party/uuid/gen_uuid.c \
../src_3party/uuid/isnull.c \
../src_3party/uuid/pack.c \
../src_3party/uuid/parse.c \
../src_3party/uuid/tst_uuid.c \
../src_3party/uuid/unpack.c \
../src_3party/uuid/unparse.c \
../src_3party/uuid/uuid_time.c \
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
Auth/UserManager.cpp \
Auth/Session.cpp \
Auth/User.cpp \
Auth/Users.cpp \
Devices/Folder/HisDevFolder.cpp \
Devices/Folder/HisDevFolderRoot.cpp \
Devices/Folder/HisDevValueId.cpp \
Devices/Folder/HisExpressionIdHolder.cpp \
Devices/Handlers/BinnaryInputHandler.cpp \
Devices/Handlers/BinnaryOutputHandler.cpp \
Devices/Handlers/LOW_deviceID.cpp \
Devices/Handlers/LOW_deviceIDRaw.cpp \
Devices/Handlers/LOW_helper_crc.cpp \
Devices/Handlers/OneWireHandler.cpp \
Devices/Handlers/RFIDHandler.cpp \
Devices/Handlers/ModbusHandlers.cpp \
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

