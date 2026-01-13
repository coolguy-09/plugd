include ../mkconf

SRC = daemon.c plugd.c other.c debug.c
OBJ = $(SRC:.c=.o)

all: $(OBJ)

%.o: %.c
	@echo "  CC      src/$@"
	@$(CC) $(CFLAGS) -I../include/ -c $< -o $@

clean:
	@echo "  RM      src/*.o"
	@rm -f *.o

.PHONY: clean


