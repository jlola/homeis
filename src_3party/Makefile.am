# what flags you want to pass to the C compiler & linker
AM_CXXFLAGS=-std=c++0x
AM_LDFLAGS = -ldl -lpthread

noinst_LTLIBRARIES = libsrc3party.la

libsrc3party_la_CFLAGS= \
-I./lua-5.2.1/src

libsrc3party_la_CPPFLAGS= \
-I./ \
-I./linuxcs \
-I./srutil \
-I./uuid \
-I./rapidjson \
-I./libcrash-master \
-I./gtest_src \
-I./FakeIt

libsrc3party_la_SOURCES= \
./linuxservice.cpp \
./linuxcs/linuxcs.cpp \
./lua-5.2.1/src/lapi.c \
./lua-5.2.1/src/lauxlib.c \
./lua-5.2.1/src/lbaselib.c \
./lua-5.2.1/src/lbitlib.c \
./lua-5.2.1/src/lcode.c \
./lua-5.2.1/src/lcorolib.c \
./lua-5.2.1/src/lctype.c \
./lua-5.2.1/src/ldblib.c \
./lua-5.2.1/src/ldebug.c \
./lua-5.2.1/src/ldo.c \
./lua-5.2.1/src/ldump.c \
./lua-5.2.1/src/lfunc.c \
./lua-5.2.1/src/lgc.c \
./lua-5.2.1/src/linit.c \
./lua-5.2.1/src/liolib.c \
./lua-5.2.1/src/llex.c \
./lua-5.2.1/src/lmem.c \
./lua-5.2.1/src/loadlib.c \
./lua-5.2.1/src/lobject.c \
./lua-5.2.1/src/lopcodes.c \
./lua-5.2.1/src/loslib.c \
./lua-5.2.1/src/lparser.c \
./lua-5.2.1/src/lstate.c \
./lua-5.2.1/src/lstring.c \
./lua-5.2.1/src/lstrlib.c \
./lua-5.2.1/src/ltable.c \
./lua-5.2.1/src/ltablib.c \
./lua-5.2.1/src/lua.c \
./lua-5.2.1/src/luac.c \
./lua-5.2.1/src/lundump.c \
./lua-5.2.1/src/lvm.c \
./lua-5.2.1/src/lzio.c \
./uuid/clear.c \
./uuid/compare.c \
./uuid/copy.c \
./uuid/gen_uuid.c \
./uuid/isnull.c \
./uuid/pack.c \
./uuid/parse.c \
./uuid/tst_uuid.c \
./uuid/unpack.c \
./uuid/unparse.c \
./uuid/uuid_time.c


