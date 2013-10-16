####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libdeviceOW++.la


INCLUDES = $(all_includes)


LDFLAGS =          

libdeviceOW___la_METASOURCES = AUTO

libdeviceOW___la_SOURCES = LOW_devDS2890.cpp LOW_devUnknown.cpp LOW_devDS2401.cpp LOW_devDS2406.cpp LOW_devDS2405.cpp LOW_deviceFactory.cpp LOW_devDS1820.cpp 


EXTRA_DIST = LOW_devDS1820.h LOW_devDS1820.cpp LOW_deviceFactory.cpp LOW_deviceFactory.h LOW_devDS2405.cpp LOW_devDS2405.h LOW_devDS2406.cpp LOW_devDS2406.h LOW_devDS2401.cpp LOW_devDS2401.h LOW_devUnknown.cpp LOW_devUnknown.h LOW_devDS2890.cpp LOW_devDS2890.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_devDS1820.h $(includedir)/OneWire++/LOW_devDS1820.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_deviceFactory.h $(includedir)/OneWire++/LOW_deviceFactory.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_devDS2405.h $(includedir)/OneWire++/LOW_devDS2405.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_devDS2406.h $(includedir)/OneWire++/LOW_devDS2406.h

uninstall-local:
	-rm -f $(includedir)/OneWire++/LOW_devDS1820.h
	-rm -f $(includedir)/OneWire++/LOW_deviceFactory.h
	-rm -f $(includedir)/OneWire++/LOW_devDS2405.h
	-rm -f $(includedir)/OneWire++/LOW_devDS2406.h

####### kdevelop will overwrite this part!!! (end)############
