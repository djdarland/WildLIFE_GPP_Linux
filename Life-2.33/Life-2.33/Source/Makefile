# Generated automatically from Makefile.in by configure.
# DIST: This is the distribution Makefile for WILD Life.  configure
# DIST: can make most of the changes to this file you might want, so
# DIST: try that first.

SHELL		= /bin/sh

CC		= gcc
CFLAGS		= -g -O
CPPFLAGS	= 
LDFLAGS		= 
LIBS		=  -lm
# DEFS		=  -DNEEDXLIBINT=1 -DGARBCOL=1 -DALLOC_WORDS=20971520 -DUNIF_DEBUG=1 -DNORAW=1  
# DEFS		=  -DX11=1 -DNEEDXLIBINT=1 -DGARBCOL=1 -DALLOC_WORDS=20971520 -DUNIF_DEBUG=1 -DNORAW=1 
RANLIB		= ranlib
INSTALL		= /usr/bin/install -c
MKDIRHIER	= mkdirhier
prefix		= ~/life_local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
libdir		= ${prefix}/lib/life
srcdir		= .
X_CFLAGS	= 
X_LIBS		=  -L/usr/lib64
X_EXTRA_LIBS	= 
# X_PRE_LIBS	=  -lSM -lICE
X_PRE_LIBS      =
XLIBS		= ${X_PRE_LIBS} ${X_LIBS} -lX11 ${X_EXTRA_LIBS}
XLIBRARIES	= ${XLIBS}
MKDEP		= makedepend
SETUPDIR	= ${libdir}
SETUP		= ${prefix}/Source/.set_up
INCLUDE_PATHS	= 
XSOURCE		= ${XSRC}
XOBJECT		= ${XOBJ}
XINCLUDE	= ${XINC}
DBMSOURCE	= 
DBMOBJECT	= 

#=====================================================================
# For Linux:
# The following variable should contain the pathname of the GCC include
# directory.  Run 'gcc -v' to find where gcc's directories are, then edit
# the sample pathname below to be correct.
#INCLUDE_PATHS	= -I/usr/lib/gcc-lib/i[34]86-linux/<version>/include
#
# For Linux (continued):
# This is an example of a definition that works on a Gateway 2000 machine:
#INCLUDE_PATHS = -I/usr/lib/gcc-lib/i486-linux/2.4.5/include -I/usr/include/bsd
#
# For Solaris
#INCLUDE_PATHS = -I/usr/openwin/include
#=====================================================================

#=====================================================================
# -DNORAW
# Add this if the low-level (unbuffered) terminal input file (raw.c) won't
# compile on your system.  On SunOS for some reason this file will include some
# library files twice. This does not seem to make any difference.
#
# -DNEED_TRUNC
# If defined, adds a definition of the 'trunc' function.
# 
# -DX11
# If defined, will cause the X interface to be linked in.  If it is not
# defined then the system will contain no X interface.  -lX11 should be
# removed from LOADFLAGS in that case (see below).
#
# -DGARBCOL
# If defined will cause the garbage collector to be called when memory is low.
#
# -DUNIF_DEBUG
# If defined causes the 'debug' mode to show matching and unification sub-goals.
#=====================================================================

COMPILE.c	= $(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c
XCOMPILE.c	= $(CC) $(CFLAGS) $(X_CFLAGS) $(CPPFLAGS) $(DEFS) -c
.c.o:
	$(COMPILE.c) -o $@ $<

#=====================================================================
# MEMORY is the number of words that the system will allocate.
# Because of the half-space GC, Wild-LIFE will actually allocate twice that
# many words. So on a 32-bit system, multiply by 8 to obtain the real memory
# usage in bytes.  The default size results in about 17MB virtual memory size.
# If this is too big for your system then redefine MEMORY as suggested below.
#
# The memory size given by MEMORY is overridden by
# the command line argument: "-memoryNNNNNNNN"
#
# Normal version (totals about 17 MB virtual)
#MEMORY = 2097152
#
# Smaller versions--take your pick.
# The smallest totals about 5 MB virtual.
# Under Linux, one of the smaller versions may run better--it depends on
# how much RAM and swap space your machine has.
#MEMORY = 1048576
#MEMORY = 524288
#=====================================================================

PROGRAM = wild_life

XSRC = xpred.c xdisplaylist.c
XOBJ = xpred.o xdisplaylist.o
XINC = xpred.h xdisplaylist.h

LIBSOURCE = \
	arity.c bi_math.c bi_sys.c bi_type.c built_ins.c copy.c error.c \
	hash_table.c info.c interrupt.c lefun.c lib.c list.c login.c \
	lub.c memory.c modules.c parser.c print.c raw.c sys.c templates.c \
	token.c trees.c types.c $(XSOURCE) $(DBMSOURCE)

LIBOBJECT = \
	arity.o bi_math.o bi_sys.o bi_type.o built_ins.o copy.o error.o \
	hash_table.o info2.o interrupt.o lefun.o lib2.o list.o login.o \
	lub.o memory.o modules.o parser.o print.o raw.o sys.o templates.o \
	token.o trees.o types.o $(XOBJECT) $(DBMOBJECT)

LIBINCLUDE = def_const.h def_glob.h def_macro.h def_proto.h \
	defs.h def_struct.h


REINCLUDE = regexp.h

PROGINCLUDE = $(LIBINCLUDE)
PROGSOURCE  = $(LIBSOURCE) life.c
PROGOBJECT  = $(LIBOBJECT) life.o

PROGSOURCELIFE = built_ins.lf term_expansion.lf onlinedoc.lf

MANPAGE = $(PROGRAM).1

LIBA = c_life.a

# Please don't change this so we can tell which release you are using.
# DATE        = Fri Jul  7 13:57:49 PDT 1995
# VERSION     = 1.02
# Saved above to use above for test suite
DATE = `date`
VERSION     = 2.33



# all: $(LIBA) $(PROGRAM) .set_up Demo
all: $(LIBA)  $(PROGRAM)
	@echo
	@echo "Life generation done"
	@echo

Demo:
	cd ../Demo; $(MAKE) SETUPDIR="$(SETUPDIR)"

regexp/regexp.o regexp/regsub.o: RegExp
	@echo "Regular Expression Package: $@ done"
RegExp:
	cd regexp; $(MAKE) r CC="$(CC)" CFLAGS="$(CFLAGS)"

$(PROGRAM): $(LIBA) life2.o
	$(CC) -o $(PROGRAM) life2.o $(LIBA) $(LDFLAGS) $(LIBS) $(XLIBRARIES)

.set_up: .set_up_basis
	sed -e "s%+SETUPDIR+%$(SETUPDIR)%g" < .set_up_basis > .set_up

clean:
	-rm *~ *.o *.a .set_up life2.c lib2.c $(PROGRAM)
	cd regexp; $(MAKE) clean

veryclean: clean
	-rm *.cache *.status Makefile
	cd regexp; $(MAKE) veryclean

life2.c: life.c Makefile
	sed -e "s%+SETUP+%$(SETUP)%" < life.c > life2.c

lib2.c:	lib.c Makefile
	sed -e "s%+SETUP+%$(SETUP)%" < lib.c > lib2.c

info2.c: $(PROGSOURCE) $(PROGINCLUDE) Makefile
	@echo Don\'t be surprised, info is compiled "for ${DATE} because" something has changed
	sed -e "s/+DATE+/$(DATE)/" \
	    -e "s/+VERSION+/$(VERSION)/" < info.c > info2.c

$(LIBA): regexp/regexp.o regexp/regsub.o $(LIBOBJECT)
	-rm -f $(LIBA)
	ar cr $(LIBA) $(LIBOBJECT) regexp/regexp.o regexp/regsub.o
	$(RANLIB) $(LIBA)

xpred.o: xpred.c
	$(XCOMPILE.c) -o $@ $<

xdisplaylist.o: xdisplaylist.c
	$(XCOMPILE.c) -o $@ $<

MKINSTALL = $(MAKE) MKDIRHIER="$(MKDIRHIER)"\
 libdir="$(libdir)" INSTALL="$(INSTALL)" srcdir="$(srcdir)" install

install: $(PROGRAM) .set_up
	test -d $(bindir)          || $(MKDIRHIER) $(bindir)
	$(INSTALL) -m 755 $(PROGRAM) $(bindir)/$(PROGRAM)
	test -d $(libdir)          || $(MKDIRHIER) $(libdir)
	test -d $(libdir)/Source   || $(MKDIRHIER) $(libdir)/Source
	$(INSTALL) -m 744 .set_up           $(libdir)/Source
	$(INSTALL) -m 744 built_ins.lf      $(libdir)/Source
	$(INSTALL) -m 744 onlinedoc.lf      $(libdir)/Source
	$(INSTALL) -m 744 term_expansion.lf $(libdir)/Source
	cd $(srcdir)/../Lib;      $(MKINSTALL)
	cd $(srcdir)/../Tools;    $(MKINSTALL)
	cd $(srcdir)/../Examples; $(MKINSTALL)
	cd $(srcdir)/../Doc;      $(MKINSTALL)
	cd $(srcdir)/../Demo;     $(MKINSTALL)

#=====================================================================
# Dependencies

depend:
	$(MKDEP) $(CPPFLAGS) $(DEFS) $(INCLUDE_PATHS) $(PROGSOURCE)
