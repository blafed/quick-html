# Variables
QJS_DIR := lib/quickjs
OBJ_DIR := $(QJS_DIR)/.obj
QJS_LIB := $(OBJ_DIR)/libquickjs.a

LH_DIR := lib/litehtml
LH_BUILD_DIR := $(LH_DIR)/build
LH_LIB := $(LH_BUILD_DIR)/liblitehtml.a

EWS_DIR := src/dev/easywsclient
EWS_SRC := $(EWS_DIR)/easywsclient.cpp
EWS_OBJ := $(EWS_DIR)/easywsclient.o

TEST_SRC :=
TEST_OBJ :=

MAIN_SRC := src/main.cpp $(EWS_DIR)/easywsclient.hpp
MAIN_OBJ := src/main.o $(EWS_OBJ)
MAIN_EXE := main

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -I$(QJS_DIR) -I$(LH_DIR)/include -Ilib/litehtml/containers/test -pthread
LDFLAGS := -L$(OBJ_DIR) -L$(LH_BUILD_DIR) -lquickjs -llitehtml

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

# Step 3: Compile EasyWSClient implementation
$(EWS_OBJ): $(EWS_SRC)
	@echo "Compiling EasyWSClient implementation: $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Step 4: Compile main.cpp into an object file
src/main.o: src/main.cpp
	@echo "Compiling main.cpp..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Step 5: Link all object files into the main executable
$(MAIN_EXE): $(MAIN_OBJ) $(QJS_LIB) $(LH_LIB)
	@echo "Linking all object files into executable: $@..."
	@$(CXX) $(MAIN_OBJ) $(CXXFLAGS) $(LDFLAGS) -o $@

# Clean up build artifacts
clean:
	@echo "Cleaning up..."
	@rm -f $(MAIN_EXE) $(MAIN_OBJ) $(TEST_OBJ)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LH_BUILD_DIR)

.PHONY: all clean
