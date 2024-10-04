# "C" Makefile

CC = lcc
GAMENAME = BIWTH

# all files in src/ recursively
SRC = $(shell find src -type f -name '*.c')

# obj files o to bin/
OBJ = $(patsubst src/%.c,%.o,$(SRC))
# remove all file directories from OBJ files (so its in ./ instead of sprites-gb/)
OBJ := $(notdir $(OBJ))

BIN = bin
BUILD = build
SRC_DIR = src

CFLAGS = -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG

gb: 
	@echo "Compiling objects with $(CC)..."
	$(CC) -c $(CFLAGS) $(SRC) -o $(OBJ) -Isrc/sprites-gb

	@echo "Linking..."
	$(CC) $(OBJ) -o $(BUILD)/$(GAMENAME).gb $(CFLAGS)

gbc: 
	@echo "Compiling objects with $(CC)..."
	$(CC) -c $(CFLAGS) $(SRC) -o $(OBJ) -Isrc/sprites-gb

	@echo "Linking..."
	$(CC) $(OBJ) -o $(BUILD)/$(GAMENAME).gbc $(CFLAGS)

# if arg is "clean"
clean:
	@echo "Cleaning up..."
	rm -f *.o
	rm -f *.lst
	rm -f *.map
	rm -f *.sym
	rm -f $(BUILD)/$(GAMENAME).gb
	rm -f $(BUILD)/$(GAMENAME).gbc