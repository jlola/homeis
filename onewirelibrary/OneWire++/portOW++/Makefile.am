####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libportOW++.la


INCLUDES = $(all_includes)


LDFLAGS =          


libportOW___la_LDFLAGS = -version-info 0:0:0

libportOW___la_METASOURCES = AUTO

libportOW___la_SOURCES = LOW_portUsbDevice_Linux.cpp LOW_portUsb_Factory.cpp LOW_portUsbDevice.cpp LOW_portSerialFactory.cpp LOW_portSerial_Linux.cpp LOW_portSerial.cpp 


EXTRA_DIST = LOW_portSerial.cpp LOW_portSerial.h LOW_portSerial_Linux.cpp LOW_portSerial_Linux.h LOW_portSerialFactory.cpp LOW_portSerialFactory.h LOW_portUsbDevice.cpp LOW_portUsbDevice.h LOW_portUsb_Factory.cpp LOW_portUsb_Factory.h LOW_portUsbDevice_Linux.cpp LOW_portUsbDevice_Linux.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portSerial.h $(includedir)/OneWire++/LOW_portSerial.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portSerial_Linux.h $(includedir)/OneWire++/LOW_portSerial_Linux.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portSerialFactory.h $(includedir)/OneWire++/LOW_portSerialFactory.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portUsbDevice.h $(includedir)/OneWire++/LOW_portUsbDevice.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portUsb_Factory.h $(includedir)/OneWire++/LOW_portUsb_Factory.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_portUsbDevice_Linux.h $(includedir)/OneWire++/LOW_portUsbDevice_Linux.h

uninstall-local:
	-rm -f $(includedir)/OneWire++/LOW_portSerial.h
	-rm -f $(includedir)/OneWire++/LOW_portSerial_Linux.h
	-rm -f $(includedir)/OneWire++/LOW_portSerialFactory.h
	-rm -f $(includedir)/OneWire++/LOW_portUsbDevice.h
	-rm -f $(includedir)/OneWire++/LOW_portUsb_Factory.h
	-rm -f $(includedir)/OneWire++/LOW_portUsbDevice_Linux.h

####### kdevelop will overwrite this part!!! (end)############
