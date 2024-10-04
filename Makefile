# "C" Makefile
# check for devkitpro if target is gba
ifeq ($(TARGET),gba)
	ifeq ($(DEVKITPRO),)
	$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=/path/to/devkitpro")
	endif
	ifeq ($(DEVKITARM),)
	$(error "Please set DEVKITARM in your environment. export DEVKITARM=/path/to/devkitarm")
	endif
endif

CC = lcc
GAMENAME = BIWTH

# all files in src/ recursively
SRC = $(shell find src -type f -name '*.c')
ifeq ($(TARGET),gba)
	SRC := $(filter-out src/sprites-gb/%,$(SRC))
endif

# obj files o to bin/
OBJ = $(patsubst src/%.c,%.o,$(SRC))
# remove all file directories from OBJ files (so its in ./ instead of sprites-gb/)
OBJ := $(notdir $(OBJ))

BIN = bin
BUILD = build
SRC_DIR = src

CFLAGS = -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG

ifeq ($(TARGET),gba)
	CC = arm-none-eabi-gcc
	CFLAGS = -mthumb -mthumb-interwork -specs=$(DEVKITARM)/arm-none-eabi/lib/gba.specs -I$(DEVKITPRO)/libgba/include -I$(SRC_DIR) -DUSE_SFR_FOR_REG -DGBA
endif

gb: 
	@echo "Compiling objects with $(CC)... with linker flag GB"
	$(CC) -c $(CFLAGS) $(SRC) -o $(OBJ) -Isrc/sprites-gb

	@echo "Linking..."
	$(CC) $(OBJ) -o $(BUILD)/$(GAMENAME).gb $(CFLAGS)

gbc: 
	@echo "Compiling objects with $(CC)..."
	$(CC) -c $(CFLAGS) $(SRC) -o $(OBJ) -Isrc/sprites-gb

	@echo "Linking..."
	$(CC) $(OBJ) -o $(BUILD)/$(GAMENAME).gbc $(CFLAGS)

gba:
	@echo "Compiling objects with $(CC)..."
	$(CC) -c $(CFLAGS) $(SRC) -o $(OBJ)

	@echo "Linking..."
	$(CC) $(OBJ) -o $(BUILD)/$(GAMENAME).gba $(CFLAGS)

# if arg is "clean"
clean:
	@echo "Cleaning up..."
	rm -f *.o
	rm -f *.lst
	rm -f *.map
	rm -f *.sym
	rm -f $(BUILD)/$(GAMENAME).gb
	rm -f $(BUILD)/$(GAMENAME).gbc