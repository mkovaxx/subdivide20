##############################################################################
#
# Makefile for examples
#
#
# $Id: Makefile.CC,v 1.13 2000/08/06 14:01:31 biermann Exp $
# $Source: /tools/cvs-repos/sig00code/nyuSub/examples/Makefile.CC,v $
#
##############################################################################


##############################################################################
# This file defines the following targets:
# ===========================================================================
# <null target> or all: makes your application.
# 
# clean: removes all the usual stuff (if your Makefile is checked in, it will
#        be removed too... you might then want to do a "co Makefile")
#
############################################################################## 

SHELL = /bin/sh
##############################################################################
# Application file definitions -- EDIT THESE
##############################################################################

# You may need to make sub-definitions for these definitions if you have
# multiple languages, or other requirements.

# All your source files
SWSRCS=

SRCS=$(SWSRCS) $(ARCHSRCS)

# C-language source, to compile with cc
SWCSRCS= 

CSRCS=$(SWCSRCS) $(ARCHCSRCS)
# source files with no associated .o 
SRCSNOO=

HDRSNOS= 

# sources that are #include'd, and have no associated .o
TABLESRCS=

ALLSRCS=$(SRCSNOH) $(CSRCS) $(SRCS) $(SRCSNOO) $(TABLESRCS) 

# All the local .h files (these are not automatically depended)
HDRS=$(HDRSNOS) ${SRCS:.cpp=.h} ${CSRCS:.c=.h} ${SRCSNOO:.cpp=.h}

# All .o files generated
COBJS=${CSRCS:.c=.o}
OBJS= $(COBJS) ${SRCS:.cpp=.o}
ALLOBJS=$(OBJS) ${SRCSNOH:.cpp=.o}
PROTOOBJS = ${PROTOS:.cpp=.o}

# Man pages, etc. (leave blank if none)
DOCS=




##############################################################################
# Application-specific definitions  -- EDIT THESE
##############################################################################

DEFS=

#include directories
INCS= -I. -I../include -I../template -I../viewer

# other C and C++ common options

# CC options

INOPTS=-g
#-DNDEBUG -Ofast 
DEBUGOPTS=  


OPTS=  $(INOPTS) $(DEBUGOPTS) -ptused  -MDupdate Makedepend

# C specific options
COPTS=

# C++ specific options
CCOPTS= -fullwarn -ansi 


# linker options

LIBS=   -L../../qvlib/src -L../src  -lsub -lqv  -lm

VIEWERLIBS= -L../viewer -lviewer  -lglut -lGLU -lGL -lGLcore -lX11 -lXmu -lm 

# ----------------------------------------------------------------------------
# -- You shouldn't need to edit anything below here, unless you have more
# -- than one target or are building a library, or you're doing something
# -- nonstandard.
# ----------------------------------------------------------------------------

##############################################################################
# Language definitions
##############################################################################

CFLAGS= $(INCS) $(DEFS) $(DEBUG) $(OPTS) $(COPTS) 
CCFLAGS=$(INCS) $(DEFS) $(DEBUG) $(OPTS) $(CCOPTS)

CC=CC
cc=cc

TARGET_COMPILER_FLAGS=$(CCFLAGS) $(LIBOPTS)
TARGET_COMPILER=$(CC)

##############################################################################
# extra default rules
##############################################################################

.SUFFIXES: .C .cpp .c .sl .slo

.cpp:
	$(CC) $(CCFLAGS) -o $@ $<

.C:
	$(CC) $(CFLAGS) -o $@ $<

.c:
	$(cc) $(CFLAGS) -o $@ $<

.C.o:
	$(CC) $(CCFLAGS) -c $<

.cpp.o:
	$(CC) $(CCFLAGS) -c $<

.c.o:
	$(cc) $(CFLAGS) -c $<

.sl:
	$(SHADECOM) $@ $<

.sl.slo:
	$(SHADECOM) $<



TARGETS=subviewer loopsub ccsub
all: qvlib libsub libviewer $(TARGETS)


##############################################################################
# Application-specific rules and dependencies - may need editing
##############################################################################
qvlib:
	cd ../../qvlib/src; make -f Makefile.CC CC=$(CC) cc=$(cc)
libsub:
	cd ../src; make -f Makefile.CC CC=$(CC) cc=$(cc)

libviewer:
	cd ../viewer; make -f Makefile.CC CC=$(CC) cc=$(cc)


subviewer: subviewer.o ../viewer/libviewer.a ../src/libsub.a
	@echo Compiling $@
	$(TARGET_COMPILER) -o subviewer $(TARGET_COMPILER_FLAGS) \
	subviewer.o $(ALLOBJS)  $(VIEWERLIBS) $(LIBS)
	@echo $@ is made.


loopsub: loopsub.o  ../viewer/libviewer.a ../src/libsub.a
	@echo Compiling $@
	$(TARGET_COMPILER) -o loopsub $(TARGET_COMPILER_FLAGS) \
	loopsub.o $(ALLOBJS) $(LIBS)
	@echo $@ is made.

test: test.o  ../viewer/libviewer.a
	@echo Compiling $@
	$(TARGET_COMPILER) -o test $(TARGET_COMPILER_FLAGS) \
	test.o $(ALLOBJS) $(VIEWERLIBS) $(LIBS)
	@echo $@ is made.

subdiv_array: subdiv_array.o  ../viewer/libviewer.a
	@echo Compiling $@
	$(TARGET_COMPILER) -o subdiv_array $(TARGET_COMPILER_FLAGS) \
	subdiv_array.o $(ALLOBJS) $(VIEWERLIBS) $(LIBS)
	@echo $@ is made.

ccsub: ccsub.o  ../viewer/libviewer.a ../src/libsub.a
	@echo Compiling $@
	$(TARGET_COMPILER) -o ccsub $(TARGET_COMPILER_FLAGS) \
	ccsub.o $(ALLOBJS) $(LIBS)
	@echo $@ is made.

##############################################################################
# General-purpose targets - do not edit, in general:
##############################################################################

clean:	
	rm -f $(TARGETS) $(OBJS) *.slo *.o *~ *.s \
		*.a *..c ptrepository/* TAGS \
		core a.out \#* *.bak *.BAK *.CKP \
		*.l *.Addrs *.Counts *.pixie .\#* \
		loopsub ccsub; 
	rm -rf ii_files; 
	rm -f Makedepend.CC; touch Makedepend.CC

.IGNORE:    

# DO NOT DELETE THIS LINE -- make depend depends on it.

include Makedepend.CC
