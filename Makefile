CC = gcc
LD = $(CC)
CFLAGS = -g -Wall -O2 -I./src/
LDFLAGS = 

SRC = src/daemon.c src/plugd.c
OBJ = $(SRC:.c=.o)

PREFIX ?= /usr/local
SBIN = $(PREFIX)/sbin

OUT = plugd

src/%.o: src/%.c
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@echo "  LD      $@.unstripped"
	@$(LD) $(LDFLAGS) -o $@.unstripped $^
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
	@echo "  RM      src/*.o $(OUT) $(OUT).unstripped"
	@rm -f src/*.o $(OUT) $(OUT).unstripped


