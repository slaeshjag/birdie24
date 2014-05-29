# Project: birdie24-compo
include $(TOPDIR)/config.common.mk

ifeq ($(strip $(OS)), Windows_NT)
	DEPENDS	+=	compo.res
endif

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ LD ] $(LIB)"
	@$(CC) $(CFLAGS) $(OBJFILES) -o $(LIB) $(LDFLAGS)
	@echo "Done."
	@echo
	
clean:
	@echo
	@echo " [ RM ] $(OBJFILES)"
	@$(RM) $(OBJFILES)
	@$(RM) compo.res
	@echo "Done."
	@echo 

%.o: %.c %.h
	@echo " [ CC ] src/$<"
	@$(CC) $(CFLAGS) -c -o $@ $<
	
darnit.res:
	@windres compo.rc -O coff -o compo.res

