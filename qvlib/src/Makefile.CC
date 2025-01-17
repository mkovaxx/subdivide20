SHELL = /bin/sh
CC= CC

DEFS =
DEBUG = 
OPTS =  -ansi -fullwarn -woff 1209 -MDupdate Makedepend.CC

INCS = -I ../include

CCFLAGS=$(INCS) $(DEFS) $(DEBUG) $(OPTS)

libqv.a: QvLib.o
	@echo COMPILER $(CC)
	ar csr libqv.a QvLib.o

clean:
	rm -f *.o *.a Makedepend.CC; touch Makedepend.CC;

##############################################################################
# extra default rules
##############################################################################

.SUFFIXES: .cpp 

.cpp.o:
	$(CC) $(CCFLAGS) -c $<

.IGNORE:    

# DO NOT DELETE THIS LINE -- make depend depends on it.

include Makedepend.CC
