# Project: birdie24-compo
# Makefile configurations

BINNAME		=	compo.elf
BIN		=	$(TOPDIR)/bin/$(BINNAME)

VERSION		=	v.0.1
#DBGFLAGS	=	-O0 -g -D__DEBUG__
DBGFLAGS	=	-O3
#General flags
CFLAGS		+=	-Wall -Wextra -shared -fPIC $(DBGFLAGS)
LDFLAGS		+=	-Wl,-soname,$(BINNAME)

#Makefile tools
RM		=	rm -Rf
MKDIR		=	mkdir -p

include		$(TOPDIR)/config.mk


