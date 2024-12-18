# Variables
QJS_DIR := lib/quickjs
OBJ_DIR := $(QJS_DIR)/.obj
QJS_LIB := $(OBJ_DIR)/libquickjs.a

LH_DIR := lib/litehtml
LH_BUILD_DIR := $(LH_DIR)/build
LH_LIB := $(LH_BUILD_DIR)/liblitehtml.a

MAIN_SRC := src/main.cpp
MAIN_EXE := main

# Default target
all: $(MAIN_EXE)

# Step 1: Compile QuickJS to a static library
$(QJS_LIB):
	@echo "Building QuickJS static library..."
	@mkdir -p $(OBJ_DIR)
	@cd $(QJS_DIR) && \
		grep -q "libquickjs.a" Makefile || \
		echo -e "\nlibquickjs.a: quickjs.o cutils.o libregexp.o libbf.o\n\tar rcs libquickjs.a quickjs.o cutils.o libregexp.o libbf.o" >> Makefile
	@cd $(QJS_DIR) && make libquickjs.a
	@mv $(QJS_DIR)/libquickjs.a $(QJS_LIB)

# Step 2: Compile LiteHTML to a static library
$(LH_LIB):
	@echo "Building LiteHTML static library..."
	@mkdir -p $(LH_BUILD_DIR)
	@cd $(LH_BUILD_DIR) && cmake .. && make

# Step 3: Compile main.cpp into the main executable
$(MAIN_EXE): $(MAIN_SRC) $(QJS_LIB) $(LH_LIB)
	@echo "Compiling main.cpp..."
	@g++ -std=c++17 $(MAIN_SRC) -I$(QJS_DIR) -I$(LH_DIR)/include -L$(OBJ_DIR) -L$(LH_BUILD_DIR) -lquickjs -llitehtml -o $(MAIN_EXE)

# Clean up build artifacts
clean:
	@echo "Cleaning up..."
	@rm -f $(MAIN_EXE)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LH_BUILD_DIR)

.PHONY: all clean
