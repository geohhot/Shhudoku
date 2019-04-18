
PRG = sudokku
LIBS_PATH = ../EasyCursorControl/lib/ ../lib

SRC = sudokku.c
EXT_SRC = $(wildcard ../EasyCursorControl/lib/*.c)
EXT_OBJ = $(EXT_SRC:.c=.o)
OBJ = $(SRC:.c=.o)
OBJ += $(EXT_OBJ)

CFLAGS = -pthread $(addprefix -I, $(LIBS_PATH))
LFLAGS = -pthread

.PHONY: all clean

all: $(PRG)

$(PRG): $(OBJ)
	gcc $(LFLAGS) $^ -o $@

.c.o:
	@echo "Compiling" $< ".."
	gcc -c $(CFLAGS) $< -o $@


clean:
	echo $(OBJ)
	rm -rf $(SRC:.c=.o) $(PRG)
