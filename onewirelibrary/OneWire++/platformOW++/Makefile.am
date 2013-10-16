####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libplatformOW++.la


INCLUDES = $(all_includes)


LDFLAGS =          


libplatformOW___la_LDFLAGS = -version-info 0:0:0

libplatformOW___la_METASOURCES = AUTO

libplatformOW___la_SOURCES = LOW_platformMisc.cpp LOW_platformMisc_Linux.cpp LOW_platformMiscFactory.cpp 


EXTRA_DIST = LOW_platformMiscFactory.cpp LOW_platformMiscFactory.h LOW_platformMisc_Linux.cpp LOW_platformMisc_Linux.h LOW_platformMisc.cpp LOW_platformMisc.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_platformMiscFactory.h $(includedir)/OneWire++/LOW_platformMiscFactory.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_platformMisc_Linux.h $(includedir)/OneWire++/LOW_platformMisc_Linux.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_platformMisc.h $(includedir)/OneWire++/LOW_platformMisc.h

uninstall-local:
	-rm -f $(includedir)/OneWire++/LOW_platformMiscFactory.h
	-rm -f $(includedir)/OneWire++/LOW_platformMisc_Linux.h
	-rm -f $(includedir)/OneWire++/LOW_platformMisc.h

####### kdevelop will overwrite this part!!! (end)############
