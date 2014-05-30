# Project: birdie24-compo
include $(TOPDIR)/config.mk

SRCFILES	=	$(wildcard *.tmx)
OBJFILES	=	$(SRCFILES:.tmx=.ldmz)

.PHONY: all clean 

all: $(OBJFILES)
	@$(CP) *.ldmz "$(TOPDIR)/res/"
	@$(MV) *.ldmz "$(TOPDIR)/bin/res/"
	@echo "Done."
	@echo 

	
clean:

%.ldmz: %.tmx
	@echo " [LDMZ] map/$<"
	@$(LDMZ) "$<" "$@"
	
