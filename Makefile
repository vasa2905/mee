

# directories

BIN_DIR := bin
LIB_SRC_DIR := lib/src
LIB_BIN_DIR := lib/bin
SRC_DIR := src
OBJ_DIR := obj

# names

LIB_NAME := mee
MAIN_NAME := main
PROGRAM := $(BIN_DIR)/$(MAIN_NAME)

# source files

MEE_CPP := $(LIB_SRC_DIR)/$(LIB_NAME).cpp
MAIN_CPP := $(SRC_DIR)/$(MAIN_NAME).cpp

# output files

MEE_SO := $(LIB_BIN_DIR)/$(LIB_NAME).so
MEE_A := $(LIB_BIN_DIR)/$(LIB_NAME).a
MEE_O := $(OBJ_DIR)/$(LIB_NAME).o
MAIN_O := $(OBJ_DIR)/$(MAIN_NAME).o

# compiler flags

CXX = g++
CPPFLAGS = -I $(LIB_SRC_DIR)
CXXFLAGS = -O3 -Wall -std=c++17
LDFLAGS = -lstdc++exp
LDLIBS = -static-libstdc++

all: static shared

# static compilation

static: $(PROGRAM)-ldstatic
	@echo "Static executable ready"

$(PROGRAM)-ldstatic: $(MAIN_O) $(MEE_A) | $(BIN_DIR)
	@echo "Linking static executable"
	$(CXX) $(MAIN_O) $(MEE_A) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

$(MEE_A): $(MEE_O) | $(LIB_BIN_DIR)
	@echo "Making $(MEE_A) static library"
	ar rcs $@ $<

# shared compilation

shared: $(PROGRAM)-ldshared
	@echo "Shared executable ready"

$(PROGRAM)-ldshared: $(MAIN_O) $(MEE_SO) | $(BIN_DIR)
	@echo "Linking shared executable"
	$(CXX) $(MAIN_O) $(MEE_SO) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -Wl,-rpath,$(abspath $(LIB_BIN_DIR)) -o $@

$(MEE_SO): $(MEE_O) | $(LIB_BIN_DIR)
	@echo "Making $(MEE_SO) shared library"
	$(CXX) -shared $(MEE_O) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

# common compilation

$(MAIN_O): $(MAIN_CPP) | $(OBJ_DIR)
	@echo "Making $(MAIN_O) object file"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(MEE_O): $(MEE_CPP) | $(OBJ_DIR)
	@echo "Making $(MEE_O) object file"
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $< -o $@

# make directories

$(OBJ_DIR) $(BIN_DIR) $(LIB_BIN_DIR):
	@echo "Creating directories"
	@mkdir -p $@

clean:
	@echo "Cleaning build"
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_BIN_DIR)

libs: $(MEE_A) $(MEE_SO)
	@echo "Making $(MEE_A) $(MEE_SO) libraries"

.PHONY: static shared libs clean
