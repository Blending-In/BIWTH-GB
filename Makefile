GAMENAME = BIWTH
SRC_DIR = src
BUILD_DIR = build

CC = lcc
CFLAGS = -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -DGB -I$(GBDK_INCLUDE)

ifeq ($(TARGET),gba)
	CC = arm-none-eabi-gcc
	CFLAGS = -mthumb -mthumb-interwork \
		-specs=$(DEVKITARM)/arm-none-eabi/lib/gba.specs \
		-I$(DEVKITPRO)/libgba/include \
		-I$(SRC_DIR) \
		-DUSE_SFR_FOR_REG -DGBA
endif

# Find all C source files (recursively in subfolders)
SRC = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)

# Convert .c -> .o, preserving folder names under build/
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Create build directory tree automatically
$(shell mkdir $(BUILD_DIR) 2>nul)

# Rule for compiling any .c into matching .o in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $< ..."
	@mkdir $(dir $@) 2>nul || true
	$(CC) -c $(CFLAGS) $< -o $@


all: gb

gb: $(OBJ)
	@echo "Linking $(GAMENAME).gb ..."
	$(CC) $(OBJ) -o $(BUILD_DIR)/$(GAMENAME).gb $(CFLAGS)

gbc: $(OBJ)
	@echo "Linking $(GAMENAME).gbc ..."
	$(CC) $(OBJ) -o $(BUILD_DIR)/$(GAMENAME).gbc $(CFLAGS)

gba: $(OBJ)
	@echo "Linking $(GAMENAME).gba ..."
	$(CC) $(OBJ) -o $(BUILD_DIR)/$(GAMENAME).gba $(CFLAGS)

clean:
	@echo "Cleaning up..."
	del /Q $(BUILD_DIR)\*.o 2>nul
	del /Q $(BUILD_DIR)\*.lst 2>nul
	del /Q $(BUILD_DIR)\*.map 2>nul
	del /Q $(BUILD_DIR)\*.sym 2>nul
	del /Q $(BUILD_DIR)\$(GAMENAME).gb 2>nul
	del /Q $(BUILD_DIR)\$(GAMENAME).gbc 2>nul
	del /Q $(BUILD_DIR)\$(GAMENAME).gba 2>nul
