CC=g++  -Wno-write-strings -fpermissive 

# Things you might want to put in ENV and LENV:
# -Dvoid=int		compilers that don't do void
# -DCHARBITS=0377	compilers that don't do unsigned char
# -DSTATIC=extern	compilers that don't like "static foo();" as forward decl
# -DSTRCSPN		library does not have strcspn()
# -Dstrchr=index	library does not have strchr()
# -DERRAVAIL		have utzoo-compatible error() function and friends
# ENV=-Dvoid=int -DCHARBITS=0377 -DSTATIC=extern
# LENV=-Dvoid=int -DCHARBITS=0377

# Things you might want to put in TEST:
# -DDEBUG		debugging hooks
# -I.			regexp.h from current directory, not /usr/include
TEST=-I.

# Things you might want to put in PROF:
# -Dstatic='/* */'	make everything global so profiler can see it.
# -p			profiler
PROF=
# -Wno-write-strings  -fpermissive 
CFLAGS=-O $(ENV) $(TEST) $(PROF)
LINTFLAGS=$(LENV) $(TEST) -ha
LDFLAGS=

OBJ=regexp.o regsub.o
LSRC=regexp.cpp regsub.cpp regerror.cpp
DTR=README dMakefile regexp.3 regexp.h regexp.cpp regsub.cpp regerror.cpp \
	regmagic.h Try.cpp timer.cpp tests
DEST = ..

Try:	Try.o $(OBJ)
	$(CC) $(LDFLAGS) Try.o $(OBJ) -o Try

# Making timer will probably require putting stuff in $(PROF) and then
# recompiling everything; the following is just the final stage.
timer:	timer.o $(OBJ)
	$(CC) $(LDFLAGS) $(PROF) timer.o $(OBJ) -o timer

timer.o:	timer.cpp timer.t.h

timer.t.h:	tests
	sed 's/	/","/g;s/\\/&&/g;s/.*/{"&"},/' tests >timer.t.h

# Regression test.
r:	Try tests
	@echo 'No news is good news...'
	./Try <tests

lint:	timer.t.h
	@echo 'Complaints about multiply-declared regerror() are legit.'
	lint $(LINTFLAGS) $(LSRC) Try.cpp
	lint $(LINTFLAGS) $(LSRC) timer.cpp

regexp.o:	regexp.cpp regexp.h regmagic.h
regsub.o:	regsub.cpp regexp.h regmagic.h

clean:
	rm -f *~ *.o core mon.out timer.t.h dMakefile dtr Try timer

dtr:	r makedtr $(DTR)
	makedtr $(DTR) >dtr

dMakefile:	Makefile
	sed '/^L*ENV=/s/ *-DERRAVAIL//' Makefile >dMakefile

mv:	$(OBJ) regerror.o
	mv $(OBJ) regerror.o $(DEST)

foo: foo.o regexp.o
	$(CC) -o foo foo.o regexp.o

DISTSOURCE = Makefile README Try.cpp regerror.cpp regexp.3 regexp.README \
	regexp.cpp regexp.h regmagic.h regsub.cpp tests timer.cpp

dist:
	test -d $(distdir)/Source/regexp || $(MKDIRHIER) $(distdir)/Source/regexp
	$(CP) $(DISTSOURCE) $(distdir)/Source/regexp/
