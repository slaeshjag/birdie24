# Project: birdie24-compo
include $(TOPDIR)/config.mk

ifeq ($(strip $(OS)), Windows_NT)
	DEPENDS	+=	compo.res
endif

SRCFILES	=	$(wildcard *.c)
OBJFILES	=	$(SRCFILES:.c=.o)
.PHONY: all clean

all: $(OBJFILES) $(DEPENDS)
	@echo " [ LD ] $(BIN)"
	@$(CC) $(CFLAGS) $(OBJFILES) -o $(BIN) $(LDFLAGS)
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
	
compo.res:
	@windres compo.rc -O coff -o compo.res

