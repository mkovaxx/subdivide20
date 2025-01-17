##############################################################################
#
# Makefile for Subdivide
#
#
# $Id: Makefile.CC,v 1.8 2000/04/30 00:16:06 biermann Exp $
# $Source: /tools/cvs-repos/sig00code/nyuSub/Makefile.CC,v $
#
##############################################################################
SHELL = /bin/sh

ENV=CC
DIRS= ../qvlib/src src viewer examples

all:
	( for i in $(DIRS); do ( cd $$i; touch Makedepend.$(ENV); make -f Makefile.$(ENV)  ); done )
clean:  
	( for i in $(DIRS); do ( cd $$i; touch Makedepend.$(ENV); make -f Makefile.$(ENV) clean ); done )
