####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = liblinkOW++.la


INCLUDES = $(all_includes)


LDFLAGS =          


liblinkOW___la_LDFLAGS = -version-info 0:0:0

liblinkOW___la_METASOURCES = AUTO

liblinkOW___la_SOURCES = LOW_linkDS2490.cpp LOW_linkFlexibleSpeed.cpp LOW_linkPassiveSerial.cpp LOW_linkDS2480B.cpp LOW_link.cpp 


EXTRA_DIST = LOW_link.cpp LOW_link.h LOW_linkDS2480B.cpp LOW_linkDS2480B.h LOW_linkPassiveSerial.cpp LOW_linkPassiveSerial.h LOW_linkFlexibleSpeed.cpp LOW_linkFlexibleSpeed.h LOW_linkDS2490.cpp LOW_linkDS2490.h 

install-data-local:
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_link.h $(includedir)/OneWire++/LOW_link.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_linkDS2480B.h $(includedir)/OneWire++/LOW_linkDS2480B.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_linkPassiveSerial.h $(includedir)/OneWire++/LOW_linkPassiveSerial.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_linkFlexibleSpeed.h $(includedir)/OneWire++/LOW_linkFlexibleSpeed.h
	$(mkinstalldirs) $(includedir)/OneWire++/
	$(INSTALL_DATA) $(srcdir)/LOW_linkDS2490.h $(includedir)/OneWire++/LOW_linkDS2490.h

uninstall-local:
	-rm -f $(includedir)/OneWire++/LOW_link.h
	-rm -f $(includedir)/OneWire++/LOW_linkDS2480B.h
	-rm -f $(includedir)/OneWire++/LOW_linkPassiveSerial.h
	-rm -f $(includedir)/OneWire++/LOW_linkFlexibleSpeed.h
	-rm -f $(includedir)/OneWire++/LOW_linkDS2490.h

####### kdevelop will overwrite this part!!! (end)############
