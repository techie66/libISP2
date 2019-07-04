EXTRA_PROGRAMS = isp2_example isp2_example_dyn
isp2_example_SOURCES = isp2.cpp …
isp2_example_dyn_SOURCES = isp2.cpp …
isp2_example_LDADD = ../libisp2.la
isp2_example_dyn_LDADD = -lisp2
example:
	$(MAKE) $(AM_MAKEFLAGS) -C .. all
	$(MAKE) $(AM_MAKEFLAGS) isp2_example
	$(MAKE) $(AM_MAKEFLAGS) isp2_example_dyn
