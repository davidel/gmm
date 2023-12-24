
INCLUDES = -I. -I../include

lib_LTLIBRARIES = libgmm.la
libgmm_la_SOURCES = gmm.c gmm_aux.c
libgmm_la_LIBADD = $(DL_LIB)
libgmm_la_LDFLAGS = -module -avoid-version
libgmm_la_CFLAGS = -Wstrict-prototypes -pipe -O0

