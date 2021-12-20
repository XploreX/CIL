$(shell cp --no-target-directory -lr $(top_srcdir)/tests/results $(top_builddir)/tests/results)
$(shell cp -l $(top_srcdir)/tests/in.png $(top_builddir)/tests/)
$(shell cp -l $(top_srcdir)/tests/in.jpeg $(top_builddir)/tests/)
$(shell cp -l $(top_srcdir)/tests/in.ppm $(top_builddir)/tests/)

AM_DEFAULT_SOURCE_EXT = .cpp
AM_CPPFLAGS += -I$(top_srcdir)/include

check_LTLIBRARIES = libciltests.la
libciltests_la_SOURCES = main.cpp
libciltests_la_CXXFLAGS = $(AM_CPPFLAGS) $(shell pkg-config --cflags catch2)

LDADD = $(top_builddir)/libcil.la libciltests.la
check_PROGRAMS = crop rotate flip
nobase_check_DATA = in.ppm in.png in.jpeg results/crop-1.jpeg results/crop-1.png results/crop-1.ppm
check_CXXFLAGS = $(AM_CPPFLAGS) $(shell pkg-config --cflags catch2)

TESTS = $(check_PROGRAMS)
