  UTILDIR := util/
  INCLUDEDIR := analysis/
  SRC_DIR := Root/
  OBJ_DIR := obj/
  SRC_FILES := $(wildcard $(SRC_DIR)/*.cxx)
  OBJ_FILES := $(patsubst $(SRC_DIR)/%.cxx,$(OBJ_DIR)/%.o,$(SRC_FILES))
  
  # the compiler: gcc for C program, define as g++ for C++
  CC = g++

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -g -Wall -I$(INCLUDEDIR) `root-config --cflags`
  
  # this is needed to create the bin directory if it doesn't exist
  _dummy := $(shell mkdir -p bin/ obj/)

  all: AnalTDCUS
  
  AnalTDCUS: $(OBJ_FILES)
	  $(CC) $(CFLAGS) -o bin/$@ $^ $(UTILDIR)/$@.cxx `root-config --libs`
  
  $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	  $(CC) $(CFLAGS) -c -o $@ $<

  clean:
	  rm bin/*
	  rm obj/*
