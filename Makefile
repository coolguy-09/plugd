MAKEFLAGS += --no-print-directory

include mkconf

SRC = src/daemon.c src/plugd.c src/other.c src/debug.c
OBJ = $(SRC:.c=.o)

PREFIX ?= /usr/local
SBIN = $(PREFIX)/sbin

OUT = plugd

all: $(OUT)

compile:
	@$(MAKE) -C src

$(OUT): compile
	@echo "  LD      $@.unstripped"
	@$(LD) $(LDFLAGS) -o $@.unstripped $(OBJ)
	@echo "  STRIP   $@"
	@cp $@.unstripped $@
	@strip $@

install: $(OUT)
	@echo "  INSTALL $< -> $(DESTDIR)$(SBIN)/$<"
	@install $< $(DESTDIR)$(SBIN)/$<

uninstall:
	@echo "  RM      $(DESTDIR)$(SBIN)/$(OUT)"
	@rm -f $(DESTDIR)$(SBIN)/$(OUT)

clean:
	@$(MAKE) -C src clean
	@echo "  RM      $(OUT) $(OUT).unstripped"
	@rm -f $(OUT) $(OUT).unstripped

.PHONY: compile install uninstall clean
