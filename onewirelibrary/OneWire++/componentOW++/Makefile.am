####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libcomponentOW++.la


INCLUDES = $(all_includes)


LDFLAGS =          


libcomponentOW___la_LDFLAGS = -version-info 0:0:0

libcomponentOW___la_METASOURCES = AUTO

libcomponentOW___la_SOURCES = LOW_compToggleSwitch.cpp LOW_compTwinSwitch.cpp LOW_compJalousieController.cpp LOW_compTempSensor.cpp 


EXTRA_DIST = LOW_compTempSensor.cpp LOW_compTempSensor.h LOW_compJalousieController.cpp LOW_compJalousieController.h LOW_compTwinSwitch.cpp LOW_compTwinSwitch.h LOW_compToggleSwitch.cpp LOW_compToggleSwitch.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_compTempSensor.h $(includedir)/OneWire++/LOW_compTempSensor.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_compJalousieController.h $(includedir)/OneWire++/LOW_compJalousieController.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_compTwinSwitch.h $(includedir)/OneWire++/LOW_compTwinSwitch.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_compToggleSwitch.h $(includedir)/OneWire++/LOW_compToggleSwitch.h

uninstall-local:
	-rm -f $(includedir)/OneWire++/LOW_compTempSensor.h
	-rm -f $(includedir)/OneWire++/LOW_compJalousieController.h
	-rm -f $(includedir)/OneWire++/LOW_compTwinSwitch.h
	-rm -f $(includedir)/OneWire++/LOW_compToggleSwitch.h

####### kdevelop will overwrite this part!!! (end)############
