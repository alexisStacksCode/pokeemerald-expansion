CXX = g++
STRIP = strip
LDFLAGS :=
CXXFLAGS = -Wall -Wextra -Wconversion -std=c++17 -O2 -g

EXE :=
ifeq ($(OS),Windows_NT)
	EXE := .exe
endif

BINARY = wav2agb$(EXE)

SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

ifneq (,$(RELEASE))
	LDFLAGS += -static
	CXXFLAGS += -flto
endif

.PHONY: clean clean
all: $(BINARY)

clean:
	rm -f $(OBJ_FILES) $(BINARY)

$(BINARY): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
	if [ $(RELEASE)x != x ]; then strip -s $@; fi

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
