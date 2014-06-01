# Project: birdie24-compo
# Makefile configurations

NAME		=	BondeN^wArs
VERSION		=	0.1

BIN		=	"$(TOPDIR)/bin/$(NAME)"
PREFIX		=	/usr/local
DATAPATH	=	"/usr/share/games/BondeN^wArs"
APPLICATIONSPATH=	/usr/share/applications

DBGFLAGS	=	-O0 -g -D__DEBUG__
#DBGFLAGS	=	-O3 -g
CFLAGS		+=	-Wall $(INCS) $(DBGFLAGS)
LDFLAGS		+=	-ldarnit -lm
RM		=	rm -fR
MKDIR		=	mkdir -p
CP		=	cp
MV		=	mv
LDMZ	=	darnit-tmxconv

ifneq ($(wildcard /etc/debian_version),) 
	#Debian packaging
	ARCH	:=	$(shell dpkg-architecture -qDEB_BUILD_ARCH)
	DEB	:=	$(NAME)-$(VERSION)$(ARCH)
	PACKAGE	:=	$(DEB).deb
	SECTION	:=	games
	DEPS	=	libc6, libsdl1.2debian, libbz2-1.0, zlib1g
endif

ifeq ($(strip $(OS)), Windows_NT)
	LDFLAGS	+=	-lws2_32 -mwindows
	BIN	=	"$(TOPDIR)/bin/$(NAME).exe"
else
ifeq ($(BUILDFOR), WIN32)
	CC	=	i586-mingw32msvc-gcc
	STRIP	=	i586-mingw32msvc-strip
	AR	=	i586-mingw32msvc-ar
	LDFLAGS	+=	-lws2_32
	BIN	=	"$(TOPDIR)/bin/$(NAME).exe"
else
	LDFLAGS	+=	-lpthread
ifeq ($(strip $(SBOX_UNAME_MACHINE)), arm)
	#Maemo packaging
	PACKAGE	:=	$(NAME)-$(VERSION)maemo.deb
	SECTION	:=	user/games
	DEPS	=	libc6, libsdl-gles1.2-1, libbz2-1.0, zlib1g
endif
endif
endif
