# Project: birdie24-compo
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR
include config.mk

.PHONY: all lib install strip clean

all:
	@echo " [INIT] bin/"
	@$(MKDIR) bin/
	@echo " [INIT] bin/res/"
	@$(CP) -R res bin/
	@$(CP) README.md bin/
	@$(CP) src/run.sh bin/
	@echo " [ CD ] src/"
	+@make -C src/
	@echo " [ CD ] map/"
	+@make -C map/
	
	
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
	+@$(RM) res/*.ldmz
	@echo
	@echo "Source tree cleaned."
	@echo

windows-release: all
	@cp /usr/i586-mingw32msvc/lib/libdarnit.dll bin/

strip:
	@echo " [STRP] bin/"
	@strip $(LIB)
	
