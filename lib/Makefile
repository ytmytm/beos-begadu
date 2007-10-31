
prefix = /boot/home/config
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include

# increased with ABI changes
VERSION_MAJOR=3
# increased with API changes
VERSION_MINOR=5

CC = gcc 
CFLAGS =  -Wall -I/boot/home/config/include -I. -I..
LIBS =  -L/boot/home/config/lib
# -lssl -lcrypto 

MAKE = /boot/home/config/bin/make
AR = ar
INSTALL = /boot/home/config/bin/install -c
RANLIB = ranlib
STRIP = strip
LN_S = ln -s

OBJS = common.o libgadu.o pubdir.o http.o dcc.o events.o pubdir50.o obsolete.o
LOBJS = common.lo libgadu.lo pubdir.lo http.lo dcc.lo events.lo pubdir50.lo obsolete.lo
SRCS = $(OBJS:.o=.c)

SHAREDMM = libgadu.so
SHAREDM = $(SHAREDMM).$(VERSION_MAJOR)
SHARED = $(SHAREDM).$(VERSION_MINOR)
STATIC = libgadu.a

all:	static 

#

clean:
	rm -f *.o *.lo *.a *.so *.so.* *~ core .depend

distclean:	clean
	rm -f Makefile libgadu-config.h libgadu-stdint.h libgadu.pc

#

static: dep $(STATIC)

$(STATIC): $(OBJS)
	$(AR) r $(STATIC) $(OBJS) 
	$(RANLIB) $(STATIC)

#

%.lo:	%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -fPIC $< -o $@

shared:	dep $(SHARED)

$(SHARED): $(LOBJS)
	$(CC) -shared -Wl,-soname,libgadu.so.$(VERSION_MAJOR) -o $(SHARED) $(LOBJS) $(LIBS) -lc
	$(LN_S) -f $(SHARED) $(SHAREDM)
	$(LN_S) -f $(SHARED) $(SHAREDMM)

#

dep:	.depend

.depend:	$(SRCS)
	$(CC) -MM $(CFLAGS) $(SRCS) 1> .depend

#

install:  install-static

install-common:
	$(INSTALL) -d $(DESTDIR)$(includedir)
	$(INSTALL) -m 644 libgadu.h $(DESTDIR)$(includedir)
	$(INSTALL) -m 644 libgadu-config.h $(DESTDIR)$(includedir)
	if test -f libgadu-stdint.h; then $(INSTALL) -m 644 libgadu-stdint.h $(DESTDIR)$(includedir); fi
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(INSTALL) -d $(DESTDIR)$(libdir)/pkgconfig
	$(INSTALL) -m 644 libgadu.pc $(DESTDIR)$(libdir)/pkgconfig

install-shared:	install-common 
	$(INSTALL) -m 755 $(SHARED) $(DESTDIR)$(libdir)
	cd $(DESTDIR)$(libdir) && $(LN_S) -f $(SHARED) $(SHAREDM)
	cd $(DESTDIR)$(libdir) && $(LN_S) -f $(SHARED) $(SHAREDMM)

install-static:	install-common
	$(INSTALL) -m 755 $(STATIC) $(DESTDIR)$(libdir)

strip-libgadu-so:
	$(STRIP) $(SHARED)

#

ifneq ($(wildcard .depend),)
include .depend
endif

