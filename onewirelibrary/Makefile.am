####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = onewirelibrary

## INCLUDES were found outside kdevelop specific part

onewirelibrary_SOURCES = test_ds2890.cpp test_msgLogging.cpp test_lock.cpp test_thread.cpp test_link.cpp test_switch.cpp test_ds2406.cpp test_jalousie.cpp test_ds2405.cpp test_net.cpp helper.cpp test_temp.cpp main.cpp 
onewirelibrary_LDADD   = ./OneWire++/libOneWire++.la ./OneWire++/platformOW++/libplatformOW++.la ./OneWire++/deviceOW++/libdeviceOW++.la ./OneWire++/componentOW++/libcomponentOW++.la ./OneWire++/threadOW++/libthreadOW++.la ./OneWire++/linkOW++/liblinkOW++.la ./OneWire++/portOW++/libportOW++.la 

SUBDIRS = docs OneWire++ 

EXTRA_DIST = main.cpp test_temp.cpp test_temp.h helper.h helper.cpp test_net.h test_net.cpp test_ds2405.h test_ds2405.cpp test_jalousie.h test_jalousie.cpp test_ds2406.cpp test_ds2406.h test_switch.cpp test_switch.h test_link.cpp test_link.h test_thread.cpp test_thread.h test_lock.h test_lock.cpp test_msgLogging.cpp test_msgLogging.h test_ds2890.h test_ds2890.cpp 

####### kdevelop will overwrite this part!!! (end)############
# set the include path found by configure
INCLUDES= $(all_includes)

#
# (HR): force library order
#
#onewirelibrary_LDADD   = ./OneWire++/deviceOW++/libdeviceOW++.a ./OneWire++/componentOW++/libcomponentOW++.a ./OneWire++/libOneWire++.a ./OneWire++/platformOW++/libplatformOW++.a 
 
# the library search path.
onewirelibrary_LDFLAGS = $(all_libraries) 
                                                                                
