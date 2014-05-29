# Project: birdie24-compo
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR
include config.mk

.PHONY: all lib install strip clean

all:
	@echo " [INIT] bin/"
	@$(MKDIR) bin/
	@echo " [ CD ] src/"
	+@make -C src/
	
	@echo "Build complete."
	@echo 

bin:
	@$(MKDIR) bin/
	@echo " [ CD ] src/"
	+@make -C src/
	
clean:
	@echo " [ RM ] bin/"
	+@$(RM) bin/
	@echo " [ CD ] src/"
	+@make -C src/ clean
	@echo
	@echo "Source tree cleaned."
	@echo

windows-release: all
	@cp /usr/i586-mingw32msvc/lib/libdarnit.dll bin/

strip:
	@echo " [STRP] bin/"
	@strip $(LIB)
	
