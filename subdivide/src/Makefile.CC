##############################################################################
#
# Makefile for libsub
#
#
# $Id: Makefile.CC,v 1.11 2001/02/07 05:55:26 biermann Exp $
# $Source: /tools/cvs-repos/sig00code/nyuSub/src/Makefile.CC,v $
#
##############################################################################

SHELL = /bin/sh
##############################################################################
# Application file definitions -- EDIT THESE
##############################################################################

# You may need to make sub-definitions for these definitions if you have
# multiple languages, or other requirements.

# All your source files
SWSRCS= flatmesh.cpp     \
	tagflatmesh.cpp  \
	trirule.cpp      \
	quadrule.cpp     \
	ivgraph.cpp      \
	tagivgraph.cpp   \
	subtri.cpp       \
	subquad.cpp      \
	trimesh.cpp      \
	quadmesh.cpp	 \
	quadmanipulator.cpp \
	trimanipulator.cpp

SRCS=$(SWSRCS) $(ARCHSRCS)

# C-language source, to compile with cc
SWCSRCS=

CSRCS=$(SWCSRCS) $(ARCHCSRCS)
# source files with no associated .o 
SRCSNOO=


# source files with no associated .h 
SRCSNOH=

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

# Application name
TARGET= libsub.a

DEFS = 

#include directories
INCS = -I. -I../include -I../template -I../../qvlib/include

# other C and C++ common options

# CC options
INOPTS= 
#-DNDEBUG -O3
DEBUGOPTS= -g


OPTS=  $(INOPTS) $(DEBUGOPTS)   -MDupdate Makedepend.CC

# C specific options
COPTS=

# C++ specific options
CCOPTS= -ansi -fullwarn -no_auto_include

# linker options
LIBS= 

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


# This rule must be first.  It insures that the desired target is the default.

all: $(TARGET)


##############################################################################
# Application-specific rules and dependencies - may need editing
##############################################################################

$(TARGET): $(ALLOBJS)
	@echo COMPILER $(CC)
	CC -ar  -o $(TARGET) $(ALLOBJS)
	@echo $@ is made.

##############################################################################
# General-purpose targets - do not edit, in general:
##############################################################################

clean:	
	rm -f $(TARGET) $(OBJS) *.slo *.o *~ *.s \
		*.a *..c ptrepository/* TAGS \
		core a.out \#* *.bak *.BAK *.CKP \
		*.l *.Addrs *.Counts *.pixie .\#*; 
	rm -rf ii_files;
	rm -f Makedepend.CC; touch Makedepend.CC

.IGNORE:    

# DO NOT DELETE THIS LINE -- make depend depends on it.

include Makedepend.CC









